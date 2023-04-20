#include "ColliderPixelWindow.h"
#include "Engine.h"
#include "PathManager.h"
#include "CollisionManager.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorComboBox.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderPixel.h"
#include "GameObject/GameObject.h"

CColliderPixelWindow::CColliderPixelWindow()
	: m_ChannelList(nullptr)
	, m_ProfileRelationList(nullptr)
	, m_ProfileRelationCheck(nullptr)
	, m_ColorTypeIndex(0)
	, m_AlphaTypeIndex(0)
	, m_ProfileName(nullptr)
	, m_Slider(nullptr)
	, m_ColorTypeCB(nullptr)
	, m_AlphaTypeCB(nullptr)
{
}

CColliderPixelWindow::~CColliderPixelWindow()
{

}

void CColliderPixelWindow::SetSelectComponent(CCollider* component)
{
	m_SelectComponent = (CColliderPixel*)component;
	m_Slider->SetPosition(0, m_SelectComponent->GetInfo().typeColor[0]);
	m_Slider->SetPosition(1, m_SelectComponent->GetInfo().typeColor[1]);
	m_Slider->SetPosition(2, m_SelectComponent->GetInfo().typeColor[2]);
	m_Slider->SetPosition(3, m_SelectComponent->GetInfo().typeColor[3]);
	m_ColorTypeIndex = (int)m_SelectComponent->GetInfo().pixelColorCollisionType;
	std::string colorName= m_ColorTypeCB->GetItem(m_ColorTypeIndex);
	m_ColorTypeCB->SetPrevViewName(colorName);
	m_AlphaTypeIndex = (int)m_SelectComponent->GetInfo().pixelAlphaCollisionType;
	std::string alphaName = m_AlphaTypeCB->GetItem(m_AlphaTypeIndex);
	m_AlphaTypeCB->SetPrevViewName(alphaName);
}

void CColliderPixelWindow::SetSelectSprite(CSpriteComponent* component)
{
	m_SelectSprite = component;
}

bool CColliderPixelWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("콜라이더 픽셀");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("생성", 50.f, 30.f);
	button->SetColor(29, 47, 73, 255);
	button->SetClickCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::ColiderCreateCallback);
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.0f);
	m_ProfileName = CreateWidget<CEditorInputText>("profile", 80.f, 30.f);
	m_ProfileName->SetHideName("profile");
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.0f);
	button = CreateWidget<CEditorButton>("프로파일 생성", 100.f, 30.f);
	button->SetClickCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::ProfileCreateCallback);
	m_ChannelList = CreateWidget<CEditorListBox>("ChannelListBox");
	m_ChannelList->SetHideName("ChannelListBox");
	m_ChannelList->SetSize(200.f, 300.f);
	m_ChannelList->SetPageItemCount(6);
	m_ChannelList->SetSelectCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::ChannelClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("설정", 50.f, 30.f);
	button->SetClickCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::ChannelChangeCallback);
	m_ProfileRelationList = CreateWidget<CEditorListBox>("ProfileRelationList");
	m_ProfileRelationList->SetHideName("ProfileRelationList");
	m_ProfileRelationList->SetSize(200.f, 300.f);
	m_ProfileRelationList->SetPageItemCount(6);
	m_ProfileRelationList->SetSelectCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::RelationClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ProfileRelationCheck = CreateWidget<CEditorCheckBox>("충돌");
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPos(-66.f, 40.f);
	button = CreateWidget<CEditorButton>("적용", 50.f, 30.f);
	button->SetClickCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::RelationChangeCallback);
	button = CreateWidget<CEditorButton>("이미지 변경", 100.f, 30.f);
	button->SetClickCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::PixelCreateCallback);
	m_Slider = CreateWidget<CEditorSlider>("Slider");
	m_Slider->SetText("색상");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_ColorTypeCB = CreateWidget<CEditorComboBox>("ColorTypeComboBox");
	m_ColorTypeCB->SetHideName("ColorTypeComboBox");
	m_ColorTypeCB->SetPrevViewName("None");
	m_ColorTypeCB->AddItem("None");
	m_ColorTypeCB->AddItem("Color_Ignore");
	m_ColorTypeCB->AddItem("Color_Confirm");
	m_ColorTypeCB->SetSelectPrevViewName(true);
	m_ColorTypeCB->SetSelectCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::ColorTypeClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_AlphaTypeCB = CreateWidget<CEditorComboBox>("AlphaTypeComboBox");
	m_AlphaTypeCB->SetHideName("AlphaTypeComboBox");
	m_AlphaTypeCB->SetPrevViewName("None");
	m_AlphaTypeCB->AddItem("None");
	m_AlphaTypeCB->AddItem("Alpha_Ignore");
	m_AlphaTypeCB->AddItem("Alpha_Confirm");
	m_AlphaTypeCB->SetSelectPrevViewName(true);
	m_AlphaTypeCB->SetSelectCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::AlphaTypeClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.f);
	button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CColliderPixelWindow>(this, &CColliderPixelWindow::PixelInfoChangeCallback);
	LoadChannelName();
	return true;
}

void CColliderPixelWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectSprite)
	{
		if (!m_SelectSprite->GetActive())
		{
			m_SelectSprite = nullptr;
		}
	}
	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
		}
	}
}

void CColliderPixelWindow::ChannelClickCallback(int index, const std::string& item)
{
	m_SelectChannelItem = item;
}

void CColliderPixelWindow::RelationClickCallback(int index, const std::string& item)
{
	m_SelectRelationItem = item;
	bool isCollision = CCollisionManager::GetInst()->CheckCollisionInteraction(m_SelectChannelItem
		, m_SelectRelationItem, ECollision_Interaction::Collision);
	m_ProfileRelationCheck->SetCheck(isCollision);
}

void CColliderPixelWindow::ColorTypeClickCallback(int index, const std::string& item)
{
	m_ColorTypeIndex = index;
}

void CColliderPixelWindow::AlphaTypeClickCallback(int index, const std::string& item)
{
	m_AlphaTypeIndex = index;
	if (m_AlphaTypeIndex > 0)
	{
		m_AlphaTypeIndex += 2;
	}
}

void CColliderPixelWindow::ColiderCreateCallback()
{
	if (!m_SelectSprite)
	{
		return;
	}
	CColliderPixel* m_Pixel2D = m_SelectSprite->GetOwner()->CreateComponent<CColliderPixel>("Pixel2D");
	m_Pixel2D->SetInfo("PixelCollision", TEXT("PixelCollision.png"));
	m_Pixel2D->SetPixelColorCollisionType(EPixelCollision_Type::Color_Confirm);
	m_Pixel2D->SetPixelColor(255, 0, 255);
	m_SelectSprite->AddChild(m_Pixel2D);
	m_Pixel2D->SetRelativePosition(0.f, 0.f);
	m_Pixel2D->SetPivot(0.5f, 0.5f);
}

void CColliderPixelWindow::ProfileCreateCallback()
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

void CColliderPixelWindow::PixelCreateCallback()
{
	if (!m_SelectComponent)
	{
		return;
	}
	OPENFILENAME	ofn = {};
	TCHAR	fullPath[MAX_PATH] = {};
	TCHAR	filter[] = TEXT("모든 파일\0*.*\0PNG\0*.png\0JPG\0*.jpg\0BMP\0*.bmp\0GIF\0*.gif");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fullPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->path;
	if (GetOpenFileName(&ofn) != 0)
	{
		TCHAR	wTexName[256] = {};
		_wsplitpath_s(fullPath, 0, 0, 0, 0, wTexName, 256, 0, 0);
		char	texName[256] = {};
		int length = (int)WideCharToMultiByte(CP_ACP, 0, wTexName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, wTexName, -1, texName, length, nullptr, nullptr);
		m_SelectComponent->SetInfoFullPath(texName, fullPath);
	}
}

void CColliderPixelWindow::ChannelChangeCallback()
{
	if (m_SelectComponent)
	{
		m_SelectComponent->SetCollisionProfile(m_SelectChannelItem);
	}
}

void CColliderPixelWindow::RelationChangeCallback()
{
	ECollision_Interaction interaction =
		m_ProfileRelationCheck->GetCheck() ? ECollision_Interaction::Collision : ECollision_Interaction::Ignore;
	CCollisionManager::GetInst()->SetCollisionInteraction(m_SelectChannelItem, m_SelectRelationItem, interaction);
}

void CColliderPixelWindow::PixelInfoChangeCallback()
{
	if (m_SelectComponent)
	{
		m_SelectComponent->SetPixelColor((unsigned char)m_Slider->GetPosition(0), (unsigned char)m_Slider->GetPosition(1),
			(unsigned char)m_Slider->GetPosition(2));
		m_SelectComponent->SetPixelAlpha((unsigned char)m_Slider->GetPosition(3));
		m_SelectComponent->SetPixelColorCollisionType((EPixelCollision_Type)m_ColorTypeIndex);
		m_SelectComponent->SetPixelAlphaCollisionType((EPixelCollision_Type)m_AlphaTypeIndex);
	}
}

void CColliderPixelWindow::LoadChannelName()
{
	auto	iterBegin = CCollisionManager::GetInst()->MapProfileBegin();
	auto	iterEnd = CCollisionManager::GetInst()->MapProfileEnd();
	for (auto iter = iterBegin; iter != iterEnd; ++iter)
	{
		m_ChannelList->AddItem(iter->second->name);
		m_ProfileRelationList->AddItem(iter->second->name);
	}
}