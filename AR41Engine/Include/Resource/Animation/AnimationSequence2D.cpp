#include "AnimationSequence2D.h"
#include "../ResourceManager.h"
#include "../../PathManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CAnimationSequence2D::CAnimationSequence2D()
	: m_Scene(nullptr)
	, m_Anim2DType(EAnimation2DType::Atlas)
{
	SetTypeID<CAnimationSequence2D>();
}

CAnimationSequence2D::CAnimationSequence2D(const CAnimationSequence2D& anim)
	: CRef(anim)
{
	m_Texture = anim.m_Texture;
	m_vecFrameData = anim.m_vecFrameData;
	m_Anim2DType = anim.m_Anim2DType;
}

CAnimationSequence2D::~CAnimationSequence2D()
{
}

bool CAnimationSequence2D::Init(CTexture* texture)
{
	m_Texture = texture;
	if (m_Texture)
	{
		m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();
	}
	return true;
}

bool CAnimationSequence2D::Init(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			return false;
		}
		m_Texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			return false;
		}
		m_Texture = CResourceManager::GetInst()->FindTexture(name);
	}
	if (m_Texture)
	{
		m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();
	}
	return true;
}

bool CAnimationSequence2D::InitFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, fullPath))
		{
			return false;
		}
		m_Texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
		{
			return false;
		}
		m_Texture = CResourceManager::GetInst()->FindTexture(name);
	}
	if (m_Texture)
	{
		m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();
	}
	return true;
}

bool CAnimationSequence2D::Init(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, vecFileName, pathName))
		{
			return false;
		}
		m_Texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
		{
			return false;
		}
		m_Texture = CResourceManager::GetInst()->FindTexture(name);
	}
	if (m_Texture)
	{
		m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();
	}
	return true;
}

bool CAnimationSequence2D::InitFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, vecFullPath))
		{
			return false;
		}
		m_Texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
		{
			return false;
		}
		m_Texture = CResourceManager::GetInst()->FindTexture(name);
	}
	if (m_Texture)
	{
		m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();
	}
	return true;
}

void CAnimationSequence2D::AddFrame(const Vector2& start, const Vector2& end)
{
	Animation2DFrameData	data;
	data.start = start;
	data.end = end;
	m_vecFrameData.push_back(data);
}

void CAnimationSequence2D::AddFrame(float startX, float startY, float endX, float endY)
{
	Animation2DFrameData	data;
	data.start = Vector2(startX, startY);
	data.end = Vector2(endX, endY);
	m_vecFrameData.push_back(data);
}

void CAnimationSequence2D::AddFrameAll(int count, const Vector2& start, const Vector2& end)
{
	for (int i = 0; i < count; ++i)
	{
		Animation2DFrameData	data;
		data.start = start;
		data.end = end;
		m_vecFrameData.push_back(data);
	}
}

void CAnimationSequence2D::AddFrameAll(int count, float startX, float startY, float endX, float endY)
{
	for (int i = 0; i < count; ++i)
	{
		Animation2DFrameData	data;
		data.start = Vector2(startX, startY);
		data.end = Vector2(endX, endY);
		m_vecFrameData.push_back(data);
	}
}

void CAnimationSequence2D::DeleteFrame(int index)
{
	auto	iter = m_vecFrameData.begin() + index;
	m_vecFrameData.erase(iter);
}

void CAnimationSequence2D::ClearFrame()
{
	m_vecFrameData.clear();
}

CAnimationSequence2D* CAnimationSequence2D::Clone()
{
	return new CAnimationSequence2D(*this);
}

bool CAnimationSequence2D::Save(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return false;
	}
	int	length = (int)m_Name.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_Name.c_str(), 1, length, file);
	fwrite(&m_Anim2DType, sizeof(EAnimation2DType), 1, file);
	bool	texEnable = false;
	if (m_Texture)
	{
		texEnable = true;
	}
	fwrite(&texEnable, 1, 1, file);
	if (m_Texture)
	{
		std::string texName = m_Texture->GetName();
		length = (int)texName.length();
		fwrite(&length, 4, 1, file);
		fwrite(texName.c_str(), 1, length, file);
		m_Texture->Save(file);
	}
	int	frameCount = (int)m_vecFrameData.size();
	fwrite(&frameCount, 4, 1, file);
	fwrite(&m_vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
	fclose(file);
	return true;
}

bool CAnimationSequence2D::Load(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	int	length = 0;
	char	name[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(name, 1, length, file);
	m_Name = name;
	fread(&m_Anim2DType, sizeof(EAnimation2DType), 1, file);
	bool	texEnable = false;
	fread(&texEnable, 1, 1, file);
	if (texEnable)
	{
		char texName[256] = {};
		length = 0;
		fread(&length, 4, 1, file);
		assert(length >= 0);
		fread(texName, 1, length, file);
		EImageType	imageType;
		fread(&imageType, sizeof(EImageType), 1, file);
		int	textureCount = 0;
		fread(&textureCount, sizeof(int), 1, file);
		TCHAR	folderName[8] = {};
		lstrcpy(folderName, TEXT("Texture"));
		if (textureCount == 1)
		{
			TCHAR	fileName[MAX_PATH] = {};
			char	pathName[MAX_PATH] = {};
			fread(fileName, sizeof(TCHAR), MAX_PATH, file);
			fread(pathName, sizeof(char), MAX_PATH, file);
			if (m_Scene)
			{
				m_Scene->GetResource()->LoadTexture(texName, fileName, pathName);
				m_Texture = m_Scene->GetResource()->FindTexture(texName);
			}
			else
			{
				CResourceManager::GetInst()->LoadTexture(texName, fileName, pathName);
				m_Texture = CResourceManager::GetInst()->FindTexture(texName);
			}
		}
		else
		{
			if (imageType == EImageType::Frame)
			{
				std::vector<const TCHAR*>	vecFileName;
				std::string	resultPathName;
				for (int i = 0; i < textureCount; ++i)
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
					m_Texture = m_Scene->GetResource()->FindTexture(texName);
				}
				else
				{
					CResourceManager::GetInst()->LoadTexture(texName, vecFileName, resultPathName);
					m_Texture = CResourceManager::GetInst()->FindTexture(texName);
				}
				for (int i = 0; i < textureCount; ++i)
				{
					SAFE_DELETE_ARRAY(vecFileName[i]);
				}
			}
			else
			{
				std::vector<const TCHAR*>	vecFileName;
				std::string	resultPathName;
				for (int i = 0; i <textureCount; ++i)
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
					m_Texture = m_Scene->GetResource()->FindTexture(texName);
				}
				else
				{
					CResourceManager::GetInst()->LoadTextureArray(texName, vecFileName, resultPathName);
					m_Texture = CResourceManager::GetInst()->FindTexture(texName);
				}
				for (int i = 0; i < textureCount; ++i)
				{
					SAFE_DELETE_ARRAY(vecFileName[i]);
				}
			}
		}
	}
	int	frameCount = 0;
	fread(&frameCount, 4, 1, file);
	m_vecFrameData.resize(frameCount);
	fread(&m_vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
	fclose(file);
	return true;
}

bool CAnimationSequence2D::Save(const char* fileName, const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return Save(fullPath);
}

bool CAnimationSequence2D::Load(const char* fileName, const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return Load(fullPath);
}
