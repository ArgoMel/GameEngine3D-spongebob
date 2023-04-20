#pragma once
#include "GameObject/GameObject.h"

class CMarker : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderSphere2D>	m_Body;
	CSharedPtr<class CAnimation2D>	m_Anim;
protected:
	CMarker();
	CMarker(const CMarker& obj);
	virtual ~CMarker();
public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CMarker* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual int InflictDamage(float damage);
};

