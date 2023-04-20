#include "ComponentWindow.h"
#include "TransformWindow.h"
#include "DetailWindow.h"
#include "AnimationWindow.h"
#include "Animation3DWindow.h"
#include "ColiderWindow.h"
#include "ColliderPixelWindow.h"
#include "ColliderCubeWindow.h"
#include "ColliderSphere3DWindow.h"
#include "ColliderObb3DWindow.h"
#include "CameraWindow.h"
#include "TargetArmWindow.h"
#include "UIButtonWindow.h"
#include "UIImageWindow.h"
#include "UINumberWindow.h"
#include "UIProgressBarWindow.h"
#include "UITextWindow.h"
#include "StatusWindow.h"
#include "RigidBodyWindow.h"
#include "LightWindow.h"
#include "MaterialWindow.h"
#include "MeshWindow.h"
#include "ParticleWindow.h"
#include "TerrainWindow.h"
#include "OceanWindow.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Component.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2d.h"
#include "Component/ColliderSphere2d.h"
#include "Component/ColliderObb2d.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/RigidBody.h"
#include "Component/LightComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/OceanComponent.h"
#include "Component/DecalComponent.h"
#include "Editor/EditorGUIManager.h"
#include "UI/UIWidget.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UINumber.h"
#include "UI/UIProgressBar.h"
#include "UI/UIText.h"
#include "GameObject/GameObject.h"

CComponentWindow::CComponentWindow()
	: m_Tree(nullptr)
	, m_WidgetTree(nullptr)
{
}

CComponentWindow::~CComponentWindow()
{
}

bool CComponentWindow::AddItem(CComponent* component, const std::string& name, const std::string& parentName)
{
	return m_Tree->AddItem(component, name, parentName);
}

bool CComponentWindow::AddWidget(CUIWidget* widget, const std::string& name, const std::string& parentName)
{
	return m_WidgetTree->AddItem(widget, name, parentName);
}

void CComponentWindow::AddInput(CScene* scene)
{	
	//CInput::GetInst()->AddBindFunction<CComponentWindow>
	//	("Del2", Input_Type::Down, this, &CComponentWindow::Delete, scene);
	CInput::GetInst()->AddBindFunction<CComponentWindow>
		("F4", Input_Type::Down, this, &CComponentWindow::ChangePos, scene);
}

void CComponentWindow::Clear()
{
	m_Tree->Clear();
	m_WidgetTree->Clear();
}

void CComponentWindow::ClearSelect()
{
	m_SelectComponent = nullptr;
	m_SelectWidget = nullptr;
}

bool CComponentWindow::Init()
{
	m_Tree = CreateWidget<CEditorTree<CComponent*>>("ComponentWindow");
	m_Tree->SetHideName("ComponentWindow");
	m_Tree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::TreeCallback);
	m_Tree->SetDoubleClickCallback<CComponentWindow>(this, &CComponentWindow::TreeDCCallback);
	m_Tree->SetSize(400.f, 300.f);
	m_WidgetTree = CreateWidget<CEditorTree<CUIWidget*>>("WidgetTree");
	m_WidgetTree->SetHideName("WidgetTree");
	m_WidgetTree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::WidgetCallback);
	m_WidgetTree->SetDoubleClickCallback<CComponentWindow>(this, &CComponentWindow::WidgetDCCallback);
	m_WidgetTree->SetSize(400.f, 300.f);

	AddInput(CSceneManager::GetInst()->GetScene());
	return true;
}

void CComponentWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
		}
	}
	if (m_SelectWidget)
	{
		if (!m_SelectWidget->GetActive())
		{
			m_SelectWidget = nullptr;
		}
	}
}

void CComponentWindow::Delete()
{
	if(!m_SelectComponent && !m_SelectWidget)
	{
		return;
	}
	if(m_SelectComponent)
	{
		m_SelectComponent->Destroy();
		m_Tree->DeleteItem(m_SelectComponent->GetName()+"("+ m_SelectComponent->GetComponentTypeName() + ")");
	}
	else if (m_SelectWidget)
	{
		m_SelectWidget->Destroy();
		m_WidgetTree->DeleteItem(m_SelectWidget->GetName() + "(" + m_SelectWidget->GetWidgetTypeName() + ")");
	}
	ClearSelect();
}

