#include "ClassWindow.h"
#include "ObjectWindow.h"
#include "ComponentWindow.h"
#include "PathManager.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorGUIManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../GameObject\Player.h"
#include "../GameObject\Bullet.h"
#include "../GameObject\Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/TileMapComponent.h"
#include "Component/ColliderBox2d.h"
#include "Component/ColliderOBB2d.h"
#include "Component/ColliderSphere2d.h"
#include "Component/ColliderPixel.h"
#include "Component/RigidBody.h"
#include "Component/LightComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/BillboardComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/DecalComponent.h"
#include "Component/NavigationAgent.h"
#include "Component/NavigationAgent3D.h"

CClassWindow::CClassWindow()
	: m_ComponentList(nullptr)
	, m_ObjectList(nullptr)
	, m_ComponentSelectName(nullptr)
	, m_ObjectSelectName(nullptr)
{
}

CClassWindow::~CClassWindow()
{
}

bool CClassWindow::Init()
{
	CEditorLabel* Label = CreateWidget<CEditorLabel>("ObjectClass");
	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSizeY(30.f);

	CEditorSameLine* Line = CreateWidget<CEditorSameLine>("Line");

	m_ObjectSelectName = CreateWidget<CEditorInputText>("ObjectName");
	m_ObjectSelectName->SetHideName("ObjectName");
	m_ObjectSelectName->SetSize(150.f, 30.f);

	m_ObjectList = CreateWidget<CEditorListBox>("ObjectListBox");
	m_ObjectList->SetHideName("ObjectListBox");
	m_ObjectList->SetSize(200.f, 300.f);
	m_ObjectList->SetPageItemCount(6);
	m_ObjectList->SetSelectCallback<CClassWindow>(this, &CClassWindow::ObjectClickCallback);

	Line = CreateWidget<CEditorSameLine>("Line");

	CEditorButton* Button = CreateWidget<CEditorButton>("오브젝트검색", 100.f, 30.f);
	Button->SetClickCallback<CClassWindow>(this, &CClassWindow::ObjectSearch);

	Line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("pos");
	pos->SetPos(-108.f, 40.f);

	Button = CreateWidget<CEditorButton>("오브젝트생성", 100.f, 30.f);
	Button->SetClickCallback<CClassWindow>(this, &CClassWindow::ObjectCreateCallback);

	Label = CreateWidget<CEditorLabel>("ComponentClass");
	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSizeY(30.f);
	Label->SetSizeX(140.f);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_ComponentSelectName = CreateWidget<CEditorInputText>("ComponentName");
	m_ComponentSelectName->SetHideName("ComponentName");
	m_ComponentSelectName->SetSize(150.f, 30.f);

	m_ComponentList = CreateWidget<CEditorListBox>("ComponentListBox");
	m_ComponentList->SetHideName("ComponentListBox");
	m_ComponentList->SetSize(200.f, 300.f);
	m_ComponentList->SetPageItemCount(6);
	m_ComponentList->SetSelectCallback<CClassWindow>(this, &CClassWindow::ComponentClickCallback);

	Line = CreateWidget<CEditorSameLine>("Line");

	Button = CreateWidget<CEditorButton>("컴포넌트검색", 100.f, 30.f);
	Button->SetClickCallback<CClassWindow>(this, &CClassWindow::ComponentSearch);

	Line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("pos");
	pos->SetPos(-108.f, 40.f);

	Button = CreateWidget<CEditorButton>("컴포넌트생성", 100.f, 30.f);
	Button->SetClickCallback<CClassWindow>(this, &CClassWindow::ComponentCreateCallback);

	LoadGameObjectName();
	LoadComponentName();
	return true;
}

void CClassWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CClassWindow::ObjectClickCallback(int index, const std::string& item)
{
	m_SelectObjectItem = item;
	m_ObjectSelectName->SetText(item.c_str());
}

void CClassWindow::ComponentClickCallback(int index, const std::string& item)
{
	m_SelectComponentItem = item;
	m_ComponentSelectName->SetText(item.c_str());
}

void CClassWindow::ObjectCreateCallback()
{
	if (m_SelectObjectItem == "")
	{
		return;
	}
	CGameObject* obj = nullptr;
	CObjectWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
	if (window)
	{
		window->CreateObject(m_SelectObjectItem);
	}
}

