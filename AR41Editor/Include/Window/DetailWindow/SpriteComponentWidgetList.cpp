#include "SpriteComponentWidgetList.h"
#include "../Animation2DWindow.h"
#include "../DetailWindow.h"
#include "../ResourceWindow.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorGUIManager.h"
#include "Component/SpriteComponent.h"
#include "Animation/Animation2D.h"
#include "Resource/ResourceManager.h"
#include "Resource/Animation/AnimationManager.h"
#include "Resource/Animation/AnimationSequence2D.h"

CSpriteComponentWidgetList::CSpriteComponentWidgetList()
	: m_AnimationClass(nullptr)
	, m_AnimationList(nullptr)
	, m_PlayTime(nullptr)
	, m_PlayScale(nullptr)
	, m_LoopCheck(nullptr)
	, m_ReverseCheck(nullptr)
	, m_SequenceName(nullptr)
	, m_Animation2DDataName(nullptr)
	, m_Sprite(nullptr)
	, m_Image(nullptr)
	, m_ImageName(nullptr)
{
}

CSpriteComponentWidgetList::~CSpriteComponentWidgetList()
{
}

void CSpriteComponentWidgetList::SetSpriteContent(CSpriteComponent* sprite)
{
	m_Sprite = sprite;
	m_AnimationClass->SetText(m_Sprite->GetAnimationName().c_str());
	m_AnimationList->Clear();
	std::vector<std::string>	vecAnimNames;
	m_Sprite->GetAnimationNames(vecAnimNames);
	size_t	size = vecAnimNames.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_AnimationList->AddItem(vecAnimNames[i]);
	}
}

void CSpriteComponentWidgetList::SetSelectAnimationSequence2DName(const std::string& name)
{
	m_SequenceName->SetText(name.c_str());
}

bool CSpriteComponentWidgetList::Init()
{
	CEditorTree<void*>* category = CreateWidget<CEditorTree<void*>>("SpriteComponent");
	category->SetHideName("SpriteComponent");
	category->SetSize(400.f, 300.f);
	category->AddItem(nullptr, "SpriteComponent");
	category->AddItem(nullptr, "Sprite", "SpriteComponent");
	category->AddItem(nullptr, "Animation", "SpriteComponent");
	m_ImageName =category->CreateWidget<CEditorInputText>("Sprite", "SpriteName");
	m_Image=category->CreateWidget<CEditorImage>("Sprite", "SpriteImage");
	m_Image->SetIsFixed(true);
	category->CreateWidget<CEditorSameLine>("Sprite", "Line");
	CEditorButton* button = category->CreateWidget<CEditorButton>("Sprite", "SelectTexture");
	button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::SelectTextureButtonClick);
	m_AnimationClass = category->CreateWidget<CEditorInputText>("Animation", "AnimationClass", 100.f, 30.f);
	m_AnimationClass->ReadOnly(true);
	m_AnimationClass->SetText("None");
	m_AnimationClass->SetSize(150.f, 30.f);
	category->CreateWidget<CEditorSameLine>("Animation", "Line");
	button = category->CreateWidget<CEditorButton>("Animation", "CreateAnimation");
	button->SetSize(150.f, 30.f);
	button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::CreateAnimationButtonClick);
	m_AnimationList = category->CreateWidget<CEditorListBox>("Animation", "AnimationList");
	m_AnimationList->SetHideName("AnimationList");
	m_AnimationList->SetSize(150.f, 300.f);
	m_AnimationList->SetPageItemCount(6);
	m_AnimationList->SetSelectCallback<CSpriteComponentWidgetList>
		(this, &CSpriteComponentWidgetList::AnimationSelectCallback);
	category->CreateWidget<CEditorSameLine>("Animation", "Line");
	CEditorGroup* animDataGroup = category->CreateWidget<CEditorGroup>("Animation", "Animation2DData");
	m_Animation2DDataName = animDataGroup->CreateWidget<CEditorInputText>("Animation2DDataName", 150.f, 30.f);
	m_Animation2DDataName->SetText("None");
	m_SequenceName = animDataGroup->CreateWidget<CEditorInputText>("SequenceName", 150.f, 30.f);
	m_SequenceName->ReadOnly(true);
	m_SequenceName->SetText("None");
	m_PlayTime = animDataGroup->CreateWidget<CEditorInputText>("PlayTime", 100.f, 30.f);
	m_PlayTime->SetInputType(EImGuiInputType::Float);
	m_PlayScale = animDataGroup->CreateWidget<CEditorInputText>("PlayScale", 100.f, 30.f);
	m_PlayScale->SetInputType(EImGuiInputType::Float);
	m_LoopCheck = animDataGroup->CreateWidget<CEditorCheckBox>("Loop", 100.f, 30.f);
	m_ReverseCheck = animDataGroup->CreateWidget<CEditorCheckBox>("Reverse", 100.f, 30.f);
	button = animDataGroup->CreateWidget<CEditorButton>("AddAnimation2D", 150.f, 30.f);
	button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::AddAnimation2DData);
	animDataGroup->CreateWidget<CEditorSameLine>("Line");
	button = animDataGroup->CreateWidget<CEditorButton>("DeleteAnimation2D", 150.f, 30.f);
	button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::DeleteAnimation2DData);
	button = animDataGroup->CreateWidget<CEditorButton>("ModifyAnimation2D", 150.f, 30.f);
	button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::ModifyAnimation2DData);
	return true;
}

void CSpriteComponentWidgetList::SelectTextureButtonClick()
{
	CResourceWindow* resourceWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CResourceWindow>("ResourceWindow");
	if (!resourceWindow)
	{
		return;
	}
	CTexture* texture = resourceWindow->GetSelectTexture();
	if (texture)
	{
		m_Image->SetTexture(texture);
	}
	if(m_Sprite)
	{
		m_Sprite->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, m_ImageName->GetTextUTF8(), texture);
	}
}

void CSpriteComponentWidgetList::CreateAnimationButtonClick()
{
	CAnimation2DWindow* animation2DWindow = 
		CEditorGUIManager::GetInst()->FindEditorWindow<CAnimation2DWindow>("Animation2DWindow");
	if(!animation2DWindow)
	{
		return;
	}
	std::string name = animation2DWindow->GetAnimation2DSelectName();
	if (name.empty())
	{
		return;
	}
	CSpriteComponent* sprite = (CSpriteComponent*)m_DetailWindow->GetSelectComponent();
	if (name == "Animation2D")
	{
		sprite->SetAnimation<CAnimation2D>("Animation2D");
		m_AnimationClass->SetText("Animation2D");
	}
}

void CSpriteComponentWidgetList::AnimationSelectCallback(int Index, const std::string& Item)
{
}

void CSpriteComponentWidgetList::AddAnimation2DData()
{
	CAnimation2D* animation = m_Sprite->GetAnimation2D();
	if (animation->FindAnimation(m_Animation2DDataName->GetText()))
	{
		return;
	}
	CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequence2D(m_SequenceName->GetText());
	if (!sequence)
	{
		return;
	}
	animation->AddAnimation(m_Animation2DDataName->GetText(), sequence, m_PlayTime->GetFloat(), m_PlayScale->GetFloat(),
		m_LoopCheck->GetCheck(), m_ReverseCheck->GetCheck());
	m_AnimationList->AddItem(m_Animation2DDataName->GetText());
}

void CSpriteComponentWidgetList::DeleteAnimation2DData()
{
}

void CSpriteComponentWidgetList::ModifyAnimation2DData()
{
}
