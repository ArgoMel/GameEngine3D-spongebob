#include "MainSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster3D.h"
#include "../GameObject/Weapon3D.h"
#include "Component/TerrainComponent.h"
#include "Component/BillboardComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/DecalComponent.h"
#include "Component/LightComponent.h"

CMainSceneInfo::CMainSceneInfo()
{
}

CMainSceneInfo::~CMainSceneInfo()
{
}

bool CMainSceneInfo::Init()
{
	CSceneInfo::Init();
	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(45.f, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CPlayer* player = m_Owner->CreateObject<CPlayer>("Player");
	player->SetWorldPosition(50.f, 0.f, 50.f);
	SetPlayerObject(player);
	//CMonster3D* monster = m_Owner->CreateObject<CMonster3D>("Monster");

	CGameObject* terrainObj = m_Owner->CreateObject<CGameObject>("Terrain");
	CTerrainComponent* terrain = terrainObj->CreateComponent<CTerrainComponent>("Terrain");

	//CGameObject* decalObj = m_Owner->CreateObject<CGameObject>("Decal");
	//CDecalComponent* decal = decalObj->CreateComponent<CDecalComponent>("Decal");
	//decal->SetWorldScale(300.f, 300.f, 300.f);
	//decal->SetWorldPosition(200.f, 0.f, 200.f);
	//decal->SetFadeState(EDecalFadeState::FadeInOut);
	//decal->SetFadeInTime(5.f);
	//decal->SetFadeOutTime(5.f);
	//decal->SetFadeOutDelayTime(5.f);

	//CGameObject* billboardObj = m_Owner->CreateObject<CGameObject>("Billboard");
	//CBillboardComponent* billboard = billboardObj->CreateComponent<CBillboardComponent>("Billboard");
	//billboard->SetWorldPosition(300.f, 0.f, 500.f);
	return true;
}
