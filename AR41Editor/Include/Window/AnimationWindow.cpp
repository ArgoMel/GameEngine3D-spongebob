#include "AnimationWindow.h"
#include "PathManager.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorGUIManager.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material/Material.h"
#include "Scene/SceneManager.h"
#include "GameObject/GameObject.h"

CAnimationWindow::CAnimationWindow()
	: m_IMGList(nullptr)
	, m_FrameList(nullptr)
	, m_IMGSelectName(nullptr)
	, m_AnimationName(nullptr)
	, m_AnimationInfo{}
	, m_Image(nullptr)
	, m_Slider(nullptr)
	, m_LoopCheck(nullptr)
	, m_ReverseCheck(nullptr)
	, m_Anim(nullptr)
	, m_ListIndex(0)
{
}

CAnimationWindow::~CAnimationWindow()
{
	
}

void CAnimationWindow::SetSelectComponent(CSpriteComponent* component)
{
	m_SelectComponent = component;
	LoadIMGName(m_SelectComponent);
}

bool CAnimationWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("캐릭터 애니메이션");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("추가", 50.f, 30.f);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::IMGAddCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("일괄선택", 70.f, 30.f);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::IMGMultiAddCallback);
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_IMGSelectName = CreateWidget<CEditorInputText>("IMGName");
	m_IMGSelectName->SetHideName("IMGName");
	m_IMGSelectName->SetSize(150.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.5f);
	button = CreateWidget<CEditorButton>("검색", 50.f, 30.f);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::IMGSearch);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.5f);
	button = CreateWidget<CEditorButton>("이름변경", 70.f, 30.f);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::IMGSetName);
	m_Slider = CreateWidget<CEditorSlider>("Slider");
	m_Slider->SetSliderCallback<CAnimationWindow>(this, &CAnimationWindow::IMGColorCallback);
	m_Slider->SetText("색상");
	m_IMGList = CreateWidget<CEditorListBox>("IMGListBox");
	m_IMGList->SetHideName("IMGListBox");
	m_IMGList->SetSize(200.f, 300.f);
	m_IMGList->SetPageItemCount(6);
	m_IMGList->SetSelectCallback<CAnimationWindow>(this, &CAnimationWindow::IMGClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPos(20.f, 25.f);
	m_Image = CreateWidget<CEditorImage>("Char_Animation");
	m_Image->SetTexture("Char_Animation", TEXT("teemo.png"));
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPos(0.f, 2.0f);
	m_AnimationName = CreateWidget<CEditorInputText>("AnimationName");
	m_AnimationName->SetHideName("AnimationName");
	m_AnimationName->SetSize(150.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPos(0.f, -2.0f);
	button = CreateWidget<CEditorButton>("생성", 50.f, 30.f);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::AnimationCreateCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPos(0.f, -2.0f);
	button = CreateWidget<CEditorButton>("삭제", 50.f, 30.f);
	button->SetColor(29, 47, 73, 255);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::FrameDeleteCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPos(0.f, -2.0f);
	button = CreateWidget<CEditorButton>("일괄삭제", 70.f, 30.f);
	button->SetColor(29, 47, 73, 255);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::FrameClearCallback);
	m_AnimationInfo[0] = CreateWidget<CEditorInputText>("재생시간");
	m_AnimationInfo[0]->SetSize(150.f, 30.f);
	m_AnimationInfo[0]->SetInputType(EImGuiInputType::Float);
	m_AnimationInfo[0]->SetFloat(1.0f);
	m_AnimationInfo[1] = CreateWidget<CEditorInputText>("재생비율");
	m_AnimationInfo[1]->SetSize(150.f, 30.f);
	m_AnimationInfo[1]->SetInputType(EImGuiInputType::Float);
	m_AnimationInfo[1]->SetFloat(1.0f);
	m_LoopCheck = CreateWidget<CEditorCheckBox>("반복재생", 50.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ReverseCheck = CreateWidget<CEditorCheckBox>("역재생", 50.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::AnimationChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("저장", 50.f, 30.f);
	button->SetClickCallback<CAnimationWindow>(this, &CAnimationWindow::AnimationSaveCallback);
	m_FrameList = CreateWidget<CEditorListBox>("FrameListBox");
	m_FrameList->SetHideName("FrameListBox");
	m_FrameList->SetSize(200.f, 300.f);
	m_FrameList->SetPageItemCount(6);
	m_FrameList->SetSelectCallback<CAnimationWindow>(this, &CAnimationWindow::IMGClickCallback);
	return true;
}

void CAnimationWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
		}
	}
}