void CComponentWindow::ChangePos()
{
	if (!m_SelectComponent && !m_SelectWidget)
	{
		return;
	}
	if (m_SelectComponent)
	{
		CSceneComponent* component = (CSceneComponent*)m_SelectComponent.Get();
		CGameObject* gizmo = CSceneManager::GetInst()->GetScene()->FindObject("Gizmo");
		if(gizmo)
		{
			component->SetWorldPosition(gizmo->GetWorldPos());
		}
	}
	else if (m_SelectWidget)
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseUIPos();
		m_SelectWidget->SetPos(mousePos);
	}
}

void CComponentWindow::TreeCallback(CEditorTreeItem<class CComponent*>* node, const std::string& item)
{
	m_SelectComponent = node->GetCustomData();
	if (!m_SelectComponent)
	{
		return;
	}
	CDetailWindow* detailWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CDetailWindow>("DetailWindow");
	if (detailWindow)
	{
		detailWindow->SetSelectComponent((CSceneComponent*)m_SelectComponent.Get());
	}
	CStatusWindow* statusWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CStatusWindow>("StatusWindow");
	if (statusWindow)
	{
		statusWindow->SetSelectComponent((CSceneComponent*)m_SelectComponent.Get());
	}
	CColiderWindow* colliderWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CColiderWindow>("ColliderWindow");
	CColliderPixelWindow* colliderPixelWindow = CEditorGUIManager::GetInst()->
		FindEditorWindow<CColliderPixelWindow>("ColliderPixelWindow");
	CMeshWindow* meshWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMeshWindow>("MeshWindow");
	CMaterialWindow* materialWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMaterialWindow>("MaterialWindow");
	if (m_SelectComponent->GetComponentTypeName() == "SpriteComponent")
	{
		CAnimationWindow* animationWindow = CEditorGUIManager::GetInst()->
			FindEditorWindow<CAnimationWindow>("AnimationWindow");
		if (animationWindow)
		{
			animationWindow->SetSelectComponent((CSpriteComponent*)m_SelectComponent.Get());
		}
		if (colliderWindow)
		{
			colliderWindow->SetSelectSprite((CSpriteComponent*)m_SelectComponent.Get());
		}
		if (colliderPixelWindow)
		{

			colliderPixelWindow->SetSelectSprite((CSpriteComponent*)m_SelectComponent.Get());
		}
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "AnimationMeshComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		CAnimation3DWindow* animation3DWindow = 
			CEditorGUIManager::GetInst()->FindEditorWindow<CAnimation3DWindow>("Animation3DWindow");
		if(animation3DWindow)
		{
			animation3DWindow->SetSelectComponent((CAnimationMeshComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "StaticMeshComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "TerrainComponent") 
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		CTerrainWindow* terrainWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CTerrainWindow>("TerrainWindow");
		if (terrainWindow)
		{
			terrainWindow->SetSelectComponent((CTerrainComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "OceanComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		COceanWindow* oceanWindow = CEditorGUIManager::GetInst()->FindEditorWindow<COceanWindow>("OceanWindow");
		if (oceanWindow)
		{
			oceanWindow->SetSelectComponent((COceanComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ParticleComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		CParticleWindow* particleWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CParticleWindow>("ParticleWindow");
		if (particleWindow)
		{
			particleWindow->SetSelectComponent((CParticleComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "DecalComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderBox2D"&& colliderWindow)
	{
		colliderWindow->SetSelectComponent((CColliderBox2D*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderSphere2D" && colliderWindow)
	{
		colliderWindow->SetSelectComponent((CColliderSphere2D*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderOBB2D" && colliderWindow)
	{
		colliderWindow->SetSelectComponent((CColliderOBB2D*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderPixel"&& colliderPixelWindow)
	{
		colliderPixelWindow->SetSelectComponent((CColliderPixel*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "CameraComponent")
	{
		CCameraWindow* cameraWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CCameraWindow>("CameraWindow");
		if(cameraWindow)
		{
			cameraWindow->SetSelectComponent((CCameraComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "TargetArm")
	{
		CTargetArmWindow* targetArmWindow = 
			CEditorGUIManager::GetInst()->FindEditorWindow<CTargetArmWindow>("TargetArmWindow");
		if (targetArmWindow)
		{
			targetArmWindow->SetSelectComponent((CTargetArm*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "RigidBody")
	{
		CRigidBodyWindow* rigidBody = CEditorGUIManager::GetInst()->FindEditorWindow<CRigidBodyWindow>("RigidBodyWindow");
		if(rigidBody)
		{
			rigidBody->SetSelectComponent((CRigidBody*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "LightComponent")
	{
		CLightWindow* light = CEditorGUIManager::GetInst()->FindEditorWindow<CLightWindow>("LightWindow");
		if (light)
		{
			light->SetSelectComponent((CLightComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderCube")
	{
		CColliderCubeWindow* colliderCubeWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CColliderCubeWindow>("ColliderCubeWindow");
		if (colliderCubeWindow)
		{
			colliderCubeWindow->SetSelectComponent((CCollider*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderSphere3D")
	{
		CColliderSphere3dWindow* colliderSphere3dWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CColliderSphere3dWindow>("ColliderSphere3dWindow");
		if (colliderSphere3dWindow)
		{
			colliderSphere3dWindow->SetSelectComponent((CCollider*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderOBB3D")
	{
		CColliderObb3dWindow* colliderObb3dWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CColliderObb3dWindow>("ColliderObb3dWindow");
		if (colliderObb3dWindow)
		{
			colliderObb3dWindow->SetSelectComponent((CCollider*)m_SelectComponent.Get());
		}
	}
	CTransformWindow* transformWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CTransformWindow>("TransformWindow");
	if(transformWindow)
	{
		CSceneComponent* component = (CSceneComponent*)m_SelectComponent.Get();
		transformWindow->SetSelectComponent(component);
		if (component->GetParent())
		{
			transformWindow->SetPos(component->GetRelativePos());
			transformWindow->SetScale(component->GetRelativeScale());
			transformWindow->SetRotation(component->GetRelativeRot());
		}
		else
		{
			transformWindow->SetPos(component->GetWorldPos());
			transformWindow->SetScale(component->GetWorldScale());
			transformWindow->SetRotation(component->GetWorldRot());
		}
	}
}

void CComponentWindow::TreeDCCallback(CEditorTreeItem<class CComponent*>* node, const std::string& item)
{
	m_SelectComponent = node->GetCustomData();
	if (!m_SelectComponent)
	{
		return;
	}
	CMeshWindow* meshWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMeshWindow>("MeshWindow");
	CMaterialWindow* materialWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMaterialWindow>("MaterialWindow");
	if (m_SelectComponent->GetComponentTypeName() == "SpriteComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "AnimationMeshComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		CAnimation3DWindow* animation3DWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CAnimation3DWindow>("Animation3DWindow");
		if (!animation3DWindow)
		{
			animation3DWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CAnimation3DWindow>("Animation3DWindow");
		}
		animation3DWindow->Open();
		animation3DWindow->SetSelectComponent((CAnimationMeshComponent*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "StaticMeshComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "TerrainComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		CTerrainWindow* terrainWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CTerrainWindow>("TerrainWindow");
		if (!terrainWindow)
		{
			terrainWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CTerrainWindow>("TerrainWindow");
		}
		terrainWindow->Open();
		terrainWindow->SetSelectComponent((CTerrainComponent*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "OceanComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		COceanWindow* oceanWindow = CEditorGUIManager::GetInst()->FindEditorWindow<COceanWindow>("OceanWindow");
		if (!oceanWindow)
		{
			oceanWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<COceanWindow>("OceanWindow");
		}
		oceanWindow->Open();
		oceanWindow->SetSelectComponent((COceanComponent*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ParticleComponent")
	{
		if (materialWindow)
		{
			materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		if (meshWindow)
		{
			meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
		}
		CParticleWindow* particleWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CParticleWindow>("ParticleWindow");
		if (!particleWindow)
		{
			particleWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CParticleWindow>("ParticleWindow");
		}
		particleWindow->Open();
		particleWindow->SetSelectComponent((CParticleComponent*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "CameraComponent")
	{
		CCameraWindow* cameraWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CCameraWindow>("CameraWindow");
		if (!cameraWindow)
		{
			cameraWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CCameraWindow>("CameraWindow");
		}
		cameraWindow->Open();
		cameraWindow->SetSelectComponent((CCameraComponent*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "TargetArm")
	{
		CTargetArmWindow* targetArmWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CTargetArmWindow>("TargetArmWindow");
		if (!targetArmWindow)
		{
			targetArmWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CTargetArmWindow>("TargetArmWindow");
		}
		targetArmWindow->Open();
		targetArmWindow->SetSelectComponent((CTargetArm*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "RigidBody")
	{
		CRigidBodyWindow* rigidBody = CEditorGUIManager::GetInst()->FindEditorWindow<CRigidBodyWindow>("RigidBodyWindow");
		if (!rigidBody)
		{
			rigidBody = CEditorGUIManager::GetInst()->CreateEditorWindow<CRigidBodyWindow>("RigidBodyWindow");
		}
		rigidBody->Open();
		rigidBody->SetSelectComponent((CRigidBody*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "LightComponent")
	{
		CLightWindow* light = CEditorGUIManager::GetInst()->FindEditorWindow<CLightWindow>("LightWindow");
		if (!light)
		{
			light = CEditorGUIManager::GetInst()->CreateEditorWindow<CLightWindow>("LightWindow");
		}
		light->Open();
		light->SetSelectComponent((CLightComponent*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderCube")
	{
		CColliderCubeWindow* colliderCubeWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CColliderCubeWindow>("ColliderCubeWindow");
		if (!colliderCubeWindow)
		{
			colliderCubeWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CColliderCubeWindow>("ColliderCubeWindow");
		}
		colliderCubeWindow->Open();
		colliderCubeWindow->SetSelectComponent((CCollider*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderSphere3D")
	{
		CColliderSphere3dWindow* colliderSphere3dWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CColliderSphere3dWindow>("ColliderSphere3dWindow");
		if (!colliderSphere3dWindow)
		{
			colliderSphere3dWindow =
				CEditorGUIManager::GetInst()->CreateEditorWindow<CColliderSphere3dWindow>("ColliderSphere3dWindow");
		}
		colliderSphere3dWindow->Open();
		colliderSphere3dWindow->SetSelectComponent((CCollider*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderOBB3D")
	{
		CColliderObb3dWindow* colliderObb3dWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CColliderObb3dWindow>("ColliderObb3dWindow");
		if (!colliderObb3dWindow)
		{
			colliderObb3dWindow =
				CEditorGUIManager::GetInst()->CreateEditorWindow<CColliderObb3dWindow>("ColliderObb3dWindow");
		}
		colliderObb3dWindow->Open();
		colliderObb3dWindow->SetSelectComponent((CCollider*)m_SelectComponent.Get());
	}
}

void CComponentWindow::WidgetCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item)
{
	m_SelectWidget = node->GetCustomData();
	if (!m_SelectWidget)
	{
		return;
	}
	CStatusWindow* statusWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CStatusWindow>("StatusWindow");
	if (statusWindow)
	{
		statusWindow->SetSelectWidget(m_SelectWidget.Get());
	}
	if (m_SelectWidget->GetWidgetTypeName() == "UIButton")
	{
		CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		if (buttonWindow)
		{
			buttonWindow->SetSelectWidget((CUIButton*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIImage")
	{
		CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		if (imgWindow)
		{
			imgWindow->SetSelectWidget((CUIImage*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UINumber")
	{
		CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		if (numberWindow)
		{
			numberWindow->SetSelectWidget((CUINumber*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIProgressBar")
	{
		CUIProgressBarWindow* barWindow = 
			CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		if (barWindow)
		{
			barWindow->SetSelectWidget((CUIProgressBar*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIText")
	{
		CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		if (textWindow)
		{
			textWindow->SetSelectWidget((CUIText*)m_SelectWidget.Get());
		}
	}
}

void CComponentWindow::WidgetDCCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item)
{
	m_SelectWidget = node->GetCustomData();
	if (!m_SelectWidget)
	{
		return;
	}
	if (m_SelectWidget->GetWidgetTypeName() == "UIButton")
	{
		CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		if (!buttonWindow)
		{
			buttonWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUIButtonWindow>("UIButtonWindow");
		}
		buttonWindow->Open();
		buttonWindow->SetSelectWidget((CUIButton*)m_SelectWidget.Get());
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIImage")
	{
		CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		if (!imgWindow)
		{
			imgWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUIImageWindow>("UIImageWindow");
		}
		imgWindow->Open();
		imgWindow->SetSelectWidget((CUIImage*)m_SelectWidget.Get());
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UINumber")
	{
		CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		if (!numberWindow)
		{
			numberWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUINumberWindow>("UINumberWindow");
		}
		numberWindow->Open();
		numberWindow->SetSelectWidget((CUINumber*)m_SelectWidget.Get());
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIProgressBar")
	{
		CUIProgressBarWindow* barWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		if (!barWindow)
		{
			barWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		}
		barWindow->Open();
		barWindow->SetSelectWidget((CUIProgressBar*)m_SelectWidget.Get());
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIText")
	{
		CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		if (!textWindow)
		{
			textWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUITextWindow>("UITextWindow");
		}
		textWindow->Open();
		textWindow->SetSelectWidget((CUIText*)m_SelectWidget.Get());
	}
}
