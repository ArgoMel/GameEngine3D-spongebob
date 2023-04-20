#include "UIImage.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CUIImage::CUIImage()
{
	m_WidgetTypeName = "UIImage";
}

CUIImage::CUIImage(const CUIImage& image)
	: CUIWidget(image)
{
	m_TextureInfo = image.m_TextureInfo;
}

CUIImage::~CUIImage()
{
}

void CUIImage::SetTexture(CTexture* texture)
{
	m_TextureInfo.texture = texture;
}

bool CUIImage::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			return false;
		}
		m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			return false;
		}
		m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

bool CUIImage::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, fullPath))
		{
			return false;
		}
		m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
		{
			return false;
		}
		m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

bool CUIImage::SetTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName
	, const std::string& pathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, vecFileName, pathName))
		{
			return false;
		}
		m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
		{
			return false;
		}
		m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

bool CUIImage::SetTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, vecFullPath))
		{
			return false;
		}
		m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
		{
			return false;
		}
		m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

void CUIImage::SetImageTint(const Vector4& tint)
{
	m_TextureInfo.tint = tint;
}

void CUIImage::SetImageTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_TextureInfo.tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CUIImage::AddFrameData(const Vector2& start, const Vector2& end)
{
	Animation2DFrameData    frame;
	frame.start = start;
	frame.end = end;
	m_TextureInfo.vecFrameData.push_back(frame);
	m_TextureInfo.frameTime = m_TextureInfo.vecFrameData.size() / m_TextureInfo.playTime;
}

void CUIImage::DeleteFrame(int index)
{
	auto	iter = m_TextureInfo.vecFrameData.begin() + index;
	m_TextureInfo.vecFrameData.erase(iter);
}

void CUIImage::ClearFrame()
{
	m_TextureInfo.vecFrameData.clear();
}

void CUIImage::SetPlayTime(float playTime)
{
	m_TextureInfo.playTime = playTime;
	m_TextureInfo.frameTime = m_TextureInfo.vecFrameData.size() / m_TextureInfo.playTime;
}

void CUIImage::SetPlayScale(float playScale)
{
	m_TextureInfo.playScale = playScale;
}

void CUIImage::Start()
{
	CUIWidget::Start();
}

bool CUIImage::Init()
{
	if (!CUIWidget::Init())
	{
		return false;
	}
	return true;
}

void CUIImage::Update(float deltaTime)
{
	CUIWidget::Update(deltaTime);
	if (!m_TextureInfo.vecFrameData.empty())
	{
		m_TextureInfo.time += deltaTime;
		if (m_TextureInfo.time >= m_TextureInfo.frameTime)
		{
			m_TextureInfo.time -= m_TextureInfo.frameTime;
			++m_TextureInfo.frame;
			if (m_TextureInfo.frame == m_TextureInfo.vecFrameData.size())
			{
				m_TextureInfo.frame = 0;
			}
		}
	}
}

void CUIImage::PostUpdate(float deltaTime)
{
	CUIWidget::PostUpdate(deltaTime);
}

void CUIImage::Render()
{
	bool textureEnable = m_TextureInfo.texture ? true : false;
	m_CBuffer->SetTextureEnable(textureEnable);
	if (textureEnable)
	{
		if (!m_TextureInfo.vecFrameData.empty())
		{
			int textureFrame = 0;
			if (m_TextureInfo.texture->GetImageType() == EImageType::Frame)
			{
				textureFrame = m_TextureInfo.frame;
			}
			m_TextureInfo.texture->SetShader(0, (int)EShaderBufferType::Pixel, textureFrame);
			m_AnimCBuffer->SetAnim2DEnable(true);
			m_AnimCBuffer->SetFrame(m_TextureInfo.frame);
			m_AnimCBuffer->SetImageFrame(m_TextureInfo.vecFrameData[m_TextureInfo.frame].start,
				m_TextureInfo.vecFrameData[m_TextureInfo.frame].end);
			m_AnimCBuffer->SetImageSize((float)m_TextureInfo.texture->GetWidth(),
				(float)m_TextureInfo.texture->GetHeight());
			m_AnimCBuffer->SetImageType((EAnimation2DType)m_TextureInfo.texture->GetImageType());
		}
		else
		{
			m_TextureInfo.texture->SetShader(0, (int)EShaderBufferType::Pixel, 0);
			m_AnimCBuffer->SetAnim2DEnable(false);
		}
	}
	m_Tint = m_TextureInfo.tint;
	CUIWidget::Render();
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}

