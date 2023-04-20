#include "Scene.h"
#include "SceneManager.h"
#include "NavigationManager2D.h"
#include "NavigationManager3D.h"
#include "../Input.h"
#include "../PathManager.h"
#include "../Engine.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/SkySphere.h"
#include "../Component/SpriteComponent.h"
#include "../Component/CameraComponent.h"
#include "../Component/TargetArm.h"
#include "../Component/SceneComponent.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderCube.h"
#include "../Component/ColliderOBB3D.h"
#include "../Component/TileMapComponent.h"
#include "../Component/TerrainComponent.h"
#include "../Component/LightComponent.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Component/ParticleComponent.h"
#include "../Component/NavigationAgent.h"
#include "../Animation/Animation2D.h"
#include "../UI/UIButton.h"
#include "../UI/UIImage.h"
#include "../UI/UIWindow.h"
std::unordered_map<std::string, CSceneInfo*> CScene::m_mapSceneInfoCDO;

CScene::CScene()
	: m_Change(false)
	, m_Start(false)
{
	m_Name = "Scene";
	m_SceneInfo = new CSceneInfo;
	m_SceneInfo->m_Owner = this;
	m_SceneInfo->Init();
	m_Resource = new CSceneResource;
	m_Resource->m_Owner = this;
	m_Resource->Init();
	m_CameraManager = new CCameraManager;
	m_CameraManager->m_Owner = this;
	m_CameraManager->Init();
	m_CollisionManager = new CSceneCollision;
	m_CollisionManager->m_Owner = this;
	m_CollisionManager->Init();
	m_Viewport = new CSceneViewport;
	m_Viewport->m_Owner = this;
	m_Viewport->Init();
	m_LightManager = new CLightManager;
	m_LightManager->m_Owner = this;
	m_LightManager->Init();
	if (CEngine::GetInst()->GetRender2D())
	{
		m_NavManager = new CNavigationManager2D;
	}
	else
	{
		m_NavManager = new CNavigationManager3D;
	}
	m_NavManager->m_Owner = this;
	m_NavManager->Init();
	m_CutScene = new CCutScene;
}

CScene::~CScene()
{
	CInput::GetInst()->ClearCallback(this);
	SAFE_DELETE(m_CutScene);
	SAFE_DELETE(m_LightManager);
	SAFE_DELETE(m_NavManager);
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_CollisionManager);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_Resource);
	SAFE_DELETE(m_SceneInfo);
}

bool CScene::SortObject(CGameObject* src, CGameObject* dest)
{
	Vector3	srcCenter = src->GetCenter();
	Vector3	destCenter = dest->GetCenter();
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
	float	srcDist = camera->GetWorldPos().Distance(srcCenter) - src->GetObjectRadius();
	float	destDist = camera->GetWorldPos().Distance(destCenter) - dest->GetObjectRadius();
	return srcDist < destDist;
}

