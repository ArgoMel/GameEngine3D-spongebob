#pragma once
#include "GameObject/GameObject.h"

class CMonster3D : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
protected:
	CMonster3D();
	CMonster3D(const CMonster3D& obj);
	virtual ~CMonster3D();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CMonster3D* Clone()    const;
};