void CUIImage::Save(FILE* file)
{
	CUIWidget::Save(file);
	fwrite(&m_TextureInfo.tint, sizeof(Vector4), 1, file);
	fwrite(&m_TextureInfo.frameTime, sizeof(float), 1, file);
	fwrite(&m_TextureInfo.playTime, sizeof(float), 1, file);
	fwrite(&m_TextureInfo.playScale, sizeof(float), 1, file);
	int frameCount = (int)m_TextureInfo.vecFrameData.size();
	fwrite(&frameCount, sizeof(int), 1, file);
	if (frameCount > 0)
	{
		fwrite(&m_TextureInfo.vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
	}
	if(m_TextureInfo.texture)
	{
		std::string texName = m_TextureInfo.texture->GetName();
		int length = (int)texName.length();
		fwrite(&length, sizeof(int), 1, file);
		fwrite(texName.c_str(), 1, length, file);
		m_TextureInfo.texture->Save(file);
	}
	else
	{
		int length = 0;
		fwrite(&length, sizeof(int), 1, file);
	}
}

void CUIImage::Load(FILE* file)
{
	CUIWidget::Load(file);
	fread(&m_TextureInfo.tint, sizeof(Vector4), 1, file);
	fread(&m_TextureInfo.frameTime, sizeof(float), 1, file);
	fread(&m_TextureInfo.playTime, sizeof(float), 1, file);
	fread(&m_TextureInfo.playScale, sizeof(float), 1, file);
	int frameCount = 0;
	fread(&frameCount, sizeof(int), 1, file);
	m_TextureInfo.vecFrameData.resize((size_t)frameCount);
	if (frameCount > 0)
	{
		fread(&m_TextureInfo.vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
	}
	char    texName[256] = {};
	int length = 0;
	fread(&length, sizeof(int), 1, file);
	if (length ==0)
	{
		return;
	}
	fread(texName, 1, length, file);
	EImageType  imageType;
	fread(&imageType, sizeof(EImageType), 1, file);
	int	textureSRVCount = 0;
	fread(&textureSRVCount, sizeof(int), 1, file);
	if (textureSRVCount == 1)
	{
		TCHAR	fileName[MAX_PATH] = {};
		char	pathName[MAX_PATH] = {};
		fread(fileName, sizeof(TCHAR), MAX_PATH, file);
		fread(pathName, sizeof(char), MAX_PATH, file);
		if (m_Scene)
		{
			m_Scene->GetResource()->LoadTexture(texName, fileName, pathName);
			m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(texName);
		}
		else
		{
			CResourceManager::GetInst()->LoadTexture(texName, fileName, pathName);
			m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(texName);
		}
	}
	else
	{
		if (imageType == EImageType::Frame)
		{
			std::vector<const TCHAR*>	vecFileName;
			std::string	resultPathName;
			for (int i = 0; i < textureSRVCount; ++i)
			{
				TCHAR* fileName = new TCHAR[MAX_PATH];
				char	pathName[MAX_PATH] = {};
				fread(fileName, sizeof(TCHAR), MAX_PATH, file);
				fread(pathName, sizeof(char), MAX_PATH, file);
				resultPathName = pathName;
				vecFileName.push_back(fileName);
			}
			if (m_Scene)
			{
				m_Scene->GetResource()->LoadTexture(texName, vecFileName, resultPathName);
				m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(texName);
			}
			else
			{
				CResourceManager::GetInst()->LoadTexture(texName, vecFileName, resultPathName);
				m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(texName);
			}
			for (int i = 0; i < textureSRVCount; ++i)
			{
				SAFE_DELETE_ARRAY(vecFileName[i]);
			}
		}
		else
		{
			std::vector<const TCHAR*>	vecFileName;
			std::string	resultPathName;
			for (int i = 0; i < textureSRVCount; ++i)
			{
				TCHAR* fileName = new TCHAR[MAX_PATH];
				char	pathName[MAX_PATH] = {};
				fread(fileName, sizeof(TCHAR), MAX_PATH, file);
				fread(pathName, sizeof(char), MAX_PATH, file);
				resultPathName = pathName;
				vecFileName.push_back(fileName);
			}
			if (m_Scene)
			{
				m_Scene->GetResource()->LoadTextureArray(texName, vecFileName, resultPathName);
				m_TextureInfo.texture = m_Scene->GetResource()->FindTexture(texName);
			}
			else
			{
				CResourceManager::GetInst()->LoadTextureArray(texName, vecFileName, resultPathName);
				m_TextureInfo.texture = CResourceManager::GetInst()->FindTexture(texName);
			}
			for (int i = 0; i < textureSRVCount; ++i)
			{
				SAFE_DELETE_ARRAY(vecFileName[i]);
			}
		}
	}
}