void CScene::CreateCDO()
{
// ==================== SceneInfo ====================
	CSceneInfo* info = new CSceneInfo;
	info->Init();
	CScene::AddSceneInfoCDO("SceneInfo", info);
// ==================== GameObject ====================
	CGameObject* objCDO = new CGameObject;
	objCDO->Init();
	CGameObject::AddObjectCDO("GameObject", objCDO);
// ==================== Component ====================
	CComponent* comCDO = new CSceneComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("SceneComponent", comCDO);
	comCDO = new CSpriteComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("SpriteComponent", comCDO);
	comCDO = new CCameraComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("CameraComponent", comCDO);
	comCDO = new CTargetArm;
	comCDO->Init();
	CComponent::AddComponentCDO("TargetArm", comCDO);
	comCDO = new CColliderBox2D;
	comCDO->Init();
	CComponent::AddComponentCDO("Box2D", comCDO);
	comCDO = new CColliderCube;
	comCDO->Init();
	CComponent::AddComponentCDO("ColliderCube", comCDO);
	comCDO = new CColliderOBB3D;
	comCDO->Init();
	CComponent::AddComponentCDO("ColliderOBB3D", comCDO);
	comCDO = new CTileMapComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("TileMapComponent", comCDO);
	comCDO = new CNavigationAgent;
	comCDO->Init();
	CComponent::AddComponentCDO("NavigationAgent", comCDO);
	comCDO = new CTerrainComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("TerrainComponent", comCDO);
	comCDO = new CLightComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("LightComponent", comCDO);
	comCDO = new CStaticMeshComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("StaticMeshComponent", comCDO);
	comCDO = new CAnimationMeshComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("AnimationMeshComponent", comCDO);
	comCDO = new CParticleComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("ParticleComponent", comCDO);
// ==================== Animation ====================
	CAnimation2D* animCDO = new CAnimation2D;
	animCDO->Init();
	CAnimation2D::AddAnimationCDO("Animation2D", animCDO);
// ==================== UI ====================
	CUIWindow* UIWindowCDO = new CUIWindow;
	UIWindowCDO->Init();
	CUIWindow::AddUIWindowCDO("UIWindow", UIWindowCDO);
	CUIWidget* UIWidgetCDO = new CUIButton;
	UIWidgetCDO->Init();
	CUIWidget::AddUIWidgetCDO("UIButton", UIWidgetCDO);
	UIWidgetCDO = new CUIImage;
	UIWidgetCDO->Init();
	CUIWidget::AddUIWidgetCDO("UIImage", UIWidgetCDO);
}

void CScene::SetSkyTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_SkySphere->SetSkyTexture(name, fileName, pathName);
}

void CScene::ClearSky()
{
	m_SkySphere = nullptr;
}

void CScene::Start()
{
	m_Start = true;
	m_SkySphere = new CSkySphere;
	m_SkySphere->SetName("Sky");
	m_SkySphere->SetScene(this);
	m_SkySphere->Init();
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
	m_CameraManager->Start();
	m_Viewport->Start();
	m_LightManager->Start();
}

bool CScene::Init()
{
	return true;
}

void CScene::Update(float deltaTime)
{
	if (m_SceneInfo)
	{
		m_SceneInfo->Update(deltaTime);
	}
	if (m_SkySphere)
	{
		m_SkySphere->Update(deltaTime);
	}
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(deltaTime);
		++iter;
	}
	m_CameraManager->Update(deltaTime);
	m_Viewport->Update(deltaTime);
}

void CScene::PostUpdate(float deltaTime)
{
	if (m_SceneInfo)
	{
		m_SceneInfo->PostUpdate(deltaTime);
	}
	if (m_SkySphere)
	{
		m_SkySphere->PostUpdate(deltaTime);
	}
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->PostUpdate(deltaTime);
		++iter;
	}
	m_CameraManager->PostUpdate(deltaTime);
	m_Viewport->PostUpdate(deltaTime);
	m_LightManager->Update(deltaTime);
	CCameraComponent* camera = m_CameraManager->GetCurrentCamera();
	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->FrustumCull(camera);
	}
}

void CScene::Collision(float deltaTime)
{
	m_CollisionManager->Update(deltaTime);
}

void CScene::Save(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return;
	}
	int	length = (int)m_Name.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_Name.c_str(), 1, length, file);
	m_SceneInfo->Save(file);
	m_CameraManager->Save(file);
	m_CollisionManager->Save(file);
	m_Viewport->Save(file);
	m_LightManager->Save(file);
	int	objCount = (int)m_ObjList.size();
	fwrite(&objCount, 4, 1, file);
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		std::string	classTypeName = (*iter)->GetObjectTypeName();
		length = (int)classTypeName.length();
		fwrite(&length, 4, 1, file);
		fwrite(classTypeName.c_str(), 1, length, file);
		(*iter)->Save(file);
	}
	fclose(file);
}

void CScene::Load(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return;
	}
