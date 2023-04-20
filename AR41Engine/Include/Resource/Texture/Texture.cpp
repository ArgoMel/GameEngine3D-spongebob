#include "Texture.h"
#include "../../PathManager.h"
#include "../../Device.h"
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif // _DEBUG

CTexture::CTexture()
	: m_Scene(nullptr)
	, m_ArraySRV(nullptr)
	, m_ImageType(EImageType::Atlas)
{
	SetTypeID<CTexture>();
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_ArraySRV);
	size_t	size = m_vecTextureInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}
}

bool CTexture::CreateResource(int index)
{
	if (FAILED(DirectX::CreateShaderResourceView(CDevice::GetInst()->GetDevice(), 
		m_vecTextureInfo[index]->image->GetImages(), m_vecTextureInfo[index]->image->GetImageCount(), 
		m_vecTextureInfo[index]->image->GetMetadata(), &m_vecTextureInfo[index]->SRV)))
	{
		return false;
	}
	m_vecTextureInfo[index]->width = (unsigned int)m_vecTextureInfo[index]->image->GetImages()[0].width;
	m_vecTextureInfo[index]->height = (unsigned int)m_vecTextureInfo[index]->image->GetImages()[0].height;
	return true;
}

bool CTexture::CreateResourceArray()
{
	DirectX::ScratchImage* imageArray = new DirectX::ScratchImage;
	size_t	mipLevel = m_vecTextureInfo[0]->image->GetMetadata().mipLevels;
	size_t	count = m_vecTextureInfo.size();
	if (FAILED(imageArray->Initialize2D(m_vecTextureInfo[0]->image->GetMetadata().format,
		m_vecTextureInfo[0]->image->GetMetadata().width, m_vecTextureInfo[0]->image->GetMetadata().height,
		count, mipLevel)))
	{
		return false;
	}
	for (size_t i = 0; i < count; ++i)
	{
		const DirectX::Image* images = m_vecTextureInfo[i]->image->GetImages();
		mipLevel = m_vecTextureInfo[i]->image->GetMetadata().mipLevels;
		for (size_t j = 0; j < mipLevel; ++j)
		{
			const DirectX::Image* src = &imageArray->GetImages()[i * mipLevel + j];
			const DirectX::Image* dest = &images[j];
			memcpy(src->pixels, dest->pixels, src->slicePitch);
		}
		CreateResource((int)i);
	}
	ID3D11Texture2D* texture = nullptr;
	if (FAILED(DirectX::CreateTextureEx(CDevice::GetInst()->GetDevice(), imageArray->GetImages(),
		imageArray->GetImageCount(), imageArray->GetMetadata(), D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 0, DirectX::CREATETEX_FLAGS::CREATETEX_DEFAULT, (ID3D11Resource**)&texture)))
	{
		return false;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC	desc = {};
	desc.Format = m_vecTextureInfo[0]->image->GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	desc.Texture2DArray.MostDetailedMip = 0;
	desc.Texture2DArray.MipLevels = (UINT)m_vecTextureInfo[0]->image->GetMetadata().mipLevels;
	desc.Texture2DArray.FirstArraySlice = 0;
	desc.Texture2DArray.ArraySize = (UINT)count;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(texture, &desc, &m_ArraySRV)))
	{
		return false;
	}
	SAFE_DELETE(imageArray);
	SAFE_RELEASE(texture);
	return true;
}

bool CTexture::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		lstrcpy(fullPath, path->path);
	}
	lstrcat(fullPath, fileName);
	return LoadTextureFullPath(name, fullPath);
}

bool CTexture::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	SetName(name);
	TextureResourceInfo* info = new TextureResourceInfo;
	int	pathLength = lstrlen(fullPath);
	for (int i = pathLength - 1; i >= 0; --i)
	{
		// D:\41th\DirectX\Client\Bin\Texture\Test.png
		if ((fullPath[i] == '\\' || fullPath[i] == '/') && i >= 4)
		{
			if ((fullPath[i - 1] == 'n' || fullPath[i - 1] == 'N') &&
				(fullPath[i - 2] == 'i' || fullPath[i - 2] == 'I') &&
				(fullPath[i - 3] == 'b' || fullPath[i - 3] == 'B') &&
				(fullPath[i - 4] == '\\' ||fullPath[i - 4] == '/'))
			{
				lstrcpy(info->fileName, &fullPath[i + 1]);
				break;
			}
		}
	}
	strcpy_s(info->pathName, strlen(ROOT_PATH) + 1, ROOT_PATH);
	TCHAR	_FileExt[_MAX_EXT] = {};		// .dds, .tga, 나머지 포맷에 따라 로딩 함수가 다르다.
	_wsplitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, _FileExt, _MAX_EXT);
	char	fileExt[_MAX_EXT] = {};
#ifdef UNICODE
	int	convertLength = WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, fileExt, convertLength, nullptr, nullptr);
#else
	strcpy_s(fileExt, _FileExt);
