#pragma once
#include "GameObject/GameObject.h"

class CBackGround : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CSpriteComponent>	m_BG;
	CSharedPtr<class CColliderPixel>	m_Body;
	CSharedPtr<class CCameraComponent>	m_Camera;
	CSharedPtr<class CTargetArm>	m_Arm;
protected:
	CBackGround();
	CBackGround(const CBackGround& obj);
	virtual ~CBackGround();
public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CBackGround* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void TouchScreen();
};
