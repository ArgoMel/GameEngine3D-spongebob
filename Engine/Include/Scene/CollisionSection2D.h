#pragma once
#include "../EngineInfo.h"

class CCollisionSection2D
{
private:
	friend class CSceneCollision;
	Vector2		m_Min;
	Vector2		m_Max;
	int			m_IndexX;
	int			m_IndexY;
	int			m_Index;
	std::list<CSharedPtr<class CCollider2D>>	m_ColliderList;
	static bool ColliderSort(class CCollider2D* src, class CCollider2D* dest);
	CCollisionSection2D();
	~CCollisionSection2D();
public:
	void AddCollider(class CCollider2D* collider);
	void Collision(float deltaTime);
	class CCollider* CollisionMouse(const Vector2& mouseWorldPos, float deltaTime);
};