#endif // UNICODE
	_strupr_s(fileExt);		// 확장자를 대문자로 만들어준다.
	DirectX::ScratchImage* image = new DirectX::ScratchImage;
	if (strcmp(fileExt, ".DDS") == 0)
	{
		if (FAILED(DirectX::LoadFromDDSFile(fullPath, DirectX::DDS_FLAGS_NONE, nullptr,	*image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			return false;
		}
	}
	else if (strcmp(fileExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(fullPath, nullptr, *image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			return false;
		}
	}
	//else if (strcmp(fileExt, ".GIF") == 0)
	//{
	//	DirectX::TexMetadata data;
	//	if (FAILED(DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_ALL_FRAMES, &data, *image)))
	//	{
	//		SAFE_DELETE(info);
	//		SAFE_DELETE(image);
	//		return false;
	//	}
	//}
	else
	{
		if (FAILED(DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_NONE, nullptr,*image)))
		{
			SAFE_DELETE(info);
			SAFE_DELETE(image);
			return false;
		}
	}
	info->image = image;
	m_vecTextureInfo.push_back(info);
	return CreateResource(0);
}

bool CTexture::LoadTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	std::vector<const TCHAR*>	vecFullPath;
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	size_t	size = vecFileName.size();
	for (size_t i = 0; i < size; ++i)
	{
		TCHAR* fullPath = new TCHAR[MAX_PATH];
		memset(fullPath, 0, sizeof(TCHAR) * MAX_PATH);
		if (path)
		{
			lstrcpy(fullPath, path->path);
		}
		lstrcat(fullPath, vecFileName[i]);
		vecFullPath.push_back(fullPath);
	}
	bool result = LoadTextureFullPath(name, vecFullPath);
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE_ARRAY(vecFullPath[i]);
	}
	return result;
}

bool CTexture::LoadTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	SetName(name);
	m_ImageType = EImageType::Frame;
	size_t	size = vecFullPath.size();
	for (size_t i = 0; i < size; ++i)
	{
		TextureResourceInfo* info = new TextureResourceInfo;
		int	pathLength = lstrlen(vecFullPath[i]);
		for (int j = pathLength - 1; j >= 0; --j)
		{
			if ((vecFullPath[i][j] == '\\' || vecFullPath[i][j] == '/') && j >= 4)
			{
				if ((vecFullPath[i][j - 1] == 'n' || vecFullPath[i][j - 1] == 'N') &&
					(vecFullPath[i][j - 2] == 'i' || vecFullPath[i][j - 2] == 'I') &&
					(vecFullPath[i][j - 3] == 'b' || vecFullPath[i][j - 3] == 'B') &&
					(vecFullPath[i][j - 4] == '\\' || vecFullPath[i][j - 4] == '/'))
				{
					lstrcpy(info->fileName, &vecFullPath[i][j + 1]);
					break;
				}
			}
		}
		strcpy_s(info->pathName, strlen(ROOT_PATH) + 1, ROOT_PATH);
		TCHAR	_FileExt[_MAX_EXT] = {};
		_wsplitpath_s(vecFullPath[i], nullptr, 0, nullptr, 0, nullptr, 0, _FileExt, _MAX_EXT);
		char	fileExt[_MAX_EXT] = {};
#ifdef UNICODE
		int	convertLength = WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, fileExt, convertLength, nullptr, nullptr);
#else
		strcpy_s(fileExt, _FileExt);
#endif // UNICODE.
		_strupr_s(fileExt);
		DirectX::ScratchImage* image = new DirectX::ScratchImage;
		if (strcmp(fileExt, ".DDS") == 0)
		{
			if (FAILED(DirectX::LoadFromDDSFile(vecFullPath[i], DirectX::DDS_FLAGS_NONE, nullptr, *image)))
			{
				SAFE_DELETE(info);
				SAFE_DELETE(image);
				return false;
			}
		}
		else if (strcmp(fileExt, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(vecFullPath[i], nullptr, *image)))
			{
				SAFE_DELETE(info);
				SAFE_DELETE(image);
				return false;
			}
		}
		else
		{
			if (FAILED(DirectX::LoadFromWICFile(vecFullPath[i], DirectX::WIC_FLAGS_NONE, nullptr, *image)))
			{
				SAFE_DELETE(info);
				SAFE_DELETE(image);
				return false;
			}
		}
		info->image = image;
		m_vecTextureInfo.push_back(info);
		if (!CreateResource((int)i))
		{
			return false;
		}
	}
	return true;
}

bool CTexture::LoadTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	std::vector<const TCHAR*>	vecFullPath;
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	size_t	size = vecFileName.size();
	for (size_t i = 0; i < size; ++i)
	{
		TCHAR* fullPath = new TCHAR[MAX_PATH];
		memset(fullPath, 0, sizeof(TCHAR) * MAX_PATH);
		if (path)
		{
			lstrcpy(fullPath, path->path);
		}
		lstrcat(fullPath, vecFileName[i]);
		vecFullPath.push_back(fullPath);
	}
	bool result = LoadTextureArrayFullPath(name, vecFullPath);
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE_ARRAY(vecFullPath[i]);
	}
	return result;
}