//파일 크기 가져오기
	fseek(file, 0, SEEK_END);
	int	fileSize = (int)ftell(file);
	fseek(file, 0, SEEK_SET);
	int	loadSize = 0;
//불러오기 시작
	int	length = 0;
	char	name[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(name, 1, length, file);
	loadSize += 4 + length;
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_Name = name;
	length = 0;
	char	sceneInfoName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(sceneInfoName, 1, length, file);
	loadSize += 4 + length;
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	SAFE_DELETE(m_SceneInfo);
	CSceneInfo* CDO = FindSceneInfoCDO(sceneInfoName);
	if (!CDO)
	{
		CDO = FindSceneInfoCDO("SceneInfo");
	}
	m_SceneInfo = CDO->Clone();
	m_SceneInfo->m_Owner = this;

	m_SceneInfo->Load(file);
	int	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_CameraManager->m_Owner = this;
	m_CameraManager->Load(file);
	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_CollisionManager->m_Owner = this;
	m_CollisionManager->Load(file);
	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_Viewport->m_Owner = this;
	m_Viewport->Load(file);
	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_LightManager->m_Owner = this;
	m_LightManager->Load(file);
	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	int	objCount = 0;
	fread(&objCount, 4, 1, file);
	loadSize += 4;
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	for (int i = 0; i < objCount; ++i)
	{
		length = 0;
		char	objClassTypeName[256] = {};
		fread(&length, 4, 1, file);
		assert(length >= 0);
		fread(objClassTypeName, 1, length, file);
		loadSize += 4 + length;
		if (m_LoadingCallback)
		{
			m_LoadingCallback(loadSize / (float)fileSize);
		}

		CGameObject* objCDO = CGameObject::FindCDO(objClassTypeName);
		CGameObject* newObj = objCDO->Clone();
		newObj->SetScene(this);
		newObj->Load(file);
		if (newObj->GetName() == "GlobalLight")
		{
			m_LightManager->SetGlobalLightObject(newObj);
		}
		nextPos = (int)ftell(file);
		loadSize += (nextPos - loadSize);
		{
			if (m_LoadingCallback)
			{
				m_LoadingCallback(loadSize / (float)fileSize);
			}
		}

		m_ObjList.push_back(newObj);
	}
	m_SceneInfo->LoadComplete();
	fclose(file);
}

void CScene::Save(const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	char	fullPath[MAX_PATH] = {};
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	Save(fullPath);
}

void CScene::Load(const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	char	fullPath[MAX_PATH] = {};
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	Load(fullPath);
}

void CScene::GetAllGameObjectHierarchyName(std::vector<HierarchyObjectName>& vecName)
{
	auto    iter = m_ObjList.begin();
	auto    iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		HierarchyObjectName	names;
		names.name = (*iter)->GetName();
		names.className = (*iter)->GetObjectTypeName();
		names.obj = *iter;
		vecName.push_back(names);
	}
}

void CScene::CloneGameObject(CGameObject* object)
{
	object->SetScene(this);
	m_ObjList.push_back(object);
	object->Start();
}

CGameObject* CScene::FindObject(const std::string& name)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return nullptr;
}

bool CScene::Picking(PickingResult& result)
{
	if (CEngine::GetInst()->GetEditorMode())
	{
		std::list<CSharedPtr<class CGameObject>>	objList;
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();
		for (; iter != iterEnd; ++iter)
		{
			if (!(*iter)->GetActive() || !(*iter)->GetEnable())
			{
				continue;
			}
			else if (!(*iter)->GetFrustumCull())
			{
				continue;
			}
			objList.push_back(*iter);
			//if ((*iter)->Picking(result))
			//	return true;
		}
		objList.sort(CScene::SortObject);
		iter = objList.begin();
		iterEnd = objList.end();
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->Picking(result))
			{
				return true;
			}
		}
		return false;
	}
	return m_CollisionManager->Picking(result);
}
