#include "AnimationMeshComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/StructuredBuffer.h"

CAnimationMeshComponent::CAnimationMeshComponent()
{
	SetTypeID<CAnimationMeshComponent>();
	m_ComponentTypeName = "AnimationMeshComponent";
}

CAnimationMeshComponent::CAnimationMeshComponent(const CAnimationMeshComponent& component)
	: CPrimitiveComponent(component)
{
	if (component.m_Skeleton)
	{
		m_Skeleton = component.m_Skeleton->Clone();
	}
	m_Socket = nullptr;
	if (component.m_Animation)
	{
		m_Animation = component.m_Animation->Clone();
		if (m_Skeleton)
		{
			m_Animation->SetSkeleton(m_Skeleton);
		}
		if (m_Mesh)
		{
			m_Animation->SetInstancingBoneBuffer(((CAnimationMesh*)m_Mesh.Get())->GetBoneBuffer());
		}
	}
}

CAnimationMeshComponent::~CAnimationMeshComponent()
{
}

void CAnimationMeshComponent::GetAnimationNames(std::vector<std::string>& vecNames)
{
	if (m_Animation)
	{
		m_Animation->GetAnimationNames(vecNames);
	}
}

void CAnimationMeshComponent::SetAnimation(CAnimation* anim)
{
	m_Animation = anim;
}

bool CAnimationMeshComponent::SetMesh(const std::string& name)
{
	CPrimitiveComponent::SetMesh(name);
	CAnimationMesh* mesh = (CAnimationMesh*)m_Mesh.Get();
	m_Skeleton = mesh->GetSkeleton()->Clone();
	if (m_Animation)
	{
		m_Animation->SetSkeleton(m_Skeleton);
		m_Animation->SetInstancingBoneBuffer(mesh->GetBoneBuffer());
	}
	return true;
}

bool CAnimationMeshComponent::SetMesh(CMesh* mesh)
{
	CPrimitiveComponent::SetMesh(mesh);
	CAnimationMesh* animMesh = (CAnimationMesh*)m_Mesh.Get();
	m_Skeleton = animMesh->GetSkeleton()->Clone();
	if (m_Animation)
	{
		m_Animation->SetSkeleton(m_Skeleton);
		m_Animation->SetInstancingBoneBuffer(animMesh->GetBoneBuffer());
	}
	return true;
}

bool CAnimationMeshComponent::SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName)
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
	CAnimationMesh* animMesh = (CAnimationMesh*)m_Mesh.Get();
	m_Skeleton = animMesh->GetSkeleton()->Clone();
	if (m_Animation)
	{
		m_Animation->SetSkeleton(m_Skeleton);
		m_Animation->SetInstancingBoneBuffer(animMesh->GetBoneBuffer());
	}
	return true;
}

bool CAnimationMeshComponent::SetMeshFullPath(const std::string& name, const TCHAR* fullPath)
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
	CAnimationMesh* animMesh = (CAnimationMesh*)m_Mesh.Get();
	m_Skeleton = animMesh->GetSkeleton()->Clone();
	if (m_Animation)
	{
		m_Animation->SetSkeleton(m_Skeleton);
		m_Animation->SetInstancingBoneBuffer(animMesh->GetBoneBuffer());
	}
	return true;
}

void CAnimationMeshComponent::Start()
{
	CPrimitiveComponent::Start();
	if (m_Animation)
	{
		m_Animation->Start();
	}
}

bool CAnimationMeshComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}
	if (m_Scene)
	{
		m_ShadowMapShader = (CGraphicShader*)m_Scene->GetResource()->FindShader("ShadowMapShader");
	}
	else
	{
		m_ShadowMapShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("ShadowMapShader");
	}
	return true;
}

void CAnimationMeshComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
	if (m_Animation)
	{
		m_Animation->Update(deltaTime);
	}
}

void CAnimationMeshComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
}

void CAnimationMeshComponent::Render()
{
	if (m_Animation)
	{
		m_Animation->SetShader();
	}
	CPrimitiveComponent::Render();
	if (m_Animation)
	{
		m_Animation->ResetShader();
	}
}

void CAnimationMeshComponent::RenderShadowMap()
{
	if (m_Animation)
	{
		m_Animation->SetShader();
	}
	CPrimitiveComponent::RenderShadowMap();
	if (m_Animation)
	{
		m_Animation->ResetShader();
	}
}

CAnimationMeshComponent* CAnimationMeshComponent::Clone() const
{
	return new CAnimationMeshComponent(*this);
}

void CAnimationMeshComponent::Save(FILE* file)
{
	CPrimitiveComponent::Save(file);
}

void CAnimationMeshComponent::Load(FILE* file)
{
	CPrimitiveComponent::Load(file);
}