bool CTexture::LoadTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	SetName(name);
	m_ImageType = EImageType::Array;
	size_t	size = vecFullPath.size();
	for (size_t i = 0; i < size; ++i)
	{
		TextureResourceInfo* info = new TextureResourceInfo;
		int	pathLength = lstrlen(vecFullPath[i]);
		for (int j = pathLength - 1; j >= 0; --j)
		{
			if ((vecFullPath[i][j] == '\\' || vecFullPath[i][j] == '/') && j >= 4)
			{
				if ((vecFullPath[i][j - 1] == 'n' || vecFullPath[i][j - 1] == 'N') &&
					(vecFullPath[i][j - 2] == 'i' || vecFullPath[i][j - 2] == 'I') &&
					(vecFullPath[i][j - 3] == 'b' || vecFullPath[i][j - 3] == 'B') &&
					(vecFullPath[i][j - 4] == '\\' || vecFullPath[i][j - 4] == '/'))
				{
					lstrcpy(info->fileName, &vecFullPath[i][j + 1]);
					break;
				}
			}
		}
		strcpy_s(info->pathName, strlen(ROOT_PATH) + 1, ROOT_PATH);
		TCHAR	_fileExt[_MAX_EXT] = {};
		_wsplitpath_s(vecFullPath[i], nullptr, 0, nullptr, 0, nullptr, 0, _fileExt, _MAX_EXT);
		char	fileExt[_MAX_EXT] = {};
#ifdef UNICODE
		int	convertLength = WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, _fileExt, -1, fileExt, convertLength, nullptr, nullptr);
#else
		strcpy_s(fileExt, _fileExt);
#endif // UNICODE
		_strupr_s(fileExt);
		DirectX::ScratchImage* image = new DirectX::ScratchImage;
		if (strcmp(fileExt, ".DDS") == 0)
		{
			if (FAILED(DirectX::LoadFromDDSFile(vecFullPath[i], DirectX::DDS_FLAGS_NONE, nullptr, *image)))
			{
				SAFE_DELETE(info);
				SAFE_DELETE(image);
				return false;
			}
		}
		else if (strcmp(fileExt, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(vecFullPath[i], nullptr, *image)))
			{
				SAFE_DELETE(info);
				SAFE_DELETE(image);
				return false;
			}
		}
		else
		{
			if (FAILED(DirectX::LoadFromWICFile(vecFullPath[i], DirectX::WIC_FLAGS_NONE, nullptr, *image)))
			{
				SAFE_DELETE(info);
				SAFE_DELETE(image);
				return false;
			}
		}
		info->image = image;
		m_vecTextureInfo.push_back(info);
	}
	if (!CreateResourceArray())	// Shader Resource View Array 생성
	{
		return false;
	}
	return true;
}

void CTexture::Render()
{
}

void CTexture::SetShader(int reg, int shaderBufferType, int index)
{
	if (m_ImageType != EImageType::Array)
	{
		if (shaderBufferType & (int)EShaderBufferType::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &m_vecTextureInfo[index]->SRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &m_vecTextureInfo[index]->SRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &m_vecTextureInfo[index]->SRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Domain)
		{
			CDevice::GetInst()->GetContext()->DSSetShaderResources(reg, 1, &m_vecTextureInfo[index]->SRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &m_vecTextureInfo[index]->SRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &m_vecTextureInfo[index]->SRV);
		}
	}
	else //array texture
	{
		if (shaderBufferType & (int)EShaderBufferType::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &m_ArraySRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &m_ArraySRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &m_ArraySRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Domain)
		{
			CDevice::GetInst()->GetContext()->DSSetShaderResources(reg, 1, &m_ArraySRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &m_ArraySRV);
		}
		if (shaderBufferType & (int)EShaderBufferType::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &m_ArraySRV);
		}
	}
}

void CTexture::ResetShader(int reg, int shaderBufferType)
{
	ID3D11ShaderResourceView* SRV = nullptr;
	if (shaderBufferType & (int)EShaderBufferType::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &SRV);
	}
	if (shaderBufferType & (int)EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &SRV);
	}
	if (shaderBufferType & (int)EShaderBufferType::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &SRV);
	}
	if (shaderBufferType & (int)EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetShaderResources(reg, 1, &SRV);
	}
	if (shaderBufferType & (int)EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &SRV);
	}
	if (shaderBufferType & (int)EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &SRV);
	}
}

void CTexture::Save(FILE* file)
{
	fwrite(&m_ImageType, sizeof(EImageType), 1, file);
	int	count = (int)m_vecTextureInfo.size();
	fwrite(&count, sizeof(int), 1, file);
	for (int i = 0; i < count; ++i)
	{
		fwrite(m_vecTextureInfo[i]->fileName, sizeof(TCHAR), MAX_PATH, file);
		fwrite(m_vecTextureInfo[i]->pathName, sizeof(char), MAX_PATH, file);
	}
}

void CTexture::Load(FILE* file)
{
}
