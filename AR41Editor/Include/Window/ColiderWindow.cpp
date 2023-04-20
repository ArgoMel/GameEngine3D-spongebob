#include "ColiderWindow.h"
#include "CollisionManager.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorCheckBox.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2d.h"
#include "Component/ColliderOBB2d.h"
#include "Component/ColliderSphere2d.h"
#include "GameObject/GameObject.h"

CColiderWindow::CColiderWindow()
	: m_ColiderCB(nullptr)
	, m_ChannelList(nullptr)
	, m_ProfileRelationList(nullptr)
	, m_ProfileRelationCheck(nullptr)
	, m_ColiderSizeText{}
	, m_ColliderIndex(0)
	, m_ProfileName(nullptr)
{
}

CColiderWindow::~CColiderWindow()
{
}

void CColiderWindow::SetSelectComponent(CCollider* component)
{
	if (component->GetComponentTypeName() == "ColliderBox2D")
	{
		m_SelectBox2D = (CColliderBox2D*)component;
		m_SelectSphere2D = nullptr;
		m_SelectOBB2D = nullptr;
		m_ColiderSizeText[0]->SetFloat(m_SelectBox2D->GetBoxSize().x);
		m_ColiderSizeText[1]->SetFloat(m_SelectBox2D->GetBoxSize().y);
	}
	else if (component->GetComponentTypeName() == "ColliderOBB2D")
	{
		m_SelectBox2D = nullptr;
		m_SelectSphere2D = nullptr;
		m_SelectOBB2D = (CColliderOBB2D*)component;
		m_ColiderSizeText[0]->SetFloat(m_SelectOBB2D->GetInfo().length[0]);
		m_ColiderSizeText[1]->SetFloat(m_SelectOBB2D->GetInfo().length[1]);
	}
	else if (component->GetComponentTypeName() == "ColliderSphere2D")
	{
		m_SelectBox2D = nullptr;
		m_SelectSphere2D = (CColliderSphere2D*)component;
		m_SelectOBB2D = nullptr;
		m_ColiderSizeText[0]->SetFloat(m_SelectSphere2D->GetInfo().radius);
		m_ColiderSizeText[1]->SetFloat(0.f);
	}
}

void CColiderWindow::SetSelectSprite(CSpriteComponent* component)
{
	m_SelectSprite = component;
}

bool CColiderWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("콜라이더2D");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_ColiderCB = CreateWidget<CEditorComboBox>("ChannelComboBox");
	m_ColiderCB->SetHideName("ChannelComboBox");
	m_ColiderCB->SetPrevViewName("Box2D");
	m_ColiderCB->AddItem("Box2D");
	m_ColiderCB->AddItem("OBB2D");
	m_ColiderCB->AddItem("Sphere2D");
	m_ColiderCB->SetSelectPrevViewName(true);
	m_ColiderCB->SetSelectCallback<CColiderWindow>(this,&CColiderWindow::ColiderClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("생성", 50.f, 30.f);
	button->SetClickCallback<CColiderWindow>(this, &CColiderWindow::ColiderCreateCallback);
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.0f);
	m_ProfileName = CreateWidget<CEditorInputText>("profile", 80.f, 30.f);
	m_ProfileName->SetHideName("profile");
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.0f);
	button = CreateWidget<CEditorButton>("프로파일 생성", 100.f, 30.f);
	button->SetClickCallback<CColiderWindow>(this, &CColiderWindow::ProfileCreateCallback);
	m_ChannelList = CreateWidget<CEditorListBox>("ChannelListBox");
	m_ChannelList->SetHideName("ChannelListBox");
	m_ChannelList->SetSize(200.f, 300.f);
	m_ChannelList->SetPageItemCount(6);
	m_ChannelList->SetSelectCallback<CColiderWindow>(this, &CColiderWindow::ChannelClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("설정", 50.f, 30.f);
	button->SetClickCallback<CColiderWindow>(this, &CColiderWindow::ChannelChangeCallback);
	m_ProfileRelationList = CreateWidget<CEditorListBox>("ProfileRelationList");
	m_ProfileRelationList->SetHideName("ProfileRelationList");
	m_ProfileRelationList->SetSize(200.f, 300.f);
	m_ProfileRelationList->SetPageItemCount(6);
	m_ProfileRelationList->SetSelectCallback<CColiderWindow>(this, &CColiderWindow::RelationClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ProfileRelationCheck= CreateWidget<CEditorCheckBox>("충돌");
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPos(-66.f,40.f);
	button = CreateWidget<CEditorButton>("적용", 50.f, 30.f);
	button->SetClickCallback<CColiderWindow>(this, &CColiderWindow::RelationChangeCallback);
	m_ColiderSizeText[0] = CreateWidget<CEditorInputText>("SizeX",80.f,30.f);
	m_ColiderSizeText[0]->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText[0]->SetInputCallback<CColiderWindow>(this, &CColiderWindow::ColiderChangeCallback);
	m_ColiderSizeText[1] = CreateWidget<CEditorInputText>("SizeY", 80.f, 30.f);
	m_ColiderSizeText[1]->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText[1]->SetInputCallback<CColiderWindow>(this, &CColiderWindow::ColiderChangeCallback);
	LoadChannelName();
	return true;
}

void CColiderWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectSprite)
	{
		if (!m_SelectSprite->GetActive())
		{
			m_SelectSprite = nullptr;
		}
	}
	if (m_SelectBox2D)
	{
		if (!m_SelectBox2D->GetActive())
		{
			m_SelectBox2D = nullptr;
		}
	}
	else if (m_SelectSphere2D)
	{
		if (!m_SelectSphere2D->GetActive())
		{
			m_SelectSphere2D = nullptr;
		}
	}
	else if (m_SelectOBB2D)
	{
		if (!m_SelectOBB2D->GetActive())
		{
			m_SelectOBB2D = nullptr;
		}
	}
}

