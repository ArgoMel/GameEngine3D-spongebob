#include "Animation2DWindow.h"
#include "ResourceWindow.h"
#include "PathManager.h"
#include "Engine.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorGroup.h"
#include "Editor/EditorGUIManager.h"
#include "Resource/ResourceManager.h"
#include "Component/SpriteComponent.h"
#include "DetailWindow.h"
#include "DetailWindow/SpriteComponentWidgetList.h"

CAnimation2DWindow::CAnimation2DWindow()
	: m_Animation2DClassList(nullptr)
	, m_Animation2DSequenceList(nullptr)
	, m_SequenceFrameList(nullptr)
	, m_Sequence2DImage(nullptr)
	, m_StartX(nullptr)
	, m_StartY(nullptr)
	, m_EndX(nullptr)
	, m_EndY(nullptr)
	, m_Count(nullptr)
{
}

CAnimation2DWindow::~CAnimation2DWindow()
{
}

bool CAnimation2DWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("Animation2DClass");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(150.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* anim2DCreateButton = CreateWidget<CEditorButton>("Anim2DCreate");
	anim2DCreateButton->SetSize(140.f, 30.f);
	anim2DCreateButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::CreateAnimation2D);
	m_Animation2DClassList = CreateWidget<CEditorListBox>("Animation2DClassListBox");
	m_Animation2DClassList->SetHideName("Animation2DClassListBox");
	m_Animation2DClassList->SetSize(300.f, 300.f);
	m_Animation2DClassList->SetPageItemCount(6);
	m_Animation2DClassList->SetSelectCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::Animation2DSelectCallback);
	label = CreateWidget<CEditorLabel>("Sequence2D");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(150.f, 30.f);
	m_Animation2DSequenceList = CreateWidget<CEditorListBox>("Animation2DSequenceListBox");
	m_Animation2DSequenceList->SetHideName("Animation2DSequenceListBox");
	m_Animation2DSequenceList->SetSize(300.f, 300.f);
	m_Animation2DSequenceList->SetPageItemCount(6);
	m_Animation2DSequenceList->SetSelectCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::SequenceSelectCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorGroup* anim2DSequenceGroup = CreateWidget<CEditorGroup>("Anim2DGroup");
	label = anim2DSequenceGroup->CreateWidget<CEditorLabel>("Texture");
	label->SetColor(0, 0, 255, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(150.f, 30.f);
	m_Sequence2DImage = anim2DSequenceGroup->CreateWidget<CEditorImage>("Sequence2DTexture");
	m_Sequence2DImage->SetSize(100.f, 100.f);
	line = anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");
	CEditorButton* textureSelectButton = anim2DSequenceGroup->CreateWidget<CEditorButton>("TextureSelect");
	textureSelectButton->SetSize(150.f, 30.f);
	textureSelectButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::SelectTextureButton);
	label = anim2DSequenceGroup->CreateWidget<CEditorLabel>("Frame");
	label->SetColor(0, 0, 255, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(150.f, 30.f);
	line = anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");
	CEditorButton* createSquenceButton = anim2DSequenceGroup->CreateWidget<CEditorButton>("CreateSequence");
	createSquenceButton->SetSize(150.f, 30.f);
	createSquenceButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::CreateAnimationSequence2D);
	label = anim2DSequenceGroup->CreateWidget<CEditorLabel>("Start");
	label->SetColor(0, 0, 255, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(80.f, 30.f);
	line = anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");
	m_StartX = anim2DSequenceGroup->CreateWidget<CEditorInputText>("StartX");
	m_StartX->SetHideName("StartX");
	m_StartX->SetSize(100.f, 30.f);
	m_StartX->SetInputType(EImGuiInputType::Float);
	line = anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");
	m_StartY = anim2DSequenceGroup->CreateWidget<CEditorInputText>("StartY");
	m_StartY->SetHideName("StartY");
	m_StartY->SetSize(100.f, 30.f);
	m_StartY->SetInputType(EImGuiInputType::Float);
	label = anim2DSequenceGroup->CreateWidget<CEditorLabel>("End");
	label->SetColor(0, 0, 255, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(80.f, 30.f);
	line = anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");
	m_EndX = anim2DSequenceGroup->CreateWidget<CEditorInputText>("EndX");
	m_EndX->SetHideName("EndX");
	m_EndX->SetSize(100.f, 30.f);
	m_EndX->SetInputType(EImGuiInputType::Float);
	line = anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");
	m_EndY = anim2DSequenceGroup->CreateWidget<CEditorInputText>("EndY");
	m_EndY->SetHideName("EndY");
	m_EndY->SetSize(100.f, 30.f);
	m_EndY->SetInputType(EImGuiInputType::Float);
	label = anim2DSequenceGroup->CreateWidget<CEditorLabel>("Count");
	label->SetColor(0, 0, 255, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(80.f, 30.f);
	line = anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");
	m_Count = anim2DSequenceGroup->CreateWidget<CEditorInputText>("Count");
	m_Count->SetHideName("Count");
	m_Count->SetSize(150.f, 30.f);
	m_Count->SetInputType(EImGuiInputType::Int);
	label = anim2DSequenceGroup->CreateWidget<CEditorLabel>("FrameList");
	label->SetColor(0, 0, 255, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(80.f, 30.f);
	m_SequenceFrameList = anim2DSequenceGroup->CreateWidget<CEditorListBox>("SequenceFrameListBox");
	m_SequenceFrameList->SetHideName("SequenceFrameListBox");
	m_SequenceFrameList->SetSize(100.f, 200.f);
	m_SequenceFrameList->SetPageItemCount(6);
	m_SequenceFrameList->SetSelectCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::FrameSelectCallback);
	CEditorButton* addFrameButton = anim2DSequenceGroup->CreateWidget<CEditorButton>("AddFrame");
	addFrameButton->SetSize(150.f, 30.f);
	addFrameButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::AddFrameClick);
	line = anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");
	addFrameButton = anim2DSequenceGroup->CreateWidget<CEditorButton>("AddAllFrame");
	addFrameButton->SetSize(150.f, 30.f);
	addFrameButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::AddAllFrameClick);

	LoadAnimation2DClassList();
	LoadAnimation2DSequenceList();
	return true;
}

