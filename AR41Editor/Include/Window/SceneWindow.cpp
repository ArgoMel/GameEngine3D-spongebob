#include "SceneWindow.h"
#include "ObjectWindow.h"
#include "PathManager.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorGUIManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneInfo.h"
#include "../Scene/EditorDefaultScene.h"
#include "../Scene/LoadingSceneInfo.h"

CSceneWindow::CSceneWindow()
	: m_SceneList(nullptr)
	, m_SceneSelectName(nullptr)
{
}

CSceneWindow::~CSceneWindow()
{
}

bool CSceneWindow::Init()
{
	CEditorLabel* Label = CreateWidget<CEditorLabel>("Scene");
	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSizeY(30.f);

	CEditorSameLine* Line = CreateWidget<CEditorSameLine>("Line");

	m_SceneSelectName = CreateWidget<CEditorInputText>("SceneName");
	m_SceneSelectName->SetHideName("SceneName");
	m_SceneSelectName->SetSize(150.f, 30.f);

	m_SceneList = CreateWidget<CEditorListBox>("SceneListBox");
	m_SceneList->SetHideName("SceneListBox");
	m_SceneList->SetSize(150.f, 300.f);
	m_SceneList->SetPageItemCount(6);
	m_SceneList->SetSelectCallback<CSceneWindow>(this, &CSceneWindow::SceneClickCallback);

	Line = CreateWidget<CEditorSameLine>("Line");

	CEditorButton* Button = CreateWidget<CEditorButton>("Scene 변경", 90.f, 30.f);
	Button->SetColor(29, 47, 73, 255);
	Button->SetClickCallback<CSceneWindow>(this, &CSceneWindow::SceneChange);

	Line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("pos");
	pos->SetPos(-98.f, 40.f);

	Button = CreateWidget<CEditorButton>("Scene 저장", 90.f, 30.f);
	Button->SetClickCallback<CSceneWindow>(this, &CSceneWindow::SceneSave);

	LoadSceneDirectory();
	return true;
}

void CSceneWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CSceneWindow::SceneChange()
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	if (scene->GetName() == m_SelectSceneItem && !m_SelectSceneItem.empty())
	{
		return;
	}
	CSceneManager::GetInst()->CreateNextScene();
	CScene* nextScene = CSceneManager::GetInst()->GetNextScene();
	const PathInfo* info = CPathManager::GetInst()->FindPath(SCENE_PATH);
	char	fullPath[MAX_PATH] = {};
	if (info)
	{
		strcpy_s(fullPath, info->pathMultibyte);
	}
	strcat_s(fullPath, m_SelectSceneItem.c_str());
	strcat_s(fullPath, ".scn");
	nextScene->Load(fullPath);
	CObjectWindow* objectWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
	if(objectWindow)
	{
		objectWindow->AddInput(nextScene);
		objectWindow->AddItemList(nextScene);
	}
}

void CSceneWindow::SceneSave()
{
	std::string name = m_SceneSelectName->GetText();
	if (name.empty())
	{
		return;
	}
	const PathInfo* info = CPathManager::GetInst()->FindPath(SCENE_PATH);
	char	fullPath[MAX_PATH] = {};
	if (info)
	{
		strcpy_s(fullPath, info->pathMultibyte);
	}
	strcat_s(fullPath, name.c_str());
	strcat_s(fullPath, ".scn");
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->SetName(name);
	scene->Save(fullPath);
	if (!m_SceneList->CheckItem(name))
	{
		m_SceneList->AddItem(name);
	}
}

void CSceneWindow::SceneClickCallback(int index, const std::string& item)
{
	m_SelectSceneItem = item;
	m_SceneSelectName->SetText(item.c_str());
}

void CSceneWindow::LoadSceneDirectory()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	strcat_s(path, "Scene/");
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		m_SceneList->AddItem(name);
	}
}