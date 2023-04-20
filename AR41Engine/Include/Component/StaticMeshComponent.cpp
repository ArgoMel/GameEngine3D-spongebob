#include "StaticMeshComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	SetTypeID<CStaticMeshComponent>();
	m_ComponentTypeName = "StaticMeshComponent";
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& component)
	: CPrimitiveComponent(component)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

bool CStaticMeshComponent::SetMesh(const std::string& name)
{
	return CPrimitiveComponent::SetMesh(name);
}

bool CStaticMeshComponent::SetMesh(CMesh* mesh)
{
	return CPrimitiveComponent::SetMesh(mesh);
}

bool CStaticMeshComponent::SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (m_Scene)
	{
		m_Scene->GetResource()->LoadMesh(MeshType::Static, name, fileName, pathName);
		m_Mesh = m_Scene->GetResource()->FindMesh(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadMesh(m_Scene, MeshType::Static, name, fileName, pathName);
		m_Mesh = CResourceManager::GetInst()->FindMesh(name);
	}
	if (m_Mesh)
	{
		SetMeshSize(m_Mesh->GetMeshSize());
	}
	m_vecMaterial.clear();
	int slotCount = m_Mesh->GetSlotCount();
	for (int i = 0; i < slotCount; ++i)
	{
		CMaterial* material = m_Mesh->GetMaterial(i);
		m_vecMaterial.push_back(material->Clone());
	}
	return true;
}

bool CStaticMeshComponent::SetMeshFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (m_Scene)
	{
		m_Scene->GetResource()->LoadMeshFullPath(MeshType::Static, name, fullPath);
		m_Mesh = m_Scene->GetResource()->FindMesh(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadMeshFullPath(m_Scene, MeshType::Static, name, fullPath);
		m_Mesh = CResourceManager::GetInst()->FindMesh(name);
	}
	if (m_Mesh)
	{
		SetMeshSize(m_Mesh->GetMeshSize());
	}
	m_vecMaterial.clear();
	int slotCount = m_Mesh->GetSlotCount();
	for (int i = 0; i < slotCount; ++i)
	{
		CMaterial* material = m_Mesh->GetMaterial(i);
		m_vecMaterial.push_back(material->Clone());
	}
	return true;
}

void CStaticMeshComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CStaticMeshComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}
	if (m_Scene)
	{
		m_ShadowMapShader = (CGraphicShader*)m_Scene->GetResource()->FindShader("ShadowMapStaticShader");
	}
	else
	{
		m_ShadowMapShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("ShadowMapStaticShader");
	}
	SetMesh("GizMo");
	return true;
}

void CStaticMeshComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
}

void CStaticMeshComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
}

void CStaticMeshComponent::Render()
{
	CPrimitiveComponent::Render();
}

void CStaticMeshComponent::RenderShadowMap()
{
	CPrimitiveComponent::RenderShadowMap();
}

CStaticMeshComponent* CStaticMeshComponent::Clone() const
{
	return new CStaticMeshComponent(*this);
}

void CStaticMeshComponent::Save(FILE* file)
{
	CPrimitiveComponent::Save(file);
}

void CStaticMeshComponent::Load(FILE* file)
{
	CPrimitiveComponent::Load(file);
}
