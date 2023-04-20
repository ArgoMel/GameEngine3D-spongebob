#include "DetailWindow.h"
#include "Animation2DWindow.h"
#include "Engine.h"
#include "PathManager.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorText.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorGUIManager.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TileMapComponent.h"
#include "Component/TargetArm.h"
#include "DetailWindow/CameraWidgetList.h"
#include "DetailWindow/TargetArmWidgetList.h"
#include "DetailWindow/SpriteComponentWidgetList.h"
#include "DetailWindow/SceneComponentWidgetList.h"
#include "DetailWindow/PrimitiveWidgetList.h"
#include "DetailWindow/TileMapWidgetList.h"

CDetailWindow::CDetailWindow()
{
}

CDetailWindow::~CDetailWindow()
{
	ClearWidget();
	size_t	size = m_vecComponentWidgetList.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddWidget(m_vecComponentWidgetList[i]);
	}
}

CComponentWidgetList* CDetailWindow::GetComponentWidgetList(const std::string& name)
{
	size_t	size = m_vecComponentWidgetList.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecComponentWidgetList[i]->GetName() == name)
		{
			return m_vecComponentWidgetList[i];
		}
	}
	return nullptr;
}

void CDetailWindow::SetSelectComponent(CSceneComponent* component)
{
	if (m_SelectComponent && component)
	{
		if (m_SelectComponent != component)
		{
			ClearWidget();
			ChangeWidget(component);
		}
	}
	else if (!m_SelectComponent && component)
	{
		ClearWidget();
		ChangeWidget(component);
	}
	m_SelectComponent = component;
}

bool CDetailWindow::Init()
{
	m_vecComponentWidgetList.resize((size_t)ESceneComponentType::Max);
	for (int i = 0; i < (int)ESceneComponentType::Max; ++i)
	{
		CreateEditorWidgetList((ESceneComponentType)i);
	}
	ClearWidget();
	return true;
}

void CDetailWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
			ClearWidget();
		}
	}
}

void CDetailWindow::ChangeWidget(CSceneComponent* component)
{
	if (component->GetComponentTypeName() == "SceneComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::Scene]);
	}
	else if (component->GetComponentTypeName() == "SpriteComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::Sprite]);
		CSpriteComponentWidgetList* spriteWidget = 
			(CSpriteComponentWidgetList*)m_vecComponentWidgetList[(int)ESceneComponentType::Sprite];
		spriteWidget->SetSpriteContent((CSpriteComponent*)component);
		CAnimation2DWindow* anim2DWindow = 
			CEditorGUIManager::GetInst()->FindEditorWindow<CAnimation2DWindow>("Animation2DWindow");
		if(!anim2DWindow)
		{
			return;
		}
		spriteWidget->SetSelectAnimationSequence2DName(anim2DWindow->GetAnimation2DSequenceSelectName());
	}
	else if (component->GetComponentTypeName() == "CameraComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::Camera]);
	}
	else if (component->GetComponentTypeName() == "TargetArmComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::TargetArm]);
	}
	else if (component->GetComponentTypeName() == "TileMapComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::TileMap]);
		CTileMapWidgetList* tileMapWidget = 
			(CTileMapWidgetList*)m_vecComponentWidgetList[(int)ESceneComponentType::TileMap];
		tileMapWidget->SetTileMapComponent((CTileMapComponent*)component);
	}
}

void CDetailWindow::LoadButtonClick()
{
}

void CDetailWindow::CreateEditorWidgetList(ESceneComponentType type)
{
	CComponentWidgetList* widgetList = nullptr;
	switch (type)
	{
	case ESceneComponentType::Scene:
		widgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Primitive:
		widgetList = CreateWidgetEmpty<CPrimitiveWidgetList>("PrimitiveComponent");
		break;
	case ESceneComponentType::Sprite:
		widgetList = CreateWidgetEmpty<CSpriteComponentWidgetList>("SpriteComponent");
		break;
	case ESceneComponentType::Camera:
		widgetList = CreateWidgetEmpty<CCameraWidgetList>("CameraComponent");
		break;
	case ESceneComponentType::TargetArm:
		widgetList = CreateWidgetEmpty<CTargetArmWidgetList>("TargetArmComponent");
		break;
	case ESceneComponentType::Collider:
		//widgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Collider2D:
		//widgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Box2D:
		//widgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::OBB2D:
		//widgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Sphere2D:
		//widgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Pixel:
		//widgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Collider3D:
		//widgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::TileMap:
		widgetList = CreateWidgetEmpty<CTileMapWidgetList>("TileMapComponent");
		break;
	}
	if (!widgetList)
	{
		return;
	}
	widgetList->m_DetailWindow = this;
	m_vecComponentWidgetList[(int)type] = widgetList;
}