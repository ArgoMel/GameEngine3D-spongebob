#include "SceneInfo.h"
#include "Scene.h"
#include "../GameObject/GameObject.h"
#include "../Component/LightComponent.h"

CSceneInfo::CSceneInfo()
    : m_Owner(nullptr)
{
    m_ClassTypeName = "SceneInfo";
}

CSceneInfo::CSceneInfo(const CSceneInfo& info)
{
	m_ClassTypeName = info.m_ClassTypeName;
}

CSceneInfo::~CSceneInfo()
{

}

void CSceneInfo::SetPlayerObject(CGameObject* player)
{
	m_PlayerObject = player;
	if (player)
	{
		m_PlayerObjectName = player->GetName();
	}
}

void CSceneInfo::SetBossObject(CGameObject* boss)
{
	m_BossObject = boss;
	if (boss)
	{
		m_BossObjectName = boss->GetName();
	}
}

void CSceneInfo::LoadComplete()
{
	if (!m_PlayerObjectName.empty())
	{
		m_PlayerObject = m_Owner->FindObject(m_PlayerObjectName);
	}
	if (!m_BossObjectName.empty())
	{
		m_BossObject = m_Owner->FindObject(m_BossObjectName);
	}
}

bool CSceneInfo::Init()
{
	if(!m_Owner)
	{
		return true;
	}
	if (m_Owner->GetCutScene())
	{
		m_Owner->GetCutScene()->StartDirectShow(GetCutSceneName());
	}
	if(m_Owner->GetLightManager())
	{
		CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
		CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
		GlobalLightComponent->SetLightType(ELightType::Direction);
		GlobalLightComponent->SetRelativeRotation(0, 90.f, 0.f);
		GlobalLightComponent->SetLightIntensity(5.f);
		m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);
	}
    return true;
}

void CSceneInfo::Update(float deltaTime)
{
}

void CSceneInfo::PostUpdate(float deltaTime)
{
}

CSceneInfo* CSceneInfo::Clone()
{
	return new CSceneInfo(*this);
}

void CSceneInfo::Save(FILE* file)
{
	int	length = (int)m_ClassTypeName.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_ClassTypeName.c_str(), 1, length, file);
	bool	playerEnable = false;
	if (m_PlayerObject)
	{
		playerEnable = true;
	}
	fwrite(&playerEnable, 1, 1, file);
	if (playerEnable)
	{
		length = (int)m_PlayerObject->GetName().length();
		fwrite(&length, 4, 1, file);
		fwrite(m_PlayerObject->GetName().c_str(), 1,length, file);
	}
	bool	bossEnable = false;
	if (m_BossObject)
	{
		bossEnable = true;
	}
	fwrite(&bossEnable, 1, 1, file);
	if (bossEnable)
	{
		length = (int)m_BossObject->GetName().length();
		fwrite(&length, 4, 1, file);
		fwrite(m_BossObject->GetName().c_str(), 1, length, file);
	}
	fwrite(m_CutSceneName, sizeof(TCHAR), MAX_PATH, file);
}

void CSceneInfo::Load(FILE* file)
{
	bool	playerEnable = false;
	fread(&playerEnable, 1, 1, file);
	int length = 0;
	if (playerEnable)
	{
		char	name[256] = {};
		fread(&length, 4, 1, file);
		assert(length >= 0);
		fread(name, 1, length, file);
		m_PlayerObjectName = name;
	}
	bool	bossEnable = false;
	fread(&bossEnable, 1, 1, file);
	if (bossEnable)
	{
		char	name[256] = {};
		fread(&length, 4, 1, file);
		assert(length >= 0);
		fread(name, 1, length, file);
		m_BossObjectName = name;
	}
	fread(m_CutSceneName, sizeof(TCHAR), MAX_PATH, file);
}

void CSceneInfo::SceneChangeComplete()
{
	m_Owner->GetCutScene()->StartDirectShow(m_CutSceneName);
}
