#include "Boss.h"
#include "Player2D.h"
#include "Marker.h"
#include "Device.h"
#include "PathManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/Material/Material.h"
#include "../Scene/LoadingSceneInfo.h"
#include "../Skill/SkillManager.h"
//#include "../UI/BossUI.h"

CBoss::CBoss()
	: m_SkillCount(0)
	, m_dir(-1)
	, m_RandomNum(0)
	, m_LoopIndex(-1)
	, m_SChargeCount(0)
	, m_SMaxSummonCount(0)
	, m_SLoopCount(0)
	, m_SChargeTime(0.f)
	, m_SDownTime(0.f)
	, m_SPatternDelay(0.f)
	, m_DeadTime(-1.f)
{
	SetTypeID<CBoss>();
	m_ObjectTypeName = "Boss";
}

CBoss::CBoss(const CBoss& obj)
	: CGameObject(obj)
	, m_dir(-1)
	, m_SkillCount(obj.m_SkillCount)
	, m_RandomNum(0)
	, m_LoopIndex(-1)
	, m_SChargeCount(0)
	, m_SMaxSummonCount(0)
	, m_SChargeTime(0.f)
	, m_SDownTime(0.f)
	, m_SPatternDelay(0.f)
	, m_DeadTime(-1.f)
{
	m_Body = (CColliderSphere2D*)FindComponent("Body");
	m_Sprite = (CSpriteComponent*)FindComponent(m_BossName);
}

CBoss::~CBoss()
{
	if(m_Scene)
	{
		m_Scene->GetResource()->SoundStop(m_BossName);
		m_Scene->GetResource()->FindChannelGroup("Effect")->stop();
	}
}

void CBoss::Destroy()
{
	CGameObject::Destroy();
	CSceneManager::GetInst()->CreateNextScene();
	if (m_Scene->GetName() == "ChallengeDungeon")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "ChallengeDungeon2.scn");
	}
	else if (m_Scene->GetName() == "ChallengeDungeon2")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "ChallengeDungeon3.scn");
	}
	else if (m_Scene->GetName() == "ChallengeDungeon3")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "DungeonClear.scn");
	}
}

void CBoss::Start()
{
	CGameObject::Start();
	//m_BossUI=m_Scene->GetViewport()->CreateUIWindow<CBossUI>("BossUI");
	m_BossName = m_Sprite->GetName();
	m_Scene->GetResource()->SoundPlay(m_BossName);
	std::string imgPath = "Boss\\" + m_BossName + "\\000a.png";
	TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
	t_filename[imgPath.size()] = 0;
	std::copy(imgPath.begin(), imgPath.end(), t_filename);
	m_Sprite->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, m_BossName, t_filename);
	SAFE_DELETE_ARRAY(t_filename);
	m_Sprite->SetRelativeScale((float)m_Sprite->GetMaterial(0)->GetTexture()->GetWidth() * 2.5f,
		(float)m_Sprite->GetMaterial(0)->GetTexture()->GetHeight() * 2.5f);
//랜덤 범위 설정
	Resolution RS = CDevice::GetInst()->GetResolution();
	m_RangeX[0] = (float)RS.width * 0.4f;
	m_RangeX[1] = (float)RS.width * 0.6f;
	m_RangeY[0] = (float)RS.height * 0.3f;
	m_RangeY[1] = (float)RS.height * 0.6f;
//로드
	//if (!Load(SAVE_PATH))
	//{
	//	return;
	//}
	m_RandomNum = m_NormalPatternCount;
	m_StartPos = GetWorldPos();
//이름 설정
	t_filename = new TCHAR[m_OutputName.size() + 1];
	t_filename[m_OutputName.size()] = 0;
	std::copy(m_OutputName.begin(), m_OutputName.end(), t_filename);
	//m_BossUI->SetText(t_filename);
	SAFE_DELETE_ARRAY(t_filename);
