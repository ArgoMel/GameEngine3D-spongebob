#include "UIProgressBar.h"
#include "../Resource/Shader/UIProgressBarShader.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CUIProgressBarConstantBuffer* CUIProgressBar::m_ProgressBarCBuffer = nullptr;

CUIProgressBar::CUIProgressBar()
	: m_Min(0.f)
	, m_Max(1.f)
	, m_Value(1.f)
	, m_BarDir(EProgressBarDir::RightToLeft)
{
	m_WidgetTypeName = "UIProgressBar";
}

CUIProgressBar::CUIProgressBar(const CUIProgressBar& progressBar)
	: CUIWidget(progressBar)
{
	for (int i = 0; i < (int)EProgressBarTextureType::Max; ++i)
	{
		m_TextureInfo[i] = progressBar.m_TextureInfo[i];
	}
	m_Min = progressBar.m_Min;
	m_Max = progressBar.m_Max;
	m_Value = progressBar.m_Value;
	m_BarDir = progressBar.m_BarDir;
}

CUIProgressBar::~CUIProgressBar()
{
}

void CUIProgressBar::SetTexture(EProgressBarTextureType type, CTexture* texture)
{
	m_TextureInfo[(int)type].texture = texture;
}

bool CUIProgressBar::SetTexture(EProgressBarTextureType type, const std::string& name, 
	const TCHAR* fileName, const std::string& pathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			return false;
		}
		m_TextureInfo[(int)type].texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			return false;
		}
		m_TextureInfo[(int)type].texture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

bool CUIProgressBar::SetTextureFullPath(EProgressBarTextureType type, const std::string& name, 
	const TCHAR* fullPath)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, fullPath))
		{
			return false;
		}
		m_TextureInfo[(int)type].texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
		{
			return false;
		}
		m_TextureInfo[(int)type].texture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

bool CUIProgressBar::SetTexture(EProgressBarTextureType type, const std::string& name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, vecFileName, pathName))
		{
			return false;
		}
		m_TextureInfo[(int)type].texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
		{
			return false;
		}
		m_TextureInfo[(int)type].texture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

bool CUIProgressBar::SetTextureFullPath(EProgressBarTextureType type, const std::string& name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, vecFullPath))
		{
			return false;
		}
		m_TextureInfo[(int)type].texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
		{
			return false;
		}
		m_TextureInfo[(int)type].texture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

void CUIProgressBar::SetImageTint(EProgressBarTextureType type, const Vector4& tint)
{
	m_TextureInfo[(int)type].tint = tint;
}