void CColiderWindow::ColiderClickCallback(int index, const std::string& item)
{
	m_ColliderIndex = index;
}

void CColiderWindow::ChannelClickCallback(int index, const std::string& item)
{
	m_SelectChannelItem = item;
}

void CColiderWindow::RelationClickCallback(int index, const std::string& item)
{
	m_SelectRelationItem = item;
	bool isCollision=CCollisionManager::GetInst()->CheckCollisionInteraction(m_SelectChannelItem
		, m_SelectRelationItem, ECollision_Interaction::Collision);
	m_ProfileRelationCheck->SetCheck(isCollision);
}

void CColiderWindow::ColiderCreateCallback()
{
//compoenentwindow가 colliderwindwo를 가지고 있어서 인클루드 안됌
// 생성후에 클래스윈도우 1번 눌러줘야한다
	if(!m_SelectSprite)
	{
		return;
	}
	if (m_ColliderIndex==(int)ECollider2D_Type::Box2D)
	{
		CColliderBox2D* m_Box2D = m_SelectSprite->GetOwner()->CreateComponent<CColliderBox2D>("Box2D");
		m_SelectSprite->AddChild(m_Box2D);
		m_Box2D->SetRelativePosition(0.f, 0.f);
		m_Box2D->SetPivot(0.5f, 0.5f);
	}
	else if (m_ColliderIndex == (int)ECollider2D_Type::OBB2D)
	{
		CColliderOBB2D* m_OBB2D = m_SelectSprite->GetOwner()->CreateComponent<CColliderOBB2D>("OBB2D");
		m_SelectSprite->AddChild(m_OBB2D);
		m_OBB2D->SetRelativePosition(0.f, 0.f);
		m_OBB2D->SetPivot(0.5f, 0.5f);
	}
	else if (m_ColliderIndex == (int)ECollider2D_Type::Sphere2D)
	{
		CColliderSphere2D* m_Sphere2D = m_SelectSprite->GetOwner()->CreateComponent<CColliderSphere2D>("Sphere2D");
		m_SelectSprite->AddChild(m_Sphere2D);
		m_Sphere2D->SetRelativePosition(0.f, 0.f);
		m_Sphere2D->SetPivot(0.5f, 0.5f);
	}
}

void CColiderWindow::ProfileCreateCallback()
{
	std::string name(m_ProfileName->GetText());
	if (name == "")
	{
		return;
	}
	CCollisionManager::GetInst()->CreateChannel(name, ECollision_Interaction::Collision);
	CCollisionManager::GetInst()->CreateProfile(name, name, true);
	m_ChannelList->Clear();
	m_ProfileRelationList->Clear();
	LoadChannelName();
}

void CColiderWindow::ColiderChangeCallback()
{
	float sizeX=m_ColiderSizeText[0]->GetFloat();
	float sizeY = m_ColiderSizeText[1]->GetFloat();
	if (m_SelectBox2D)
	{
		m_SelectBox2D->SetBoxSize(sizeX, sizeY);
	}
	else if (m_SelectSphere2D)
	{
		m_SelectSphere2D->SetRadius(sizeX);
	}
	else if (m_SelectOBB2D)
	{
		m_SelectOBB2D->SetBoxHalfSize(sizeX, sizeY);
	}
}

void CColiderWindow::ChannelChangeCallback()
{
	if (m_SelectBox2D)
	{
		m_SelectBox2D->SetCollisionProfile(m_SelectChannelItem);
	}
	else if (m_SelectSphere2D)
	{
		m_SelectSphere2D-> SetCollisionProfile(m_SelectChannelItem);
	}
	else if (m_SelectOBB2D)
	{
		m_SelectOBB2D->SetCollisionProfile(m_SelectChannelItem);
	}
}

void CColiderWindow::RelationChangeCallback()
{
	ECollision_Interaction interaction = 
		m_ProfileRelationCheck->GetCheck() ? ECollision_Interaction::Collision : ECollision_Interaction::Ignore;
	CCollisionManager::GetInst()->SetCollisionInteraction(m_SelectChannelItem, m_SelectRelationItem, interaction);
}

void CColiderWindow::LoadChannelName()
{
	auto	iterBegin = CCollisionManager::GetInst()->MapProfileBegin();
	auto	iterEnd = CCollisionManager::GetInst()->MapProfileEnd();
	for (auto iter=iterBegin; iter != iterEnd; ++iter)
	{
		m_ChannelList->AddItem(iter->second->name);
		m_ProfileRelationList->AddItem(iter->second->name);
	}
}