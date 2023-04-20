#include "ColliderCube.h"
#include "CameraComponent.h"
#include "ColliderOBB3D.h"
#include "ColliderSphere3D.h"
#include "../Engine.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Scene/CameraManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../CollisionManager.h"

CColliderCube::CColliderCube()
{
	SetTypeID<CColliderCube>();
	SetRenderLayerName("Collider");
	m_ComponentTypeName = "ColliderCube";
	m_Collider3DType = ECollider3D_Type::Box;
	m_CubeSize.x = 100.f;
	m_CubeSize.y = 100.f;
	m_CubeSize.z = 100.f;
}

CColliderCube::CColliderCube(const CColliderCube& component)
	: CCollider3D(component)
{
	m_CubeSize = component.m_CubeSize;
}

CColliderCube::~CColliderCube()
{
}

void CColliderCube::Start()
{
	CCollider3D::Start();
}

bool CColliderCube::Init()
{
	if (!CCollider3D::Init())
	{
		return false;
	}
	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("CubeLinePos");
	}
	return true;
}

void CColliderCube::Update(float deltaTime)
{
	CCollider3D::Update(deltaTime);
}

void CColliderCube::PostUpdate(float deltaTime)
{
	CCollider3D::PostUpdate(deltaTime);
	Vector3	size = m_CubeSize;
	size.x *= GetWorldScale().x;
	size.y *= GetWorldScale().y;
	size.z *= GetWorldScale().z;
	Vector3	min;
	Vector3	max;
	min.x = GetWorldPos().x - size.x * 0.5f;
	min.y = GetWorldPos().y - size.y * 0.5f;
	min.z = GetWorldPos().z - size.z * 0.5f;
	max.x = min.x + size.x;
	max.y = min.y + size.y;
	max.z = min.z + size.z;
	m_Info.left = min.x;
	m_Info.bottom = min.y;
	m_Info.front = min.z;
	m_Info.right = max.x;
	m_Info.top = max.y;
	m_Info.back = max.z;
	min.x -= GetWorldPos().x;
	min.y -= GetWorldPos().y;
	min.z -= GetWorldPos().z;
	max.x -= GetWorldPos().x;
	max.y -= GetWorldPos().y;
	max.z -= GetWorldPos().z;
	m_Transform->SetMin(min);
	m_Transform->SetMax(max);
}

void CColliderCube::Render()
{
	CCollider3D::Render();
}

void CColliderCube::RenderDebug()
{
	CCollider3D::RenderDebug();
	Matrix matScale, matTranslate, matWorld;
	Matrix matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();
	Vector3	scale = GetWorldScale();
	scale.x *= m_CubeSize.x;
	scale.y *= m_CubeSize.y;
	scale.z *= m_CubeSize.z;
	matScale.Scaling(scale);
	matTranslate.Translation(GetWorldPos());
	matWorld = matScale * matTranslate;
	CColliderConstantBuffer* buffer = CResourceManager::GetInst()->GetColliderCBuffer();
	buffer->SetColor(m_Color);
	buffer->SetWVP(matWorld * matView * matProj);
	buffer->UpdateBuffer();
	m_Shader->SetShader();
	m_Mesh->Render();
}

CColliderCube* CColliderCube::Clone() const
{
	return new CColliderCube(*this);
}

void CColliderCube::Save(FILE* file)
{
	CCollider3D::Save(file);
	fwrite(&m_CubeSize, sizeof(Vector3), 1, file);
}

void CColliderCube::Load(FILE* file)
{
	CCollider3D::Load(file);
	fread(&m_CubeSize, sizeof(Vector3), 1, file);
	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("CubeLinePos");
	}
}

bool CColliderCube::Collision(CCollider* dest)
{
	Vector3	hitPoint;
	bool result = false;
	switch (((CCollider3D*)dest)->GetCollider3DType())
	{
	case ECollider3D_Type::Box:
		result = CCollisionManager::GetInst()->CollisionCubeToCube(hitPoint, this, (CColliderCube*)dest);
		break;
	case ECollider3D_Type::OBB:
		result = CCollisionManager::GetInst()->CollisionCubeToOBB3D(hitPoint, this, (CColliderOBB3D*)dest);
		break;
	case ECollider3D_Type::Sphere:
		result = CCollisionManager::GetInst()->CollisionCubeToSphere3D(hitPoint, this, (CColliderSphere3D*)dest);
		break;
	}
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, hitPoint.z);
	m_Result.hitPoint = m_HitPoint;
	dest->SetCollisionResultHitPoint(m_HitPoint);
	return result;
}