void CUIProgressBar::SetImageTint(EProgressBarTextureType type, 
	unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_TextureInfo[(int)type].tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CUIProgressBar::AddFrameData(EProgressBarTextureType type, const Vector2& start, const Vector2& end)
{
	Animation2DFrameData    frame;
	frame.start = start;
	frame.end = end;
	m_TextureInfo[(int)type].vecFrameData.push_back(frame);
	m_TextureInfo[(int)type].frameTime = m_TextureInfo[(int)type].vecFrameData.size() / m_TextureInfo[(int)type].playTime;
}

void CUIProgressBar::SetPlayTime(EProgressBarTextureType type, float playTime)
{
	m_TextureInfo[(int)type].playTime = playTime;
	m_TextureInfo[(int)type].frameTime = m_TextureInfo[(int)type].vecFrameData.size() / m_TextureInfo[(int)type].playTime;
}

void CUIProgressBar::SetPlayScale(EProgressBarTextureType type, float playScale)
{
	m_TextureInfo[(int)type].playScale = playScale;
}

void CUIProgressBar::SetProgressBarMin(float min)
{
	m_Min = min;
}

void CUIProgressBar::SetProgressBarMax(float max)
{
	m_Max = max;
}

void CUIProgressBar::SetValue(float value)
{
	m_Value = value;
	if (m_Value < m_Min)
	{
		m_Value = m_Min;
	}
	else if (m_Value > m_Max)
	{
		m_Value = m_Max;
	}
}

void CUIProgressBar::AddValue(float value)
{
	m_Value += value;
	if (m_Value < m_Min)
	{
		m_Value = m_Min;
	}
	else if (m_Value > m_Max)
	{
		m_Value = m_Max;
	}
}

void CUIProgressBar::Start()
{
	CUIWidget::Start();
	if (m_Scene)
	{
		m_BarShader = m_Scene->GetResource()->FindShader("UIProgressBarShader");
	}
	else
	{
		m_BarShader = CResourceManager::GetInst()->FindShader("UIProgressBarShader");
	}
}

bool CUIProgressBar::Init()
{
	if (!CUIWidget::Init())
	{
		return false;
	}
	if(!m_BarShader)
	{
		if (m_Scene)
		{
			m_BarShader = m_Scene->GetResource()->FindShader("UIProgressBarShader");
		}
		else
		{
			m_BarShader = CResourceManager::GetInst()->FindShader("UIProgressBarShader");
		}
	}
	return true;
}

void CUIProgressBar::Update(float deltaTime)
{
	CUIWidget::Update(deltaTime);
	for (int i = 0; i < (int)EProgressBarTextureType::Max; ++i)
	{
		if (!m_TextureInfo[i].vecFrameData.empty())
		{
			m_TextureInfo[i].time += deltaTime;
			if (m_TextureInfo[i].time >= m_TextureInfo[i].frameTime)
			{
				m_TextureInfo[i].time -= m_TextureInfo[i].frameTime;
				++m_TextureInfo[i].frame;
				if (m_TextureInfo[i].frame == m_TextureInfo[i].vecFrameData.size())
				{
					m_TextureInfo[i].frame = 0;
				}
			}
		}
	}
}

void CUIProgressBar::PostUpdate(float deltaTime)
{
	CUIWidget::PostUpdate(deltaTime);
}

void CUIProgressBar::Render()
{
	bool textureEnable = m_TextureInfo[(int)EProgressBarTextureType::Back].texture ? true : false;
	m_CBuffer->SetTextureEnable(textureEnable);
	if (textureEnable)
	{
		if (!m_TextureInfo[(int)EProgressBarTextureType::Back].vecFrameData.empty())
		{
			int textureFrame = 0;
			if (m_TextureInfo[(int)EProgressBarTextureType::Back].texture->GetImageType() == EImageType::Frame)
			{
				textureFrame = m_TextureInfo[(int)EProgressBarTextureType::Back].frame;
			}
			m_TextureInfo[(int)EProgressBarTextureType::Back].texture->SetShader(0, 
				(int)EShaderBufferType::Pixel, textureFrame);
			m_AnimCBuffer->SetAnim2DEnable(true);
			m_AnimCBuffer->SetFrame(m_TextureInfo[(int)EProgressBarTextureType::Back].frame);
			m_AnimCBuffer->SetImageFrame(m_TextureInfo[(int)EProgressBarTextureType::Back].
				vecFrameData[m_TextureInfo[(int)EProgressBarTextureType::Back].frame].start,
				m_TextureInfo[(int)EProgressBarTextureType::Back].
				vecFrameData[m_TextureInfo[(int)EProgressBarTextureType::Back].frame].end);
			m_AnimCBuffer->SetImageSize((float)m_TextureInfo[(int)EProgressBarTextureType::Back].texture->GetWidth(),
				(float)m_TextureInfo[(int)EProgressBarTextureType::Back].texture->GetHeight());
			m_AnimCBuffer->SetImageType(
				(EAnimation2DType)m_TextureInfo[(int)EProgressBarTextureType::Back].texture->GetImageType());
		}
		else
		{
			m_TextureInfo[(int)EProgressBarTextureType::Back].texture->SetShader(0, (int)EShaderBufferType::Pixel, 0);
			m_AnimCBuffer->SetAnim2DEnable(false);
		}
	}
	m_Tint = m_TextureInfo[(int)EProgressBarTextureType::Back].tint;
	CUIWidget::Render();
	textureEnable = m_TextureInfo[(int)EProgressBarTextureType::Bar].texture ? true : false;
	m_CBuffer->SetTextureEnable(textureEnable);
	if (textureEnable)
	{
		if (!m_TextureInfo[(int)EProgressBarTextureType::Bar].vecFrameData.empty())
		{
			int textureFrame = 0;
			if (m_TextureInfo[(int)EProgressBarTextureType::Bar].texture->GetImageType() == EImageType::Frame)
			{
				textureFrame = m_TextureInfo[(int)EProgressBarTextureType::Bar].frame;
			}
			m_TextureInfo[(int)EProgressBarTextureType::Bar].texture->SetShader(0,
				(int)EShaderBufferType::Pixel, textureFrame);
			m_AnimCBuffer->SetAnim2DEnable(true);
			m_AnimCBuffer->SetFrame(m_TextureInfo[(int)EProgressBarTextureType::Bar].frame);
			m_AnimCBuffer->SetImageFrame(m_TextureInfo[(int)EProgressBarTextureType::Bar].
				vecFrameData[m_TextureInfo[(int)EProgressBarTextureType::Bar].frame].start,
				m_TextureInfo[(int)EProgressBarTextureType::Bar].
				vecFrameData[m_TextureInfo[(int)EProgressBarTextureType::Bar].frame].end);
			m_AnimCBuffer->SetImageSize((float)m_TextureInfo[(int)EProgressBarTextureType::Bar].texture->GetWidth(),
				(float)m_TextureInfo[(int)EProgressBarTextureType::Bar].texture->GetHeight());
			m_AnimCBuffer->SetImageType(
				(EAnimation2DType)m_TextureInfo[(int)EProgressBarTextureType::Bar].texture->GetImageType());
		}
		else
		{
			m_TextureInfo[(int)EProgressBarTextureType::Bar].texture->SetShader(0, (int)EShaderBufferType::Pixel, 0);
			m_AnimCBuffer->SetAnim2DEnable(false);
		}
	}
	m_Tint = m_TextureInfo[(int)EProgressBarTextureType::Bar].tint;
	m_CBuffer->SetTint(m_Tint);
	m_ProgressBarCBuffer->SetBarDir(m_BarDir);
	m_ProgressBarCBuffer->SetPercent((m_Value - m_Min) / (m_Max - m_Min));
	m_ProgressBarCBuffer->UpdateBuffer();
	m_CBuffer->UpdateBuffer();
	m_AnimCBuffer->UpdateBuffer();
	m_BarShader->SetShader();
	m_Mesh->Render();
}

CUIProgressBar* CUIProgressBar::Clone()
{
	return new CUIProgressBar(*this);
}

void CUIProgressBar::Save(FILE* file)
{
	CUIWidget::Save(file);
	for (int i = 0; i < (int)EProgressBarTextureType::Max; ++i)
	{
		fwrite(&m_TextureInfo[i].tint, sizeof(Vector4), 1, file);
		fwrite(&m_TextureInfo[i].frameTime, sizeof(float), 1, file);
		fwrite(&m_TextureInfo[i].playTime, sizeof(float), 1, file);
		fwrite(&m_TextureInfo[i].playScale, sizeof(float), 1, file);
		int frameCount = (int)m_TextureInfo[i].vecFrameData.size();
		fwrite(&frameCount, sizeof(int), 1, file);
		if (frameCount > 0)
		{
			fwrite(&m_TextureInfo[i].vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
		}
		bool    texEnable = false;
		if (m_TextureInfo[i].texture)
		{
			texEnable = true;
		}
		fwrite(&texEnable, sizeof(bool), 1, file);
		if (texEnable)
		{
			std::string texName = m_TextureInfo[i].texture->GetName();
			int length = (int)texName.length();
			fwrite(&length, sizeof(int), 1, file);
			fwrite(texName.c_str(), 1, length, file);
			m_TextureInfo[i].texture->Save(file);
		}
	}
	fwrite(&m_BarDir, sizeof(EProgressBarDir), 1, file);
	fwrite(&m_Min, sizeof(float), 1, file);
	fwrite(&m_Max, sizeof(float), 1, file);
	fwrite(&m_Value, sizeof(float), 1, file);
}

void CUIProgressBar::Load(FILE* file)
{
	CUIWidget::Load(file);
	for (int i = 0; i < (int)EProgressBarTextureType::Max; ++i)
	{
		fread(&m_TextureInfo[i].tint, sizeof(Vector4), 1, file);
		fread(&m_TextureInfo[i].frameTime, sizeof(float), 1, file);
		fread(&m_TextureInfo[i].playTime, sizeof(float), 1, file);
		fread(&m_TextureInfo[i].playScale, sizeof(float), 1, file);
		int frameCount = 0;
		fread(&frameCount, sizeof(int), 1, file);
		m_TextureInfo[i].vecFrameData.resize((size_t)frameCount);
		if (frameCount > 0)
		{
			fread(&m_TextureInfo[i].vecFrameData[0], sizeof(Animation2DFrameData), frameCount, file);
		}
		bool    texEnable = false;
		fread(&texEnable, sizeof(bool), 1, file);
		if (texEnable)
		{
			char    texName[256] = {};
			int length = 0;
			fread(&length, sizeof(int), 1, file);
			assert(length >= 0);
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
					m_TextureInfo[i].texture = m_Scene->GetResource()->FindTexture(texName);
				}
				else
				{
					CResourceManager::GetInst()->LoadTexture(texName, fileName, pathName);
					m_TextureInfo[i].texture = CResourceManager::GetInst()->FindTexture(texName);
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
						m_TextureInfo[i].texture = m_Scene->GetResource()->FindTexture(texName);
					}
					else
					{
						CResourceManager::GetInst()->LoadTexture(texName, vecFileName, resultPathName);
						m_TextureInfo[i].texture = CResourceManager::GetInst()->FindTexture(texName);
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
						m_TextureInfo[i].texture = m_Scene->GetResource()->FindTexture(texName);
					}
					else
					{
						CResourceManager::GetInst()->LoadTextureArray(texName, vecFileName, resultPathName);
						m_TextureInfo[i].texture = CResourceManager::GetInst()->FindTexture(texName);
					}
					for (int i = 0; i < textureSRVCount; ++i)
					{
						SAFE_DELETE_ARRAY(vecFileName[i]);
					}
				}
			}
		}
	}
	fread(&m_BarDir, sizeof(EProgressBarDir), 1, file);
	fread(&m_Min, sizeof(float), 1, file);
	fread(&m_Max, sizeof(float), 1, file);
	fread(&m_Value, sizeof(float), 1, file);
}
