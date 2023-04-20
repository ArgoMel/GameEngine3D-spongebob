#include "TileMapWidgetList.h"
#include "Input.h"
#include "Engine.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorGUIManager.h"
#include "Component/CameraComponent.h"
#include "Component/TileMapComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "../DetailWindow.h"
#include "../../Editor.h"

CTileMapWidgetList::CTileMapWidgetList()
	: m_TileMap(nullptr)
	, m_CountX(nullptr)
	, m_CountY(nullptr)
	, m_TileSizeX(nullptr)
	, m_TileSizeY(nullptr)
	, m_ShapeCombo(nullptr)
	, m_TileEditCombo(nullptr)
	, m_TileOptionCombo(nullptr)
	, m_TileFrameList(nullptr)
	, m_TileFrameImage(nullptr)
{
}

CTileMapWidgetList::~CTileMapWidgetList()
{
}

void CTileMapWidgetList::SetTileMapComponent(CTileMapComponent* tileMap)
{
	m_TileMap = tileMap;
}

bool CTileMapWidgetList::Init()
{
	CEditorTree<void*>* category = CreateWidget<CEditorTree<void*>>("TileMapComponent");
	category->SetHideName("TileMapComponent");
	category->SetSize(400.f, 300.f);
	category->AddItem(nullptr, "TileMapComponent");
	category->AddItem(nullptr, "TileMap", "TileMapComponent");
	CEditorGroup* createGroup = category->CreateWidget<CEditorGroup>("TileMap", "CreateGroup");
	m_ShapeCombo = createGroup->CreateWidget<CEditorComboBox>("TileShape", 120.f, 30.f);
	m_ShapeCombo->SetHideName("TileShape");
	m_ShapeCombo->AddItem("사각형");
	m_ShapeCombo->AddItem("마름모");
	m_ShapeCombo->Sort(false);
	m_ShapeCombo->SetSelectPrevViewName(true);
	m_ShapeCombo->SetSelectIndex(1);
	m_ShapeCombo->SetPrevViewName("마름모");
	m_ShapeCombo->SetSelectCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::SetShapeCallback);
	m_CountX = createGroup->CreateWidget<CEditorInputText>("CountX", 100.f, 30.f);
	m_CountX->SetInputType(EImGuiInputType::Int);
	m_CountX->SetInt(100);
	createGroup->CreateWidget<CEditorSameLine>("Line");
	m_CountY = createGroup->CreateWidget<CEditorInputText>("CountY", 100.f, 30.f);
	m_CountY->SetInputType(EImGuiInputType::Int);
	m_CountY->SetInt(100);
	m_TileSizeX = createGroup->CreateWidget<CEditorInputText>("SizeX", 100.f, 30.f);
	m_TileSizeX->SetInputType(EImGuiInputType::Float);
	m_TileSizeX->SetFloat(160.f);
	createGroup->CreateWidget<CEditorSameLine>("Line");
	m_TileSizeY = createGroup->CreateWidget<CEditorInputText>("SizeY", 100.f, 30.f);
	m_TileSizeY->SetInputType(EImGuiInputType::Float);
	m_TileSizeY->SetFloat(80.f);
	CEditorButton* button = createGroup->CreateWidget<CEditorButton>("CreateTileMap", 150.f, 40.f);
	button->SetClickCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::CreateTileMapClick);
	category->CreateWidget<CEditorSameLine>("TileMap", "Line");
	CEditorGroup* editGroup = category->CreateWidget<CEditorGroup>("TileMap", "EditGroup");
	m_TileEditCombo = editGroup->CreateWidget<CEditorComboBox>("EditMode", 120.f, 30.f);
	m_TileEditCombo->SetHideName("EditMode");
	m_TileEditCombo->AddItem("이미지");
	m_TileEditCombo->AddItem("옵션");
	m_TileEditCombo->Sort(false);
	m_TileEditCombo->SetSelectPrevViewName(true);
	m_TileEditCombo->SetSelectIndex(1);
	m_TileEditCombo->SetPrevViewName("옵션");
	m_TileEditCombo->SetSelectCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::SetEditModeCallback);
	editGroup->CreateWidget<CEditorSameLine>("Line");
	m_TileOptionCombo = editGroup->CreateWidget<CEditorComboBox>("TileOption", 120.f, 30.f);
	m_TileOptionCombo->SetHideName("TileOption");
	m_TileOptionCombo->AddItem("일반");
	m_TileOptionCombo->AddItem("이동불가");
	m_TileOptionCombo->Sort(false);
	m_TileOptionCombo->SetSelectPrevViewName(true);
	m_TileOptionCombo->SetSelectIndex(1);
	m_TileOptionCombo->SetPrevViewName("이동불가");
	m_TileFrameList = editGroup->CreateWidget<CEditorListBox>("FrameList", 200.f, 300.f);
	m_TileFrameList->SetHideName("FrameList");
	for (int i = 0; i <= 379; ++i)
	{
		char	number[32] = {};
		_itoa_s(i, number, 10);
		m_TileFrameList->AddItem(number);
	}
	m_TileFrameList->Sort(false);
	m_TileFrameList->SetSelectCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::SetFrameListCallback);
	m_TileFrameImage = editGroup->CreateWidget<CEditorImage>("TileFrameImage");
	return true;
}

void CTileMapWidgetList::Render()
{
	CComponentWidgetList::Render();
	if (GetAsyncKeyState('W') & 0x8000)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionY(200.f * g_DeltaTime);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionY(-200.f * g_DeltaTime);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionX(200.f * g_DeltaTime);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionX(-200.f * g_DeltaTime);
	}
	Vector2 pos = CInput::GetInst()->GetMouseWorldPos();
	int	index = m_TileMap->GetTileIndex(pos);
	m_TileMap->SetEditorMouseOnTile(index);
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		switch ((ETileEditMode)m_TileEditCombo->GetSelectIndex())
		{
		case ETileEditMode::Image:
			m_TileMap->ChangeTileFrame(pos, m_TileFrameList->GetSelectIndex());
			break;
		case ETileEditMode::Option:
			m_TileMap->ChangeTileOption(pos, (ETileOption)m_TileOptionCombo->GetSelectIndex());
			break;
		}
	}
}

void CTileMapWidgetList::CreateTileMapClick()
{
	m_TileMap->CreateTile((ETileShape)m_ShapeCombo->GetSelectIndex(), m_CountX->GetInt(), m_CountY->GetInt()
		,Vector2(m_TileSizeX->GetFloat(), m_TileSizeY->GetFloat()));
	CTexture* texture = m_TileMap->GetTileMaterial()->GetTexture(0);
	m_TileFrameImage->SetTexture(texture);
	m_TileFrameImage->SetImageStart(0.f, 0.f);
	m_TileFrameImage->SetImageEnd(160.f, 80.f);
}

void CTileMapWidgetList::SetShapeCallback(int index, const std::string& item)
{
}

void CTileMapWidgetList::SetEditModeCallback(int index, const std::string& item)
{
}

void CTileMapWidgetList::SetFrameListCallback(int index, const std::string& item)
{
	m_TileFrameImage->SetImageIndex(index);
}