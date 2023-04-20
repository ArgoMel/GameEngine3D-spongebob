#pragma once
#include "GameObject/GameObject.h"
class CBoss : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderSphere2D>	m_Body;
	//CSharedPtr<class CBossUI>	m_BossUI;
	CSharedPtr<class CAnimation2D>	m_Anim;
	std::string m_BossName;
	std::string m_OutputName;
	std::string m_SkillName[10];
	Vector2 m_RandomPos;
	Vector2 m_StartPos;
	int m_dir;
	int m_SkillCount;
	int m_RandomNum;
	int m_LoopIndex;
	int m_NormalPatternCount;
	int m_ChargeCount;
	int m_MarkerCount;
	int m_MaxSummonCount;
	float m_ChargeTime;
	float m_DownTime;
	float m_RangeX[2];
	float m_RangeY[2];
	float m_DeadTime;
//�ʱⰪ
	int		m_SChargeCount;
	int		m_SMaxSummonCount;
	int		m_SLoopCount;
	float	m_SChargeTime;
	float	m_SDownTime;
	float	m_SPatternDelay;
//
	std::vector<int>	m_vecLoopCount;
	std::vector<Vector2>	m_vecMarkerPos;
	std::vector<int>	m_vecSummonCount;
	std::vector<float>	m_vecPatternDelay;
	std::vector<bool>	m_vecIsLoop;
	std::list<CSharedPtr<class CMarker>>  m_listMarker;
protected:
	CBoss();
	CBoss(const CBoss& obj);
	virtual ~CBoss();
public:

	void AddSMaxSummonCount(int count)
	{
		m_SMaxSummonCount += count;
		if(m_SMaxSummonCount<0)
		{
			m_SMaxSummonCount = 0;
		}
	}
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CBoss* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void PatternEnd();
	void SetRandomPos();
};

