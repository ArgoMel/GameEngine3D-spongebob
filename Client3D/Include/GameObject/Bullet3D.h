#pragma once
#include "GameObject/GameObject.h"

class CBullet3D : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CColliderCube> m_Body;
	CSharedPtr<class CAnimation> m_Anim;
	Vector3 m_Dir;
	float m_Speed;
	void CollisionBullet(const CollisionResult& result);
	void ResetIdle();	//아이들상태로 되돌리는 함수
protected:
	CBullet3D();
	CBullet3D(const CBullet3D& obj);
	virtual ~CBullet3D();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CBullet3D* Clone()    const;
	void SetDir(const Vector3& vec);
};