void CAnimationWindow::IMGClickCallback(int index, const std::string& item)
{
	if(!m_SelectComponent)
	{
		return;
	}
	m_ListIndex = index;
	m_SelectIMGItem = item;
	m_IMGPath = "Character//" + m_FolderName + "//pixelart//sprite_sheet//" + m_SelectIMGItem + ".png";
	if (m_SelectComponent->GetOwner()->GetObjectTypeName() == "Boss")
	{
		m_IMGPath = "Boss\\" + m_FolderName + "\\" + m_SelectIMGItem + ".png";
	}
	m_IMGSelectName->SetText(m_SelectIMGItem.c_str());
	TCHAR* t_filename = new TCHAR[m_IMGPath.size() + 1];
	t_filename[m_IMGPath.size()] = 0;
	std::copy(m_IMGPath.begin(), m_IMGPath.end(), t_filename);
	m_Image->SetTexture(m_SelectIMGItem, t_filename);
	SAFE_DELETE_ARRAY(t_filename);
}

void CAnimationWindow::IMGAddCallback()
{
	m_FrameList->AddItem(m_SelectIMGItem);
}

void CAnimationWindow::IMGMultiAddCallback()
{
	int size = (int)m_IMGList->GetSize();
	for (int i = m_ListIndex; i < size; ++i)
	{
		m_FrameList->AddItem(m_IMGList->GetItem(i));
	}
	m_ListIndex = 0;
}

void CAnimationWindow::IMGColorCallback()
{
	m_Image->SetColor((unsigned char)m_Slider->GetPosition(0), (unsigned char)m_Slider->GetPosition(1),
		(unsigned char)m_Slider->GetPosition(2), (unsigned char)m_Slider->GetPosition(3));
}

void CAnimationWindow::IMGSearch()
{
	if(!m_SelectComponent)
	{
		return;
	}
	std::string name(m_IMGSelectName->GetText());
	if (name == "")
	{
		m_IMGList->Clear();
		LoadIMGName(m_SelectComponent);
		return;
	}
	size_t size = m_IMGList->GetSize();
	std::vector<std::string> vecName;
	std::string checkedName;
	for (size_t i = 0; i < size; ++i)
	{
		checkedName = m_IMGList->CheckPartitionItem(i, name);
		if (checkedName != "")
		{
			vecName.push_back(checkedName);
		}
	}
	size_t vecSize = vecName.size();
	if (vecSize != 0)
	{
		m_IMGList->Clear();
		for (size_t i = 0; i < vecSize; ++i)
		{
			m_IMGList->AddItem(vecName[i]);
		}
	}
}

void CAnimationWindow::IMGSetName()
{
	if (!m_SelectComponent)
	{
		return;
	}
	std::string name(m_IMGSelectName->GetText());
	if (name == "")
	{
		return;
	}
	m_SelectComponent->SetName(name);
	LoadIMGName(m_SelectComponent);
}

void CAnimationWindow::AnimationCreateCallback()
{
	std::string animationName= m_AnimationName->GetText();
	if(animationName == "")
	{
		return;
	}
	std::vector<const TCHAR*>   vecFileName;
	std::string fileName;
	int size = (int)m_FrameList->GetSize();
	for (int i = 0; i < size; ++i)
	{
		fileName = m_FrameList->GetItem(i);
		std::string path = "Character//" + m_FolderName + "//pixelart//sprite_sheet//" + fileName + ".png";
		if (m_SelectComponent->GetOwner()->GetObjectTypeName() == "Boss")
		{
			path = "Boss\\" + m_FolderName + "\\" + fileName + ".png";
		}
		TCHAR* t_filename = new TCHAR[path.size() + 1];
		t_filename[path.size()] = 0;
		std::copy(path.begin(), path.end(), t_filename);
	    vecFileName.push_back(t_filename);
	}
	CResourceManager::GetInst()->CreateAnimationSequence2D(animationName, animationName, vecFileName);
	for (int i = 0; i < size; ++i)
	{
		fileName = m_FrameList->GetItem(i);
		std::string path = "Character//" + m_FolderName + "//pixelart//sprite_sheet//" + fileName + ".png";
		if (m_SelectComponent->GetOwner()->GetObjectTypeName() == "Boss")
		{
			path = "Boss\\" + m_FolderName + "\\" + fileName + ".png";
		}
		TCHAR* t_filename = new TCHAR[path.size() + 1];
		t_filename[path.size()] = 0;
		std::copy(path.begin(), path.end(), t_filename);
		m_Image->SetTexture(fileName, t_filename);
		CResourceManager::GetInst()->AddAnimationSequence2DFrame(animationName, 0.f, 0.f,
			(float)m_Image->GetTexture()->GetWidth(), (float)m_Image->GetTexture()->GetHeight());
		SAFE_DELETE_ARRAY(t_filename);
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}
	vecFileName.clear();

	m_SelectComponent->GetMaterial(0)->SetBaseColorUnsignedChar(
		(unsigned char)m_Slider->GetPosition(0),(unsigned char)m_Slider->GetPosition(1), 
		(unsigned char)m_Slider->GetPosition(2), (unsigned char)m_Slider->GetPosition(3));
	m_SelectComponent->GetMaterial(0)->SetOpacity((unsigned char)m_Slider->GetPosition(3) / 255.f);

	m_Anim = m_SelectComponent->GetAnimation2D();
	if(!m_Anim)
	{
		m_Anim = m_SelectComponent->SetAnimation<CAnimation2D>("TempAnim");
	}
	m_Anim->AddAnimation(animationName, animationName);
	m_Anim->SetPlayTime(animationName,m_AnimationInfo[0]->GetFloat());
	m_Anim->SetPlayScale(animationName,m_AnimationInfo[1]->GetFloat());
	if(m_LoopCheck->GetCheck())
	{
		m_Anim->SetLoop(animationName, true);
	}
	if (m_ReverseCheck->GetCheck())
	{
		m_Anim->SetReverse(animationName, true);
	}
	m_Anim->SetCurrentAnimation(animationName);
}