//애니메이션
	m_Anim = m_Sprite->SetAnimation<CAnimation2D>("BossAnim");
	m_Anim->AddAnimation("Die", "BossDie");
	std::string animeName = m_BossName + "_die";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	m_Anim->AddAnimation("Stun", "BossStun", 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Stun");
	animeName = m_BossName + "_stun";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	m_Anim->AddAnimation("Charge", "BossCharge", 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Charge");
	animeName = m_BossName + "_charge";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	for (int i = 0; i < m_SkillCount;++i)
	{
		m_Anim->AddAnimation("Skill" + std::to_string(i), "BossSkill" + std::to_string(i), 1.f, 1.f, m_vecIsLoop[i]);
		m_Anim->SetCurrentAnimation("Skill" + std::to_string(i));
		animeName = m_BossName + "_skill"+ std::to_string(i);
		m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
		if(!m_vecIsLoop[i])
		{
			m_Anim->SetCurrentEndFunction("Skill" + std::to_string(i), this, &CBoss::PatternEnd);
		}
	}
	m_Anim->AddAnimation("Idle", "BossIdle", 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Idle");
	animeName = m_BossName + "_idle";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
}

bool CBoss::Init()
{
	CGameObject::Init();
	m_Body = CreateComponent<CColliderSphere2D>("Body");
	m_Sprite = CreateComponent<CSpriteComponent>(m_BossName);

	m_Body->AddChild(m_Sprite);

	m_Body->SetCollisionProfile("Monster");
	m_Body->SetWorldPosition(960.f, 700.f);
	m_Sprite->SetPivot(0.5f, 0.5f);
	return true;
}

void CBoss::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
	if(!m_Anim|| m_Anim->GetCurrentAnimationName() == "Die")
	{
		return;
	}
//차지상태
	if (m_Anim->GetCurrentAnimationName() == "Charge")
	{
		m_SChargeTime += deltaTime;
		int leftTime = (int)(m_ChargeTime - m_SChargeTime);
		//m_BossUI->SetSkillName(std::to_string(leftTime),40.f,50.f);
		int markerCount =0;
		auto	iter = m_listMarker.begin();
		auto	iterEnd = m_listMarker.end();
		for (; iter != iterEnd; ++iter)
		{
			if (!(*iter)->GetActive())
			{
				++markerCount;
				continue;
			}
		}
//마커를 전부 깬 경우
		if (markerCount >= m_MarkerCount)
		{
			m_Scene->GetResource()->SoundStop("BossCharge");
			m_Scene->GetResource()->SoundPlay("BossStun");
			m_Anim->ChangeAnimation("Stun");
			m_SPatternDelay = 0;
			m_SChargeCount = 0;
			m_listMarker.clear();
			return;
		}
//시간초과
		if(m_SChargeTime> m_ChargeTime)
		{
			m_Scene->GetResource()->SoundStop("BossCharge");
			m_Anim->ChangeAnimation("Skill" + std::to_string(m_SkillCount - 1));
			m_RandomNum = rand() %(m_SkillCount - m_NormalPatternCount)+ m_NormalPatternCount;
			//m_BossUI->SetSkillName(m_SkillName[m_RandomNum]);
			CSkillManager::GetInst()->ActivateSkill(m_BossName + std::to_string(m_RandomNum), 
				m_Body, m_SkillName[m_RandomNum], m_vecSummonCount[m_RandomNum]);
			auto	iter = m_listMarker.begin();
			auto	iterEnd = m_listMarker.end();
			for (; iter != iterEnd; ++iter)
			{
				(*iter)->Destroy();
			}
			m_listMarker.clear();
			m_SPatternDelay = 0;
			m_SChargeCount = 0;
			if (m_vecIsLoop[m_RandomNum])
			{
				m_LoopIndex = m_RandomNum;
			}
		}
		return;
	}
//스턴상태
	if(m_Anim->GetCurrentAnimationName()=="Stun")
	{
		m_SDownTime += deltaTime;
		if(m_SDownTime> m_DownTime)
		{
			m_SDownTime = 0;
			m_Anim->ChangeAnimation("Idle");
		}
		return;
	}
//일반패턴
	m_SPatternDelay += deltaTime;
	if(m_SPatternDelay> m_vecPatternDelay[m_RandomNum])
	{
//m_ChargeCount가 -1이면 랜덤으로 스페셜스킬 발동
		if(m_ChargeCount==-1&& m_LoopIndex == -1)
		{
			if(m_RandomNum== m_NormalPatternCount)
			{
				m_SChargeCount = -2;
			}
			else
			{
				m_SChargeCount = rand() % 2 ? -2 : 0;
			}
		}
//일반패턴
		if (m_SChargeCount < m_ChargeCount)
		{
			if (m_LoopIndex == -1)
			{
				int randomNum = rand() % m_NormalPatternCount;
				while(m_RandomNum== randomNum)
				{
					randomNum = rand() % m_NormalPatternCount;
				}
				m_RandomNum = randomNum;
			}
//0번째 스킬을 소환스킬로 고정
			if(m_RandomNum ==0)	
			{
				m_SMaxSummonCount += m_vecSummonCount[m_RandomNum];
				if(m_SMaxSummonCount> m_MaxSummonCount)
				{
					m_SMaxSummonCount = m_MaxSummonCount;
					++m_RandomNum;
					m_LoopIndex = -1;
					m_SLoopCount = 0;
					//m_MoveType = EMoveType::None;
				}
			}
			if (m_vecIsLoop[m_RandomNum])
			{
				m_LoopIndex = m_RandomNum;
				++m_SLoopCount;
				if (m_SLoopCount > m_vecLoopCount[m_LoopIndex])
				{
					m_LoopIndex = -1;
					m_SLoopCount = 0;
					//m_MoveType = EMoveType::None;
					if (m_RandomNum < m_NormalPatternCount)
					{
						++m_SChargeCount;
					}
					return;
				}
			}
			m_Anim->ChangeAnimation("Skill" + std::to_string(m_RandomNum));
			//m_BossUI->SetSkillName(m_SkillName[m_RandomNum]);
			CSkillManager::GetInst()->ActivateSkill(m_BossName + std::to_string(m_RandomNum),
				m_Body, m_SkillName[m_RandomNum], m_vecSummonCount[m_RandomNum]);
			m_SPatternDelay = 0;
			if (m_LoopIndex == -1)
			{
				++m_SChargeCount;
			}
		}
//차지상태로 변경
		else
		{
			m_Scene->GetResource()->SoundPlay("BossCharge");
			m_Anim->ChangeAnimation("Charge");
			//m_MoveType = EMoveType::None;
			m_SChargeTime = 0;
			Vector2 pos = GetWorldPos();
			for (int i = 0; i < m_MarkerCount; ++i)
			{
				CMarker* marker = m_Scene->CreateObject<CMarker>("Marker");
				marker->SetWorldPosition(pos.x + m_vecMarkerPos[i].x, pos.y + m_vecMarkerPos[i].y);
				m_listMarker.push_back(marker);
			}
		}
	}
}

void CBoss::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
	if (m_DeadTime > 0.f)
	{
		m_DeadTime -= deltaTime;
		if (m_DeadTime <= 0.f)
		{
			Destroy();
			return;
		}
	}
	if (!m_Anim || m_Anim->GetCurrentAnimationName() == "Die")
	{
		return;
	}
	//BossMove(m_MoveType, deltaTime);
	//m_BossUI->SetPos(GetWorldPos());
}

CBoss* CBoss::Clone() const
{
	return new CBoss(*this);
}

void CBoss::Save(FILE* file)
{
	CGameObject::Save(file);
	int	length = (int)m_Sprite->GetName().length();
	fwrite(&length, 4, 1, file);
	fwrite(m_Sprite->GetName().c_str(), 1, length, file);
}

void CBoss::Load(FILE* file)
{
	CGameObject::Load(file);
	int	length = 0;
	char	typeName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(typeName, 1, length, file);
	m_Sprite->SetName(typeName);
}

void CBoss::PatternEnd()
{
	m_Anim->ChangeAnimation("Idle");
}

void CBoss::SetRandomPos()
{
	m_RandomPos.x = rand() % (int)(m_RangeX[1] - m_RangeX[0]) + m_RangeX[0];
	m_RandomPos.y = rand() % (int)(m_RangeY[1] - m_RangeY[0]) + m_RangeY[0];
}