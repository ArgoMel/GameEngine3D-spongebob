#include "EditorImage.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/ResourceManager.h"

CEditorImage::CEditorImage()
	: m_ImageStart{}
	, m_ImageEnd{}
	, m_BorderColor{}
	, m_ImageIndex(0)
	, m_Width(0)
	, m_Height(0)
	, m_IsFixed(false)
{
}

CEditorImage::~CEditorImage()
{
	if (m_Texture)
	{
		CResourceManager::GetInst()->ReleaseTexture(m_Texture->GetName());
	}
}

void CEditorImage::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CResourceManager::GetInst()->LoadTexture(name, fileName, pathName);
	m_Texture = CResourceManager::GetInst()->FindTexture(name);
	if (m_Texture)
	{
		SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
	}
}

void CEditorImage::SetTexture(const std::string& name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(name);
	if (m_Texture)
	{
		SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
	}
}

void CEditorImage::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath);
	m_Texture = CResourceManager::GetInst()->FindTexture(name);
	if (m_Texture)
	{
		SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
	}
}

void CEditorImage::SetTexture(CTexture* texture)
{
	m_Texture = texture;
	if (m_Texture)
	{
		SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
	}
}

bool CEditorImage::Init()
{
	SetTexture("DefaultUI", TEXT("DefaultUI.png"));
	return true;
}

void CEditorImage::Render()
{
	if (m_Texture)
	{
		if (!m_IsFixed)
		{
			m_Size = ImVec2((float)m_Width, (float)m_Height);
		}
		m_Width = m_Texture->GetWidth();
		m_Height = m_Texture->GetHeight();
		ImVec2 startUV = ImVec2(m_ImageStart.x / m_Width, m_ImageStart.y / m_Height);
		ImVec2 endUV = ImVec2(m_ImageEnd.x / m_Width, m_ImageEnd.y / m_Height);
		ImGui::Image(m_Texture->GetResource(m_ImageIndex), m_Size, startUV, endUV, m_Color, m_BorderColor);
	}
}