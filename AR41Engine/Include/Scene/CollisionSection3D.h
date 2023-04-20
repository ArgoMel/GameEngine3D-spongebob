#pragma once
#include "../EngineInfo.h"

class CCollisionSection3D
{
private:
	friend class CSceneCollision;
	std::list<CSharedPtr<class CCollider3D>>	m_ColliderList;
	Vector3		m_Min;
	Vector3		m_Max;
	Vector3		m_Center;
	float		m_Radius;
	int			m_IndexX;
	int			m_IndexY;
	int			m_IndexZ;
	int			m_Index;
	CCollisionSection3D();
	~CCollisionSection3D();
public:
	void AddCollider(class CCollider3D* collider);
	void Collision(float deltaTime);
};