void CClassWindow::ComponentCreateCallback()
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CObjectWindow* objectWindow = 
		CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
	if(!objectWindow)
	{
		return;
	}
	CGameObject* selectObject = objectWindow->GetSelectObject();
	if (!selectObject)
	{
		return;
	}
	CComponentWindow* componentWindow = 
		CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");
	if(!componentWindow)
	{
		return;
	}
	CSceneComponent* selectComponent = (CSceneComponent*)componentWindow->GetSelectComponent();
	if (!selectComponent)
	{
		selectComponent = selectObject->GetRootComponent();
	}
	std::string	name;
	CSceneComponent* newComponent = nullptr;
	CObjectComponent* objectComponent = nullptr;
	if (m_SelectComponentItem == "SpriteComponent")
	{
		name = "SpriteComponent(SpriteComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CSpriteComponent>("SpriteComponent");
	}
	else if (m_SelectComponentItem == "TileMapComponent")
	{
		name = "TileMapComponent(TileMapComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CTileMapComponent>("TileMapComponent");
	}
	else if (m_SelectComponentItem == "AnimationMeshComponent")
	{
		name = "AnimationMeshComponent(AnimationMeshComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CAnimationMeshComponent>("AnimationMeshComponent");
	}
	else if (m_SelectComponentItem == "StaticMeshComponent")
	{
		name = "StaticMeshComponent(StaticMeshComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CStaticMeshComponent>("StaticMeshComponent");
	}
	else if (m_SelectComponentItem == "TerrainComponent")
	{
		name = "TerrainComponent(TerrainComponent)";
		CTerrainComponent* terrain = selectObject->CreateComponent<CTerrainComponent>("TerrainComponent");
		terrain->CreateTerrain(680, 631, 40.f, 40.f, TEXT("LandScape/BikiniCity_Height.png"));
		newComponent = (CSceneComponent*)terrain;
	}
	else if (m_SelectComponentItem == "ParticleComponent")
	{
		name = "ParticleComponent(ParticleComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CParticleComponent>("ParticleComponent");
	}
	else if (m_SelectComponentItem == "DecalComponent")
	{
		name = "DecalComponent(DecalComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CDecalComponent>("DecalComponent");
	}
	else if (m_SelectComponentItem == "BillboardComponent")
	{
		name = "BillboardComponent(BillboardComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CBillboardComponent>("BillboardComponent");
	}
	else if (m_SelectComponentItem == "CameraComponent")
	{
		name = "CameraComponent(CameraComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CCameraComponent>("CameraComponent");
	}
	else if (m_SelectComponentItem == "TargetArm")
	{
		name = "TargetArm(TargetArm)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CTargetArm>("TargetArm");
	}
	else if (m_SelectComponentItem == "ColliderBox2D")
	{
		name = "ColliderBox2D(ColliderBox2D)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CColliderBox2D>("ColliderBox2D");
	}
	else if (m_SelectComponentItem == "ColliderOBB2D")
	{
		name = "ColliderOBB2D(ColliderOBB2D)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CColliderOBB2D>("ColliderOBB2D");
	}
	else if (m_SelectComponentItem == "ColliderSphere2D")
	{
		name = "ColliderSphere2D(ColliderSphere2D)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CColliderSphere2D>("ColliderSphere2D");
	}
	else if (m_SelectComponentItem == "ColliderPixel")
	{
		name = "ColliderPixel(ColliderPixel)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CColliderPixel>("ColliderPixel");
	}
	else if (m_SelectComponentItem == "RigidBody")
	{
		name = "RigidBody(RigidBody)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CRigidBody>("RigidBody");
	}
	else if (m_SelectComponentItem == "LightComponent")
	{
		name = "LightComponent(LightComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CLightComponent>("LightComponent");
	}
	else if (m_SelectComponentItem == "SceneComponent")
	{
		name = "SceneComponent(SceneComponent)";
		newComponent = selectObject->CreateComponent<CSceneComponent>("SceneComponent");
	}
	else if (m_SelectComponentItem == "NavigationAgent")
	{
		name = "NavigationAgent(NavigationAgent)";
		objectComponent = (CNavigationAgent*)selectObject->CreateComponent<CNavigationAgent>("NavigationAgent");
	}
	else if (m_SelectComponentItem == "NavigationAgent3D")
	{
		name = "NavigationAgent3D(NavigationAgent3D)";
		objectComponent = (CNavigationAgent3D*)selectObject->CreateComponent<CNavigationAgent3D>("NavigationAgent3D");
	}
	if (objectComponent)
	{
		std::string	objName = selectObject->GetName() + "(" + selectObject->GetObjectTypeName() + ")";
		componentWindow->AddItem((CComponent*)objectComponent, name, objName);
	}
	else if (selectComponent&& newComponent)
	{
		std::string	parentName = selectComponent->GetName() + "(" + selectComponent->GetComponentTypeName() + ")";
		selectComponent->AddChild(newComponent);
		componentWindow->AddItem((CComponent*)newComponent, name, parentName);
	}
	else
	{
		std::string	objName = selectObject->GetName() + "(" + selectObject->GetObjectTypeName() + ")";
		componentWindow->AddItem((CComponent*)newComponent, name, objName);
	}
}

