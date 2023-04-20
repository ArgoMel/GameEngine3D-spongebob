#include "Player2D.h"
#include "PathManager.h"
#include "Input.h"
#include "Engine.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation2D.h"
#include "UI/UIProgressBar.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "../Skill/SkillManager.h"
//#include "../UI/StageUI.h"

CPlayer2D::CPlayer2D()
	: m_Distance(0.f)
	, m_IdleChange(0.f)
	, m_TouchCoolTime(CAN_MOVE_TIME)
	, m_ComboTime(-1.f)
	, m_FeverGauge(0.f)
	, m_IsTouch(false)
	, m_IsStart(false)
	, m_ComboCount(0)
{
	SetTypeID<CPlayer2D>();
	m_ObjectTypeName = "Player2D";
	//CGameObject::Load(SAVE_PATH);
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj)
	: CGameObject(obj)
	, m_Distance(obj.m_Distance)
	, m_IdleChange(0.f)
	, m_TouchCoolTime(0.f)
	, m_ComboTime(-1.f)
	, m_FeverGauge(0.f)
	, m_IsTouch(false)
	, m_ComboCount(0)
{
	//CGameObject::Load(SAVE_PATH);
	m_SpriteDir = (CSpriteComponent*)FindComponent("spriteDir");
	m_DirMarker = (CSpriteComponent*)FindComponent("dirMarker");
	m_Rigid = (CRigidBody*)FindComponent("rigid");
	size_t size = std::size(m_Sprite);
	for (size_t i = 0; i < size; ++i)
	{
		std::string name = "none";//m_UserData.holdCharNames[i];
		m_Sprite[i] = (CSpriteComponent*)FindComponent(name);
		m_Body[i] = (CColliderSphere2D*)FindComponent("Body" + std::to_string(i));
	}
}

CPlayer2D::~CPlayer2D()
{
}

void CPlayer2D::Destroy()
{
	CGameObject::Destroy();
	//m_StageUI->Destroy();
	CInput::GetInst()->ClearCallback();
}