void CAnimation2DWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CAnimation2DWindow::LoadAnimation2DClassList()
{
	m_Animation2DClassList->AddItem("Animation2D");
}

void CAnimation2DWindow::LoadAnimation2DSequenceList()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ANIMATION_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
	{
		char	name[256] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		char	folderName[12] = {};
		int folderSize = (int)std::size(folderName);
		strcpy_s(fullPath, file.path().generic_string().c_str());
		int	length = (int)strlen(fullPath);
		if (length >= folderSize-1)
		{
			for (int i = length - folderSize; i >= 0; --i)
			{
				memcpy(folderName, &fullPath[i], folderSize-1);
				if (strcmp(folderName, "/Animation/") == 0)
				{
					strcpy_s(name, &fullPath[i + folderSize-1]);
					break;
				}
			}
		}
		m_Animation2DSequenceList->AddItem(name);
		CResourceManager::GetInst()->LoadSequence2D(name, fullPath);
	}
}

void CAnimation2DWindow::CreateAnimation2D()
{
}

void CAnimation2DWindow::SelectTextureButton()
{
	CResourceWindow* resourceWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CResourceWindow>("ResourceWindow");
	if (!resourceWindow)
	{
		return;
	}
	m_SelectTexture = resourceWindow->GetSelectTexture();
	m_Sequence2DImage->SetTexture(m_SelectTexture);
}