void CClassWindow::ObjectSearch()
{
	std::string name(m_ObjectSelectName->GetText());
	if (name == "")
	{
		m_ObjectList->Clear();
		LoadGameObjectName();
		return;
	}
	size_t size = m_ObjectList->GetSize();
	std::vector<std::string> vecName;
	std::string checkedName;
	for (size_t i = 0; i < size; ++i)
	{
		checkedName = m_ObjectList->CheckPartitionItem(i, name);
		if (checkedName != "")
		{
			vecName.push_back(checkedName);
		}
	}
	size_t vecSize = vecName.size();
	if (vecSize != 0)
	{
		m_ObjectList->Clear();
		for (size_t i = 0; i < vecSize; ++i)
		{
			m_ObjectList->AddItem(vecName[i]);
		}
	}
}

void CClassWindow::ComponentSearch()
{
	std::string name(m_ComponentSelectName->GetText());
	if (name == "")
	{
		m_ComponentList->Clear();
		LoadComponentName();
		return;
	}
	size_t size = m_ComponentList->GetSize();
	std::vector<std::string> vecName;
	std::string checkedName;
	for (size_t i = 0; i < size; ++i)
	{
		checkedName = m_ComponentList->CheckPartitionItem(i, name);
		if (checkedName != "")
		{
			vecName.push_back(checkedName);
		}
	}
	size_t vecSize = vecName.size();
	if (vecSize != 0)
	{
		m_ComponentList->Clear();
		for (size_t i = 0; i < vecSize; ++i)
		{
			m_ComponentList->AddItem(vecName[i]);
		}
	}
}

void CClassWindow::LoadGameObjectName()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	int	length = (int)strlen(path)-3;
	for (size_t i = length; i > 0; --i)		// Root Path에서 Bin\을 지워준다.
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i-1));
			break;
		}
	}
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	strcat_s(directory, "Include/GameObject/");
	//for (const auto& file : std::filesystem::directory_iterator(directory))
	for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (strcmp(ext, ".cpp") == 0)
		{
			continue;
		}
		m_ObjectList->AddItem(name);
	}
	length = (int)strlen(path) - 3;
	for (size_t i = length; i > 0; --i)	
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
			break;
		}
	}
	memset(directory, 0, MAX_PATH);
	strcpy_s(directory, path);
	strcat_s(directory, "Engine/Include/GameObject/");
	for (const auto& file : std::filesystem::directory_iterator(directory))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (strcmp(ext, ".cpp") == 0)
		{
			continue;
		}
		m_ObjectList->AddItem(name);
	}
}

void CClassWindow::LoadComponentName()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	int	length = (int)strlen(path)-3;
	for (size_t i = length; i > 0; --i)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
			break;
		}
	}
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	strcat_s(directory, "Include/Component/");
	for (const auto& file : std::filesystem::directory_iterator(directory))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (strcmp(ext, ".cpp") == 0)
		{
			continue;
		}
		m_ComponentList->AddItem(name);
	}
	length = (int)strlen(path) - 3;
	for (size_t i = length; i > 0; --i)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
			break;
		}
	}
	memset(directory, 0, MAX_PATH);
	strcpy_s(directory, path);
	strcat_s(directory, "Engine/Include/Component/");
	for (const auto& file : std::filesystem::directory_iterator(directory))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (strcmp(ext, ".cpp") == 0)
		{
			continue;
		}
		else if (strcmp(name, "Transform") == 0 ||
				 strcmp(name, "Transform2D") == 0 ||
				 strcmp(name, "Collider") == 0 ||
				 strcmp(name, "Collider2D") == 0 ||
				 strcmp(name, "Collider3D") == 0 ||
				 strcmp(name, "Component") == 0 ||
				 strcmp(name, "PrimitiveComponent") == 0 ||
				 strcmp(name, "ObjectComponent") == 0||
				 strcmp(name, "Tile")==0)
		{
			continue;
		}
		m_ComponentList->AddItem(name);
	}
}