void CAnimationWindow::AnimationChangeCallback()
{
	std::string animationName = m_AnimationName->GetText();
	if (animationName == "")
	{
		return;
	}
	m_SelectComponent->GetMaterial(0)->SetBaseColorUnsignedChar(
		(unsigned char)m_Slider->GetPosition(0), (unsigned char)m_Slider->GetPosition(1),
		(unsigned char)m_Slider->GetPosition(2), (unsigned char)m_Slider->GetPosition(3));
	m_SelectComponent->GetMaterial(0)->SetOpacity((unsigned char)m_Slider->GetPosition(3) / 255.f);

	m_Anim = m_SelectComponent->GetAnimation2D();
	if (!m_Anim)
	{
		m_Anim = m_SelectComponent->SetAnimation<CAnimation2D>("TempAnim");
	}
	m_Anim->AddAnimation(animationName, animationName);
	m_Anim->SetPlayTime(animationName, m_AnimationInfo[0]->GetFloat());
	m_Anim->SetPlayScale(animationName, m_AnimationInfo[1]->GetFloat());
	if (m_LoopCheck->GetCheck())
	{
		m_Anim->SetLoop(animationName, true);
	}
	if (m_ReverseCheck->GetCheck())
	{
		m_Anim->SetReverse(animationName, true);
	}
	m_Anim->SetCurrentAnimation(animationName);
}

void CAnimationWindow::AnimationSaveCallback()
{
	m_Anim = m_SelectComponent->GetAnimation2D();
	if (!m_Anim)
	{
		m_Anim = m_SelectComponent->SetAnimation<CAnimation2D>("TempAnim");
	}
	m_Anim->GetCurrentAnimationSequence()->Save(m_AnimationName->GetText(), ANIMATION_PATH);
}

void CAnimationWindow::FrameDeleteCallback()
{
	//똑같은 이름의 리스트는 먼저걸 삭제해줘야 삭제할수 있다
	if(m_ListIndex==-1|| m_ListIndex>= m_FrameList->GetSize())
	{
		return;
	}
	m_FrameList->DeleteItem(m_ListIndex);
}

void CAnimationWindow::FrameClearCallback()
{
	m_FrameList->Clear();
}

void CAnimationWindow::LoadIMGName(class CSpriteComponent* component)
{
	m_IMGList->Clear();
	std::string componentName=component->GetOwner()->GetObjectTypeName();
	if (!(componentName == "Player2D"|| componentName == "Monster"|| componentName == "Boss" || componentName == "Bullet"))
	{
		return;
	}
	m_FolderName = component->GetName();
	if(m_FolderName=="Monster"|| m_FolderName == "Bullet")
	{
		return;
	}
	const PathInfo* info = CPathManager::GetInst()->FindPath(TEXTURE_PATH);
	char	path[MAX_PATH] = {};
	char	directory[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	if (componentName == "Boss")
	{
		strcat_s(path, "Boss/");
		strcat_s(path, m_FolderName.c_str());
		strcpy_s(directory, path);
	}
	else
	{
		strcat_s(path, "Character/");
		strcat_s(path, m_FolderName.c_str());
		strcpy_s(directory, path);
		strcat_s(path, "/pixelart/sprite_sheet/");
	}
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		m_IMGList->AddItem(name);
	}
}