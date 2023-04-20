#include "Monster3D.h"
#include "Component/StaticMeshComponent.h"
#include "Resource/Material/Material.h"

CMonster3D::CMonster3D()
{
	SetTypeID<CMonster3D>();
	m_ObjectTypeName = "Monster3D";
}

CMonster3D::CMonster3D(const CMonster3D& obj)
	: CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CMonster3D::~CMonster3D()
{
}

void CMonster3D::Start()
{
	CGameObject::Start();
}

bool CMonster3D::Init()
{
	CGameObject::Init();
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Mesh->SetMesh("Monster");
	//SetWorldPosition(500.f, 500.f, 0.f);
	return true;
}

void CMonster3D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMonster3D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonster3D* CMonster3D::Clone() const
{
	return new CMonster3D(*this);
}
