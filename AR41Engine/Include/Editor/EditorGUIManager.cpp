#include "EditorGUIManager.h"
#include "../Device.h"
#include "../PathManager.h"
DEFINITION_SINGLE(CEditorGUIManager)

CEditorGUIManager::CEditorGUIManager()
	: m_Context(nullptr)
	, m_Font(nullptr)
{
}

CEditorGUIManager::~CEditorGUIManager()
{
	auto	iter = m_mapWindow.begin();
	auto	iterEnd = m_mapWindow.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool CEditorGUIManager::Init(HWND hWnd)
{
	m_Context = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
//스타일 설정
	ImGui::StyleColorsDark();
// 모서리 설정(안건드리는걸 추천)
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());
	SetFont("DefaultFont", "NotoSansKR-Regular.otf", 20.f);
	return true;
}

void CEditorGUIManager::Update(float deltaTime)
{
	if (m_mapWindow.empty())
	{
		return;
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	auto	iter = m_mapWindow.begin();
	auto	iterEnd = m_mapWindow.end();
	for (; iter != iterEnd; ++iter)
	{
		iter->second->Update(deltaTime);
	}
	ImGui::Render();
}

void CEditorGUIManager::Render()
{
	if (m_mapWindow.empty())
	{
		return;
	}
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void CEditorGUIManager::SetFont(const std::string& name, const char* fileName, float size, int overH, int overV, 
	float spacing, const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig	config;
	config.OversampleH = overH;
	config.OversampleV = overV;
	config.GlyphExtraSpacing.x = spacing;
	config.PixelSnapH = 1;
	m_Font = io.Fonts->AddFontFromFileTTF(fullPath, size, &config, io.Fonts->GetGlyphRangesKorean());
}

void CEditorGUIManager::EnableFont()
{
	if (m_Font)
	{
		ImGui::PushFont(m_Font);
	}
}

void CEditorGUIManager::DisableFont()
{
	if (m_Font)
	{
		ImGui::PopFont();
	}
}

void CEditorGUIManager::OnOffEditorWindow(bool b)
{
	auto	iter = m_mapWindow.begin();
	auto	iterEnd = m_mapWindow.end();
	for (; iter != iterEnd; ++iter)
	{
		if(b)
		{
			iter->second->Open();
		}
		else
		{
			iter->second->Close();
		}
	}
}
