#include "SkySphere.h"
#include "../Resource/Material/Material.h"
#include "../Component/StaticMeshComponent.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"

CSkySphere::CSkySphere()
{
	SetTypeID<CSkySphere>();
	m_ObjectTypeName = "SkySphere";
}

CSkySphere::CSkySphere(const CSkySphere& obj)
	: CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CSkySphere::~CSkySphere()
{
}

void CSkySphere::SetSkyTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_Material->SetTexture(0, 0, (int)EShaderBufferType::Pixel,	name, fileName, pathName);
}

void CSkySphere::Start()
{
	CGameObject::Start();
}

bool CSkySphere::Init()
{
	//	조명처리하면 안됌 예)gbuffer	
	CGameObject::Init();
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	SetRootComponent(m_Mesh);
	m_Mesh->SetMesh("SpherePos");
	m_Mesh->SetWorldScale(100000000.f, 100000000.f, 100000000.f);
	if (m_Scene)
	{
		m_Material = m_Scene->GetResource()->FindMaterial("Sky");
	}
	else
	{
		m_Material = CResourceManager::GetInst()->FindMaterial("Sky");
	}
	m_Mesh->AddMaterial(m_Material);
	m_Material = m_Mesh->GetMaterial(0);
	return true;
}

CSkySphere* CSkySphere::Clone() const
{
	return new CSkySphere(*this);
}

void CSkySphere::Save(FILE* file)
{
	CGameObject::Save(file);
}

void CSkySphere::Load(FILE* file)
{
	CGameObject::Load(file);
}
