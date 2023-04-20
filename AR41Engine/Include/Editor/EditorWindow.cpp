#include "EditorWindow.h"
#include "EditorWidget.h"
#include "EditorGUIManager.h"

CEditorWindow::CEditorWindow()
	: m_WindowFlag(ImGuiWindowFlags_NoCollapse)
	, m_Open(true)
{
}

CEditorWindow::~CEditorWindow()
{
	size_t	size = m_vecWidget.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecWidget[i]);
	}
}

void CEditorWindow::DeleteAllWidget()
{
	size_t	size = m_vecWidget.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecWidget[i]);
	}
	m_vecWidget.clear();
}

void CEditorWindow::ClearWidget()
{
	m_vecWidget.clear();
}

void CEditorWindow::Open()
{
	m_Open = true;
}

void CEditorWindow::Close()
{
	m_Open = false;
}

CEditorWidget* CEditorWindow::FindWidget(const std::string& name)
{
	size_t	size = m_vecWidget.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecWidget[i]->GetName() == name)
		{
			return m_vecWidget[i];
		}
	}
	return nullptr;
}

bool CEditorWindow::Init()
{
	return true;
}

void CEditorWindow::Update(float deltaTime)
{
	if (!m_Open)
	{
		return;
	}
	if (!ImGui::Begin(m_Name.c_str(), &m_Open, m_WindowFlag))
	{
		m_Open = false;
	}
	CEditorGUIManager::GetInst()->EnableFont();
	size_t	size = m_vecWidget.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecWidget[i]->Render();
	}
	CEditorGUIManager::GetInst()->DisableFont();
	ImGui::End();
}

//if (m_vecWidget[i]->IsActive())
//{
//	ImGui::Separator();	//¡Ÿ±ﬂ±‚ ±‚¥…
//}