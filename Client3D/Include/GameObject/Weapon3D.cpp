#include "Weapon3D.h"
//#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"

CWeapon3D::CWeapon3D()
{
	SetTypeID<CWeapon3D>();
	m_ObjectTypeName = "Weapon3D";
}

CWeapon3D::CWeapon3D(const CWeapon3D& obj)
	: CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CWeapon3D::~CWeapon3D()
{
}

void CWeapon3D::Start()
{
	CGameObject::Start();
}

bool CWeapon3D::Init()
{
	CGameObject::Init();
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	SetRootComponent(m_Mesh);
	m_Mesh->SetMesh("SpongebobWand");
	m_Mesh->SetWorldScale(0.5f, 0.5f, 0.5f);
	return true;
}

void CWeapon3D::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CWeapon3D::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CWeapon3D* CWeapon3D::Clone() const
{
	return new CWeapon3D(*this);
}

void CWeapon3D::Save(FILE* file)
{
	CGameObject::Save(file);
}

void CWeapon3D::Load(FILE* file)
{
	CGameObject::Load(file);
}
