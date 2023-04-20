#include "FogComponent.h"
#include "../PathManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/TerrainConstantBuffer.h"

CFogComponent::CFogComponent()
{
    SetTypeID<CFogComponent>();
    m_ComponentTypeName = "FogComponent";
    m_CBuffer = new CTerrainConstantBuffer;
    m_CBuffer->Init();
}

CFogComponent::CFogComponent(const CFogComponent& component)
    : CPrimitiveComponent(component)
{
    if (component.m_CBuffer)
    {
        m_CBuffer = component.m_CBuffer->Clone();
    }
}

CFogComponent::~CFogComponent()
{
    SAFE_DELETE(m_CBuffer);
}

void CFogComponent::SetFogData(const Vector4& color, float start, float end)
{
	GetMaterial(0)->SetBaseColor(color);
	m_FogStart = start;
	m_FogEnd = end;
}

bool CFogComponent::SetMesh(const std::string& name)
{
    return CPrimitiveComponent::SetMesh(name);
}

bool CFogComponent::SetMesh(CMesh* mesh)
{
    return CPrimitiveComponent::SetMesh(mesh);
}

bool CFogComponent::SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName)
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
		SetMin(m_Mesh->GetMin());
		SetMax(m_Mesh->GetMax());
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

bool CFogComponent::SetMeshFullPath(const std::string& name, const TCHAR* fullPath)
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
		SetMin(m_Mesh->GetMin());
		SetMax(m_Mesh->GetMax());
		SetMeshSize(m_Mesh->GetMeshSize());
	}
	m_vecMaterial.clear();
	int slotCount = m_Mesh->GetSlotCount();
	for (int i = 0; i < slotCount; i++)
	{
		CMaterial* material = m_Mesh->GetMaterial(i);
		m_vecMaterial.push_back(material->Clone());
	}
	return true;
}

void CFogComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CFogComponent::Init()
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
	return true;
}

void CFogComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
}

void CFogComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
}

void CFogComponent::RenderShadowMap()
{
	CPrimitiveComponent::RenderShadowMap();
}

void CFogComponent::Render()
{
	CPrimitiveComponent::Render();
}

CFogComponent* CFogComponent::Clone() const
{
	return new CFogComponent(*this);
}

void CFogComponent::Save(FILE* file)
{
}

void CFogComponent::Load(FILE* file)
{
}