void CAnimation2DWindow::CreateAnimationSequence2D()
{
	if (!m_SelectTexture || m_vecFrame.empty())
	{
		return;
	}
	OPENFILENAME	ofn = {};
	TCHAR	fullPath[MAX_PATH] = {};
	TCHAR	filter[] = TEXT("모든 파일\0*.*\0SQC\0*.sqc");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fullPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->path;
	if (GetSaveFileName(&ofn) != 0)
	{
		char	multibytePath[MAX_PATH] = {};
		int	length = (int)WideCharToMultiByte(CP_ACP, 0, fullPath, -1,0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, fullPath, -1, multibytePath, length, 0, 0);
		char	fileName[_MAX_EXT] = {};
		_splitpath_s(multibytePath, nullptr, 0, nullptr, 0, fileName, _MAX_EXT, nullptr, 0);
		if (!CResourceManager::GetInst()->CreateAnimationSequence2D(fileName, m_SelectTexture))
		{
			return;
		}
		int	count = (int)m_vecFrame.size();
		for (int i = 0; i < count; ++i)
		{
			CResourceManager::GetInst()->AddAnimationSequence2DFrame(fileName, m_vecFrame[i].start, m_vecFrame[i].end);
		}
		CResourceManager::GetInst()->SaveSequence2D(fileName, multibytePath);
		length = (int)strlen(multibytePath);
		char	folderName[12] = {};		// /Animation/
		int folderSize = (int)std::size(folderName);
		if (length >= folderSize)
		{
			for (int i = length - folderSize+1; i >= 0; --i)
			{
				memcpy(folderName, &multibytePath[i], folderSize-1);
				if (strcmp(folderName, "/Animation2D/") == 0)
				{
					memset(fileName, 0, _MAX_EXT);
					strcpy_s(fileName, &multibytePath[i + folderSize-1]);
					break;
				}
			}
		}
		m_Animation2DSequenceList->AddItem(fileName);
		//CAnimationSequence2D* Sequence =
	}
}

void CAnimation2DWindow::AddFrameClick()
{
	Animation2DFrameData	data;
	data.start.x = m_StartX->GetFloat();
	data.start.y = m_StartY->GetFloat();
	data.end.x = m_EndX->GetFloat();
	data.end.y = m_EndY->GetFloat();
	int	number = (int)m_vecFrame.size();
	char	numberString[4] = {};
	_itoa_s(number, numberString, 10);
	m_SequenceFrameList->AddItem(numberString);
	m_vecFrame.push_back(data);
}

void CAnimation2DWindow::AddAllFrameClick()
{
	m_vecFrame.clear();
	m_SequenceFrameList->Clear();
	int	count = m_Count->GetInt();
	for (int i = 0; i < count; ++i)
	{
		Animation2DFrameData	data;
		data.start.x = m_StartX->GetFloat();
		data.start.y = m_StartY->GetFloat();
		data.end.x = m_EndX->GetFloat();
		data.end.y = m_EndY->GetFloat();
		char	numberString[4] = {};
		_itoa_s(i, numberString, 10);
		m_SequenceFrameList->AddItem(numberString);
		m_vecFrame.push_back(data);
	}
}

void CAnimation2DWindow::Animation2DSelectCallback(int index, const std::string& item)
{
	m_Animation2DSelectName = item;
}

void CAnimation2DWindow::SequenceSelectCallback(int index, const std::string& item)
{
	m_Animation2DSequenceSelectName = item;
	CDetailWindow* detailWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CDetailWindow>("DetailWindow");
	if(!detailWindow)
	{
		return;
	}
	if (detailWindow->GetSelectComponent()->GetComponentTypeName() != "SpriteComponent")
	{
		return;
	}
	CSpriteComponentWidgetList* spriteWidget = 
		(CSpriteComponentWidgetList*)detailWindow->GetComponentWidgetList("SpriteComponent");
	spriteWidget->SetSelectAnimationSequence2DName(item);
}

void CAnimation2DWindow::FrameSelectCallback(int index, const std::string& item)
{
	m_StartX->SetFloat(m_vecFrame[index].start.x);
	m_StartY->SetFloat(m_vecFrame[index].start.y);
	m_EndX->SetFloat(m_vecFrame[index].end.x);
	m_EndY->SetFloat(m_vecFrame[index].end.y);
}