#pragma once
#include "GameObject\GameObject.h"

class CWeapon3D : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
protected:
	CWeapon3D();
	CWeapon3D(const CWeapon3D& obj);
	virtual ~CWeapon3D();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CWeapon3D* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
};

