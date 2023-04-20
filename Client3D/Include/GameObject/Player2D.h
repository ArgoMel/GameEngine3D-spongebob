#pragma once
#include "GameObject/GameObject.h"

//전부 비율(%) 
// 예) 0.1 == 10% 증가
struct AbilityData
{
	float HP;
	float Party_HP;
	float Attack;
	float Party_Attack;
	float MB_Attack;
	float Defense;
	float Party_Defense;
	float MB_Defence;
	float Barrier;
	float Party_Barrier;
	float Skill;
	float Party_Skill;
	float PF;				//파워플립
	float DA;				//직접공격
	float Party_DA;
	float MB_DA;
	float EA;				//추격데미지
	float Party_EA;
	float Gauge;
	float Party_Gauge;
	float MaxGauge;
	float Party_MaxGauge;
	float Fever;
	float FeverGain;		//적중시 피버 획득량
	float Party_FeverGain;	
	AbilityData()
		: HP(0.f)
		, Party_HP(0.f)
		, Attack(0.f)
		, Party_Attack(0.f)
		, MB_Attack(0.f)
		, Defense(0.f)
		, Party_Defense(0.f)
		, MB_Defence(0.f)
		, Barrier(0.f)
		, Party_Barrier(0.f)
		, Skill(0.f)
		, Party_Skill(0.f)
		, PF(0.f)
		, DA(0.f)
		, Party_DA(0.f)
		, MB_DA(0.f)
		, EA(0.f)
		, Party_EA(0.f)
		, Gauge(0.f)
		, Party_Gauge(0.f)
		, MaxGauge(0.f)
		, Party_MaxGauge(0.f)
		, Fever(0.f)
		, FeverGain(0.f)
		, Party_FeverGain(0.f)
	{
	}
	void operator += (const AbilityData& v)
	{
		HP += v.HP;
		Party_HP += v.Party_HP;
		Attack += v.Attack;
		Party_Attack += v.Party_Attack;
		MB_Attack += v.MB_Attack;
		Defense += v.Defense;
		Party_Defense += v.Party_Defense;
		MB_Defence += v.MB_Defence;
		Barrier += v.Barrier;
		Party_Barrier += v.Party_Barrier;
		Skill += v.Skill;
		Party_Skill += v.Party_Skill;
		PF += v.PF;
		DA += v.DA;
		Party_DA += v.Party_DA;
		MB_DA += v.MB_DA;
		EA += v.EA;
		Party_EA += v.Party_EA;
		Gauge += v.Gauge;
		Party_Gauge += v.Party_Gauge;
		MaxGauge += v.MaxGauge;
		Party_MaxGauge += v.Party_MaxGauge;
		Fever += v.Fever;
		FeverGain += v.FeverGain;
		Party_FeverGain += v.Party_FeverGain;
	}
	AbilityData& PartyPlus(const AbilityData& v)
	{
		Party_HP += v.Party_HP;
		Party_Attack += v.Party_Attack;
		MB_Attack += v.MB_Attack;
		Party_Defense += v.Party_Defense;
		MB_Defence += v.MB_Defence;
		Party_Barrier += v.Party_Barrier;
		Party_Skill += v.Party_Skill;
		PF += v.PF;
		Party_DA += v.Party_DA;
		MB_DA += v.MB_DA;
		Party_EA += v.Party_EA;
		Party_Gauge += v.Party_Gauge;
		Party_MaxGauge += v.Party_MaxGauge;
		Fever += v.Fever;
		Party_FeverGain += v.Party_FeverGain;
		return *this;
	}
};

class CPlayer2D :public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CSpriteComponent>	m_Sprite[3];
	CSharedPtr<class CSpriteComponent>	m_SpriteDir;
	CSharedPtr<class CSpriteComponent>	m_DirMarker;
	CSharedPtr<class CRigidBody>	m_Rigid;
	CSharedPtr<class CColliderSphere2D>	m_Body[3];
	CSharedPtr<class CAnimation2D>	m_Anim[3];
	//CSharedPtr<class CStageUI>	m_StageUI;
	float		m_Distance;
	float		m_IdleChange;
	float		m_TouchCoolTime;
	float		m_Gauge[3];
	float		m_MaxGauge[3];
	float		m_ComboTime;
	float		m_FeverGauge;
	int m_ComboCount;
	bool m_IsTouch;
	bool m_IsStart;
//저장 데이터
	std::string m_POutPutName[6];
	std::string m_SkillName[6];
	ObjectData m_PlayerData[6];
	bool m_IsLoop[6];
	int m_LoopCount[6];
	float m_PatternDelay[6];
	EGender m_Gender[6];
	float m_FeverGain[6];
	int m_HitCount[6];
	int m_SkillWait[6];
	float m_SkillDamage[6];
	int m_SummonCount[6];
	int m_MaxSummonCount[6];
	AbilityData m_AbilityData[6];
	ELeaderRole m_LeaderRole;
	AbilityData m_LeaderData;
	EMoveType m_MoveType;
//초기값
	int		m_SLoopCount[6];
	float	m_SPatternDelay[6];
	int		m_SMaxSummonCount[6];
//
protected:
	CPlayer2D();
	CPlayer2D(const CPlayer2D& obj);
	virtual ~CPlayer2D();
	void CollisionPlayer(const CollisionResult& result);
public:
	CRigidBody* GetRigidBody()	const
	{
		return m_Rigid;
	}
	ObjectData GetPlayerData(int index) const
	{
		return m_PlayerData[index];
	}
	AbilityData GetAbilityData(int index) const
	{
		return m_AbilityData[index];
	}
	int GetHitCount(int index) const
	{
		return m_HitCount[index];
	}
	float GetSkillDamage(int index) const
	{
		return m_SkillDamage[index];
	}
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CPlayer2D* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual int InflictDamage(float damage,int index);
	virtual void Reset();
	virtual void Move(EMoveType moveType, float deltaTime);
	void TouchPlayer();
	void HoldPlayer();
	void ReleasePlayer();
	void RePosition();
	void ViewSkill();
	void EndSkill();
	void ActivateSkill();
	bool PlayerWin();
	void PlayerDie();
	void CheckPlayerOut();
	void GainSkillGauge(int index,float percent);
};