void CPlayer2D::Start()
{
	CGameObject::Start();
	m_Scene->GetResource()->SoundPlay("GameReady");
	//m_StageUI = m_Scene->GetViewport()->CreateUIWindow<CStageUI>("StageUI");
	size_t size = std::size(m_PlayerData);
	for (size_t i = 0; i < size; ++i)
	{
		std::string name = "none";// m_UserData.charName[m_UserData.partyIndex * 6 + i];
		if(name=="none")
		{
			continue;
		}
		//LoadCharacter((int)i);
		if(i>2)
		{
			m_PlayerData[i % 3] += m_PlayerData[i];
			m_FeverGain[i % 3] += m_FeverGain[i];
			m_SkillWait[i % 3] += m_SkillWait[i];
			m_AbilityData[i % 3] += m_AbilityData[i];
		}
		std::string imgPath = "Character//" + name + "//voice//battle//battle_start_0.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "BattleStart_" + name + "0", false, imgPath.c_str(), TEXTURE_PATH);
		imgPath = "Character//" + name + "//voice//battle//battle_start_1.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "BattleStart_" + name + "1", false, imgPath.c_str(), TEXTURE_PATH);

		imgPath = "Character//" + name + "//voice//battle//outhole_0.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "OutHole_" + name + "0", false, imgPath.c_str(), TEXTURE_PATH);
		imgPath = "Character//" + name + "//voice//battle//outhole_1.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "OutHole_" + name + "1", false, imgPath.c_str(), TEXTURE_PATH);

		imgPath = "Character//" + name + "//voice//battle//power_flip_0.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "PowerFlip_" + name + "0", false, imgPath.c_str(), TEXTURE_PATH);
		imgPath = "Character//" + name + "//voice//battle//power_flip_1.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "PowerFlip_" + name + "1", false, imgPath.c_str(), TEXTURE_PATH);

		imgPath = "Character//" + name + "//voice//battle//skill_ready.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "SkillReady" + std::to_string(i), false, imgPath.c_str(), TEXTURE_PATH);

		imgPath = "Character//" + name + "//voice//battle//win_0.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "Win_" + name + "0", false, imgPath.c_str(), TEXTURE_PATH);
		imgPath = "Character//" + name + "//voice//battle//win_1.mp3";
		m_Scene->GetResource()->LoadSound("Voice", "Win_" + name + "1", false, imgPath.c_str(), TEXTURE_PATH);
	}
//리더 보너스
	m_AbilityData[0] += m_LeaderData;	
	m_AbilityData[0].PartyPlus(m_AbilityData[1]);
	m_AbilityData[0].PartyPlus(m_AbilityData[2]);
//최종 수치
	size = std::size(m_Sprite);
	for (size_t i = 0; i < size; ++i)
	{
		m_PlayerData[i].attack = (size_t)(m_PlayerData[i].attack* 
			(1.f + (m_AbilityData[i].Attack + m_AbilityData[0].Party_Attack) * 0.01f));
		m_PlayerData[i].defense = (size_t)(m_PlayerData[i].defense * 
			(1.f + (m_AbilityData[i].Defense + m_AbilityData[0].Party_Defense) * 0.01f));
		m_PlayerData[i].hp = (size_t)(m_PlayerData[i].hp * 
			(1.f + (m_AbilityData[i].HP + m_AbilityData[0].Party_HP) * 0.01f));
		m_PlayerData[i].maxHp = m_PlayerData[i].hp;
		m_FeverGain[i] *= ((m_AbilityData[i].FeverGain + m_AbilityData[0].Party_FeverGain) * 0.01f + 1.f);
		m_Gauge[i]= (m_AbilityData[i].Gauge+ m_AbilityData[0].Party_Gauge)*0.01f* m_SkillWait[i];
		m_MaxGauge[i]= m_SkillWait[i]+m_AbilityData[i].MaxGauge * 0.01f * m_SkillWait[i];
		m_Gauge[i] = m_Gauge[i] > m_MaxGauge[i] ? m_MaxGauge[i] : m_Gauge[i];
		//m_StageUI->m_SkillChargeBar[i]->SetValue(m_Gauge[i]);
//애니메이션
		std::string name = "none";// m_UserData.charName[m_UserData.partyIndex * 6 + i];
		m_Sprite[i]->SetName(name);
		m_Anim[i] = m_Sprite[i]->SetAnimation<CAnimation2D>("PlayerAnim" + std::to_string(i));
		m_Anim[i]->AddAnimation("Skill", "Skill" + std::to_string(i), 1.5f, 1.f, false);
		std::string animeName = name + "_skill";
		m_Anim[i]->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
		m_Anim[i]->SetCurrentEndFunction("Skill", this, &CPlayer2D::EndSkill);
		m_Anim[i]->AddAnimation("Back", "Back" + std::to_string(i), 0.5f, 1.f, true);
		m_Anim[i]->SetCurrentAnimation("Back");
		animeName = name + "_back";
		m_Anim[i]->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
		m_Anim[i]->AddAnimation("Win", "Win" + std::to_string(i), 1.f, 1.f, true);
		m_Anim[i]->SetCurrentAnimation("Win");
		animeName = name + "_win";
		m_Anim[i]->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
		m_Anim[i]->AddAnimation("Damage", "Damage" + std::to_string(i), 1.f, 1.f, false);
		m_Anim[i]->SetCurrentAnimation("Damage");
		animeName = name + "_damage";
		m_Anim[i]->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
		m_Anim[i]->SetCurrentEndFunction("Damage", this, &CPlayer2D::EndSkill);
		m_Anim[i]->AddAnimation("Idle", "Idle" + std::to_string(i), 0.5f, 1.f, true);
		m_Anim[i]->SetCurrentAnimation("Idle");
		animeName = name + "_walk";
		m_Anim[i]->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	}
	m_FeverGauge = m_AbilityData[0].Fever*0.01f*MAX_FEVER;
	m_SpriteDir->SetRelativeRotationZ(45.f);

	int ranNum = rand() % 3;
	std::string name = "none"; // m_UserData.charName[m_UserData.partyIndex * 6 + ranNum];
	ranNum = rand() % 2;
	m_Scene->GetResource()->SoundPlay("BattleStart_" + name+ std::to_string(ranNum));

	CInput::GetInst()->AddBindFunction<CPlayer2D>("Touch", Input_Type::Down, this, &CPlayer2D::TouchPlayer, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer2D>("Touch", Input_Type::Stay, this, &CPlayer2D::HoldPlayer, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer2D>("Touch", Input_Type::Up, this, &CPlayer2D::ReleasePlayer, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer2D>("Skill", Input_Type::Down, this, &CPlayer2D::ViewSkill, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer2D>("Skill", Input_Type::Up, this, &CPlayer2D::ActivateSkill, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer2D>("RePosition", Input_Type::Down, this, &CPlayer2D::RePosition, m_Scene);
	m_Body[0]->SetCollisionCallback<CPlayer2D>(ECollision_Result::Collision, this, &CPlayer2D::CollisionPlayer);
}

bool CPlayer2D::Init()
{
	CGameObject::Init();
	size_t size = std::size(m_Sprite);
	for (size_t i = 0; i < size; ++i)
	{
		m_Body[i] = CreateComponent<CColliderSphere2D>("Body" + std::to_string(i));
		m_Body[i]->SetCollisionProfile("Player");
		m_Body[i]->SetPivot(0.5f, 0.5f);
		m_Body[i]->SetRadius(25.f);
		m_Body[i]->SetInheritRotZ(true);
		std::string name = "none"; //m_UserData.holdCharNames[i];
		m_Sprite[i] = CreateComponent<CSpriteComponent>(name);
		m_Sprite[i]->SetRelativeScale(50.f, 50.f);
		m_Sprite[i]->SetPivot(0.5f, 0.5f);
		m_Sprite[i]->SetInheritRotZ(true);
		m_Body[i]->AddChild(m_Sprite[i]);
	}
	m_SpriteDir = CreateComponent<CSpriteComponent>("spriteDir");
	m_DirMarker = CreateComponent<CSpriteComponent>("dirMarker");
	m_Rigid = CreateComponent<CRigidBody>("rigid");

	m_Body[0]->AddChild(m_Body[1]);
	m_Body[0]->AddChild(m_Body[2]);
	m_Body[0]->AddChild(m_SpriteDir);
	m_Body[0]->AddChild(m_DirMarker);
	m_Body[0]->AddChild(m_Rigid);

	m_Body[0]->SetWorldPosition(960.f, 200.f);
	m_Sprite[0]->SetRenderLayerName("Player");
	m_SpriteDir->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, "spriteDir"
		, TEXT("Effect\\direction_rainbow.png"));
	m_SpriteDir->GetMaterial(0)->SetOpacity(0.5f);
	m_SpriteDir->SetRelativeScale(40.f, 40.f);
	m_SpriteDir->SetRelativePosition(0.f, MAX_DISTANCE);
	m_SpriteDir->SetPivot(0.5f, 0.5f);
	m_SpriteDir->SetInheritRotZ(true);
	m_DirMarker->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, "dirMarker", TEXT("Effect\\line.png"));
	m_DirMarker->GetMaterial(0)->SetOpacity(0.f);
	m_DirMarker->SetRelativeScale(192.f, 9.f);
	m_DirMarker->SetRelativePosition(0.f, 0.f);
	m_DirMarker->SetPivot(0.75f, 0.5f);
	m_DirMarker->SetInheritRotZ(true);
	m_DirMarker->SetRenderLayerName("Effect");
	return true;
}

void CPlayer2D::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
	deltaTime *= m_TimeScale;
	if(m_ComboTime>0.f)
	{
		m_ComboTime -= deltaTime;
		if(m_ComboTime<=0.f)
		{
			m_ComboCount = 0;
			//m_StageUI->m_ComboText->SetText(TEXT("0 Combo"));
			//m_StageUI->m_ComboText->SetEnable(false);
		}
	}
//이김상태
	if (m_Anim[0]->GetCurrentAnimationName() == "Win")
	{
		return;
	}
	m_TouchCoolTime += deltaTime;
	m_IdleChange += deltaTime;
//애니메이션 랜덤 전환
	if (m_IdleChange > 1.5f)
	{
		m_IdleChange = 0.f;
		size_t size = std::size(m_Sprite);
		for (size_t i = 0; i < size; ++i)
		{
			if (m_Anim[i]->GetCurrentAnimationName() == "Skill"||
				m_Anim[i]->GetCurrentAnimationName() == "Damage")
			{
				break;
			}
			std::string ranAnim = rand() % 2 ? "Idle" : "Back";
			m_Anim[i]->ChangeAnimation(ranAnim);
		}
	}
	if (m_IsTouch|| !m_IsStart)
	{
		return;
	}
	m_Distance += m_Rigid->GetVelocity().Length() * deltaTime;
	if (m_TouchCoolTime >= CAN_MOVE_TIME)
	{
		m_SpriteDir->GetMaterial(0)->SetOpacity(0.5f);
	}
	if (m_Body[1]->GetRelativePos().y > -MAX_DISTANCE)
	{
		m_Body[1]->AddRelativePositionY(-100.f * deltaTime);
		m_Sprite[1]->AddRelativePositionY(100.f * deltaTime);
	}
	if (m_Body[2]->GetRelativePos().y > -MAX_DISTANCE * 2)
	{
		m_Body[2]->AddRelativePositionY(-200.f * deltaTime);
		m_Sprite[2]->AddRelativePositionY(200.f * deltaTime);
	}
}

void CPlayer2D::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
	Move(m_MoveType,deltaTime);
	CheckPlayerOut();
}

CPlayer2D* CPlayer2D::Clone() const
{
	return new CPlayer2D(*this);
}

void CPlayer2D::Save(FILE* file)
{
	CGameObject::Save(file);
}

void CPlayer2D::Load(FILE* file)
{
	CGameObject::Load(file);
	//CGameObject::Load(SAVE_PATH);
}

int CPlayer2D::InflictDamage(float damage, int index)
{
	if(m_PlayerData[index].hp<0)
	{
		return 0;
	}
	int inflicDamage= (int)(damage - damage * m_PlayerData[index].defense * 0.01f);
	m_PlayerData[index].hp -= inflicDamage;
	//m_StageUI->Damaged(index,m_PlayerData[index].hp / (float)m_PlayerData[index].maxHp * 100.f);
	//m_StageUI->AddScore(inflicDamage);
	m_TouchCoolTime = CAN_MOVE_TIME;
//1명 사망
	if (m_PlayerData[index].hp <= 0)
	{
		std::string name = "none";//m_UserData.charName[m_UserData.partyIndex * 6 + index];
		int ranNum = rand() % 2;
		m_Scene->GetResource()->SoundPlay("OutHole_" + name + std::to_string(ranNum));
		if(index!=0)
		{
			m_Body[index]->SetCollisionProfile("Invisible");
		}
		m_Sprite[index]->GetMaterial(0)->SetBaseColorUnsignedChar(100, 100, 100, 50);
		PlayerDie();
	}
	return m_PlayerData[index].hp;
}

void CPlayer2D::Reset()
{
	CGameObject::Reset();
	size_t size = std::size(m_Sprite);
	for (size_t i = 0; i < size; ++i)
	{
		m_Body[i]->SetCollisionProfile("Player");
		m_Sprite[i]->GetMaterial(0)->SetBaseColorUnsignedChar(255, 255, 255, 255);
		m_Gauge[i] = (float)m_SkillWait[i];
	}
	SetWorldPosition(960.f, 300.f);
	m_TimeScale = 1.f;
	SetWorldRotationZ(0.f);
	//m_StageUI->AddScore(10000);
}

void CPlayer2D::Move(EMoveType moveType, float deltaTime)
{
	if(moveType == EMoveType::None)
	{
		return;
	}
	else if (moveType == EMoveType::RushToBoss)
	{
		Vector2 pos = GetWorldPos();
		Vector2 bossPos=m_Scene->GetSceneInfo()->GetBossObject()->GetWorldPos();
		if (pos.Distance(bossPos) < 200.f)
		{
			return;
		}
		float angle = pos.Angle(bossPos);
		SetWorldRotationZ(angle + 90.f);
		AddWorldPositionX(cosf(DegreeToRadian(angle)) * m_PlayerData[0].speed * deltaTime);
		AddWorldPositionY(sinf(DegreeToRadian(angle)) * m_PlayerData[0].speed * deltaTime);
	}
}

void CPlayer2D::TouchPlayer()
{
	if (0.f == CEngine::GetInst()->GetTimeScale()||0.f== m_TimeScale)
	{
		m_IsTouch = false;
		m_IsStart = false;
		return;
	}
	if (!m_IsStart)
	{
		m_IsStart = true;
	}
	if (m_TouchCoolTime >= CAN_MOVE_TIME)
	{
		m_IsTouch = true;
		m_Scene->GetResource()->SoundPlay("PlayerTouch");
		m_SpriteDir->GetMaterial(0)->SetOpacity(1.f);
		m_DirMarker->GetMaterial(0)->SetOpacity(0.85f);
		//m_StageUI->TouchWindow();
	}
}

void CPlayer2D::HoldPlayer()
{
	if (!m_IsTouch||0.f == m_TimeScale)
	{
		return;
	}
	Vector2 dir; //m_StageUI->m_MousePosImg->GetPos();
	Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
	float angle = dir.Angle(mouseWorldPos);
	float distance = dir.Distance(mouseWorldPos);
	m_Body[0]->SetWorldRotationZ(angle + 90.f);
	m_SpriteDir->SetRelativeRotationZ(45.f);
	m_DirMarker->SetRelativeRotationZ(90.f);
	m_Rigid->SetVelocity(0, 0);
	if (m_Body[1]->GetRelativePos().y < -1)
	{
		m_Body[1]->AddRelativePositionY(100.f * g_DeltaTime);
		m_Sprite[1]->AddRelativePositionY(-100.f * g_DeltaTime);
	}
	if (m_Body[2]->GetRelativePos().y < -1)
	{
		m_Body[2]->AddRelativePositionY(200.f * g_DeltaTime);
		m_Sprite[2]->AddRelativePositionY(-200.f * g_DeltaTime);
	}
	//m_StageUI->HoldWindow(angle, distance);
}

void CPlayer2D::ReleasePlayer()
{
	if (!m_IsTouch|| 0.f == m_TimeScale)
	{
		return;
	}
	if (!m_Rigid->GetGravity())
	{
		m_Rigid->SetGravity(true);
	}
	m_Scene->GetResource()->SoundPlay("PlayerRelease");
	size_t size = std::size(m_Gauge);
	for (size_t i = 0; i < size;++i)
	{
		if (m_Gauge[i] >= m_SkillWait[i])
		{
			continue;
		}
		float gaugeRate = m_Distance * 0.02f;
		if(gaugeRate>50.f)
		{
			gaugeRate = 50.f;
		}
		m_Gauge[i] += gaugeRate;
		if(m_Gauge[i] >= m_SkillWait[i])
		{
			m_Gauge[i] = (float)m_SkillWait[i];
			m_Scene->GetResource()->SoundPlay("SkillReady" + std::to_string(i));
		}
	}
	m_Distance = 0.f;
	m_TouchCoolTime = 0.f;
	m_IsTouch = false;
	m_SpriteDir->GetMaterial(0)->SetOpacity(0.f);
	m_DirMarker->GetMaterial(0)->SetOpacity(0.f);
	float angle=m_Body[0]->GetWorldRot().z + 90.f;
	Vector2	dir;
	m_PlayerData[0].speed = m_PlayerData[0].maxSpeed;
	float distance=0.f;// m_StageUI->m_CharDirMarker->GetSize().x;
	dir.x = cosf(DegreeToRadian(angle)) * m_PlayerData[0].speed * distance * 0.01f;
	dir.y = sinf(DegreeToRadian(angle)) * m_PlayerData[0].speed * distance * 0.01f;
	m_Rigid->AddForce(dir.x *2.f, dir.y * 2.f);
	m_Rigid->SetVelocity(dir.x , dir.y);
	std::string role = std::to_string((int)m_LeaderRole);
	if (m_ComboCount > 10)
	{
		CSkillManager::GetInst()->ActivateSkill("PowerFlip"+ role, m_Body[0], role, m_ComboCount);
	}
	m_ComboCount = 0;
	//m_StageUI->m_ComboText->SetText(TEXT("0 Combo"));
	//m_StageUI->ReleaseWindow(m_Gauge, m_SkillWait);
	//m_StageUI->AddScore((int)distance);
}

void CPlayer2D::RePosition()
{
	Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
	m_Body[0]->SetWorldPosition(mouseWorldPos);
	m_Rigid->SetGravity(false);
	m_Rigid->SetVelocity(0.f, 0.f);
}

void CPlayer2D::ViewSkill()
{
	//m_StageUI->ViewSkill(m_Gauge, m_SkillWait);
}

void CPlayer2D::EndSkill()
{
	m_MoveType=EMoveType::None;
	size_t size = std::size(m_Sprite);
	for (size_t i = 0; i < size; ++i)
	{
		if (m_Anim[i]->GetCurrentAnimationName() == "Skill"||
			m_Anim[i]->GetCurrentAnimationName() == "Damage")
		{
			m_Anim[i]->ChangeAnimation("Idle");
			m_Sprite[i]->SetRelativeScale(50.f, 50.f);
		}
	}
}

void CPlayer2D::ActivateSkill()
{
	int index=0;// = m_StageUI->ActivateSkill(m_Gauge, m_SkillWait);
	if(index<0)
	{
		return;
	}
	m_Anim[index]->ChangeAnimation("Skill");
	m_Sprite[index]->SetRelativeScale(130.f, 130.f);
	CSkillManager::GetInst()->ActivateSkill(m_Sprite[index]->GetName(), m_Body[0], m_SkillName[index], index);
	//m_StageUI->AddScore(m_SkillWait[index]);
}

bool CPlayer2D::PlayerWin()
{
	//if (m_StageUI->m_IsRetryUI)
	//{
	//	return false;
	//}
	size_t size = std::size(m_Sprite);
	for (size_t i = 0; i < size; ++i)
	{
		m_Anim[i]->ChangeAnimation("Win");
		m_Sprite[i]->SetRelativeScale(50.f, 50.f);
	}
	m_Rigid->SetGravity(false);
	m_Rigid->AddForce(0.f, -100.f);
	m_Rigid->SetVelocity(0.f, -50.f);
	m_Body[0]->SetWorldRotationZ(0.f);
	//m_StageUI->m_FadeTime = 0.f;
	int ranNum = rand() % 3;
	std::string name = "none";// m_UserData.charName[m_UserData.partyIndex * 6 + ranNum];
	ranNum = rand() % 2;
	m_Scene->GetResource()->SoundPlay("Win_" + name + std::to_string(ranNum));
	//m_StageUI->AddScore(10000);
	return true;
}

void CPlayer2D::PlayerDie()
{
	bool isDead = true;
	size_t size = std::size(m_Sprite);
	for (size_t i = 0; i < size; ++i)
	{
		if (m_PlayerData[i].hp > 0)
		{
			isDead = false;
			return;
		}
	}
	if(isDead)
	{
		m_TimeScale = 0.f;
		m_Rigid->SetGravity(false);
		m_Rigid->SetVelocity(0.f, 0.f);
		m_Body[0]->SetWorldRotationZ(0.f);
		//m_StageUI->RetryUI();
		int ranNum = rand() % 3;
		std::string name = "none"; //m_UserData.charName[m_UserData.partyIndex * 6 + ranNum];
		ranNum = rand() % 2;
		m_Scene->GetResource()->SoundPlay("OutHole_" + name + std::to_string(ranNum));
	}

}

void CPlayer2D::CheckPlayerOut()
{
	Vector2 pos = GetWorldPos();
	if(pos.x<600.f|| pos.x > 1300.f)
	{
		SetWorldPosition(960.f, 200.f);
		m_Rigid->SetGravity(false);
		m_Rigid->SetVelocity(0.f, 0.f);
	}
	if(pos.y < 0.f || pos.y > 960.f)
	{
		SetWorldPosition(960.f, 200.f);
		m_Rigid->SetGravity(false);
		m_Rigid->SetVelocity(0.f, 0.f);
	}
}

void CPlayer2D::GainSkillGauge(int index, float percent)
{
	m_Gauge[index]+= percent*m_SkillWait[index] *0.01f;
	m_Gauge[index] = m_Gauge[index] > m_MaxGauge[index] ? m_MaxGauge[index] : m_Gauge[index];
	//m_StageUI->ReleaseWindow(m_Gauge, m_SkillWait);
}

void CPlayer2D::CollisionPlayer(const CollisionResult& result)
{
	Vector2 hitPoint = result.hitPoint;
	Vector2 playerPos =GetWorldPos();
	int axisAngle=(int)(GetWorldRot().z+90.f) % 360;
	std::string profile = result.dest->GetCollisionProfile()->name;
	if (result.dest->GetOwner()->GetName() == "spike")
	{
		size_t size = std::size(m_Sprite);
		for (size_t i = 0; i < size; ++i)
		{
			m_Anim[i]->ChangeAnimation("Damage");
			m_Sprite[i]->SetRelativeScale(50.f, 130.f);
		}
		m_Rigid->SetGravity(false);
		m_Rigid->AddForce(0.f, 300.f);
		m_Rigid->SetVelocity(0.f, 150.f);
		m_Body[0]->SetWorldRotationZ(0.f);
		m_Start = false;
		int ranNum = rand() % 3;
		std::string name = "none"; // m_UserData.charName[m_UserData.partyIndex * 6 + ranNum];
		ranNum = rand() % 2;
		m_Scene->GetResource()->SoundPlay("OutHole_" + name + std::to_string(ranNum));
	}
	else if (profile == "Default"|| profile == "Monster")
	{
		float angle = playerPos.Angle(hitPoint);	// 플레이어 중심점과 히트포인트의 각도를 구함
		float verticalAngle = angle + 90;	//	수직선의 각도
		int acuteAngle =(int)verticalAngle % 90 - axisAngle % 90;	//예각
		int u_AcuteAngle = abs(acuteAngle);	//예각
		if (GetMove().y < 0 && abs(axisAngle - 90) < 60)	//30~150
		{
			u_AcuteAngle = u_AcuteAngle < 45 ? u_AcuteAngle+45: u_AcuteAngle;
			if (playerPos.x < hitPoint.x)
			{
				AddWorldRotationZ((float)u_AcuteAngle);
			}
			else
			{
				AddWorldRotationZ((float)u_AcuteAngle*-1.f);
			}
		}
		else if (u_AcuteAngle < 18)
		{
			AddWorldRotationZ(u_AcuteAngle + 180.f);
		}
		else if (u_AcuteAngle > 75&&u_AcuteAngle <= 90)
		{
			AddWorldRotationZ(u_AcuteAngle + 90.f);
		}
		else if (axisAngle < angle)
		{
			AddWorldRotationZ(-(float)u_AcuteAngle - 45.f);
		}
		else if (axisAngle >270&& 360- axisAngle>angle)
		{
			AddWorldRotationZ(-(float)u_AcuteAngle - 45.f);
		}
		else
		{
			AddWorldRotationZ((float)u_AcuteAngle +45.f);
		}
//각도계산후 바라보는 방향으로 속력을 준다
		angle = GetWorldRot().z + 90.f;
		Vector2	dir;
		if (m_ComboCount > 0)
		{
			m_PlayerData[0].speed *= 0.9f;
		}
		if (m_PlayerData[0].speed < 50.f)
		{
			m_PlayerData[0].speed = 50.f;
		}
		dir.x = cosf(DegreeToRadian(angle)) * m_PlayerData[0].speed;
		dir.y = sinf(DegreeToRadian(angle)) * m_PlayerData[0].speed;
		m_Rigid->AddForce(dir.x * 2.f, dir.y * 2.f);
		m_Rigid->SetVelocity(dir.x, dir.y);
		m_ComboCount += 3;
		m_ComboTime = 3.f;
		//m_StageUI->m_ComboText->SetEnable(true);
		TCHAR	comboCount[MAX_PATH] = {};
		wsprintf(comboCount, TEXT("%d Combo"), m_ComboCount);
		//m_StageUI->m_ComboText->SetText(comboCount);
		m_Scene->GetResource()->SoundPlay("PlayerHit");
		if (profile == "Monster")
		{
			float directAttack = m_PlayerData[0].attack *
				(1.f + (m_AbilityData[0].DA + m_AbilityData[0].Party_DA) * 0.01f);
			int leftHP = result.dest->GetOwner()->InflictDamage(directAttack);
			m_FeverGauge+=m_FeverGain[0];
			//m_StageUI->m_FeverBar->SetValue(m_FeverGauge/MAX_FEVER*100.f);
		//	m_StageUI->AddScore((int)directAttack);
		}
	}
	else if (profile == "Marker")
	{
		m_Scene->GetResource()->SoundPlay("PlayerHit");
		result.dest->GetOwner()->InflictDamage(0.f);
		//m_StageUI->AddScore(1000);
	}
}

/*
#include "Component/NavigationAgent.h"
	m_NavAgent = (CNavigationAgent*)FindComponent("NavAgent");
CInput::GetInst()->AddBindFunction<CPlayer2D>("MoveClick", Input_Type::Down, this, &CPlayer2D::MoveClick, m_Scene);
	m_NavAgent = CreateComponent<CNavigationAgent>("NavAgent");

void CPlayer2D::MoveClick()
{
	const Vector2&	MousePos = CInput::GetInst()->GetMouseWorldPos();

	if (m_NavAgent)
		m_NavAgent->Move(MousePos);
}
*/