#include "Bullet3D.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderCube.h"

CBullet3D::CBullet3D()
	: m_Speed(50.f)
{
	SetTypeID<CBullet3D>();
	m_ObjectTypeName = "Bullet3D";
}

CBullet3D::CBullet3D(const CBullet3D& obj)
	: CGameObject(obj)
	, m_Speed(obj.m_Speed)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Body = (CColliderCube*)FindComponent("Body");
}

CBullet3D::~CBullet3D()
{
}

void CBullet3D::Start()
{
	CGameObject::Start();
	m_Anim = m_Mesh->SetAnimation<CAnimation>("SpongebobMissileAnimation");
	m_Anim->AddAnimation("MissileStart", "SpongebobMissile_Start", 1.f, 1.f, false);
	m_Anim->SetCurrentEndFunction<CBullet3D>("MissileStart", this, &CBullet3D::ResetIdle);
	m_Anim->AddAnimation("MissileIdle", "SpongebobMissile_Idle", 1.f, 1.f, true);
	m_Anim->Start();
}

bool CBullet3D::Init()
{
	CGameObject::Init();
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	//m_Body = CreateComponent<CColliderCube>("Body");

	//m_Mesh->AddChild(m_Body);

	m_Mesh->SetMesh("SpongebobMissile");
	//m_Mesh->SetPivot(0.5f, 0.5f);
	//m_Mesh->SetRelativeScale(50.f, 50.f);

	//m_Body->SetCollisionCallback<CBullet>(ECollision_Result::Collision, this, &CBullet::CollisionBullet);
	return true;
}

void CBullet3D::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
	if (m_Anim->GetCurrentAnimationName() == "MissileIdle")
	{
		AddWorldPosition(m_Dir * m_Speed * deltaTime);
	}
}

void CBullet3D::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CBullet3D* CBullet3D::Clone() const
{
	return new CBullet3D(*this);
}

void CBullet3D::SetDir(const Vector3& vec)
{
	m_Dir = GetWorldPos() - vec;
	m_Dir.x = 0;
	m_Dir.y = 0;
}

void CBullet3D::CollisionBullet(const CollisionResult& result)
{
	Destroy();
}

void CBullet3D::ResetIdle()
{
	m_Anim->ChangeAnimation("MissileIdle");
}