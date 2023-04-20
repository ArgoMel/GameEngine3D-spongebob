#include "ColliderSphere3D.h"
#include "ColliderCube.h"
#include "ColliderOBB3D.h"
#include "CameraComponent.h"
#include "../Engine.h"
#include "../CollisionManager.h"
#include "../Scene/Scene.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CColliderSphere3D::CColliderSphere3D()
{
	SetTypeID<CColliderSphere3D>();
	SetRenderLayerName("Collider");
	m_ComponentTypeName = "ColliderSphere3D";
	m_Collider3DType = ECollider3D_Type::Sphere;
	m_Radius = 50.f;
}

CColliderSphere3D::CColliderSphere3D(const CColliderSphere3D& component)
	: CCollider3D(component)
{
	m_Radius = component.m_Radius;
}

CColliderSphere3D::~CColliderSphere3D()
{
}

void CColliderSphere3D::Start()
{
	CCollider3D::Start();
}

bool CColliderSphere3D::Init()
{
	if (!CCollider3D::Init())
	{
		return false;
	}
	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("SpherePos");
	}

	return true;
}

void CColliderSphere3D::Update(float deltaTime)
{
	CCollider3D::Update(deltaTime);
}

void CColliderSphere3D::PostUpdate(float deltaTime)
{
	CCollider3D::PostUpdate(deltaTime);
	m_Info.center = GetWorldPos();
	float scale = GetWorldScale().x > GetWorldScale().y ? GetWorldScale().x : GetWorldScale().y;
	scale = scale > GetWorldScale().z ? scale : GetWorldScale().z;
	m_Info.radius = m_Radius * scale;
	Vector3	min, max;
	//min = GetWorldPos() - m_Info.radius;
	//max = min + m_Info.radius * 2.f;
	//min -= GetWorldPos();
	//max -= GetWorldPos();
	min = -m_Info.radius;
	max = m_Info.radius;
	m_Transform->SetMin(min);
	m_Transform->SetMax(max);
}

void CColliderSphere3D::Render()
{
	CCollider3D::Render();
}

void CColliderSphere3D::RenderDebug()
{
	CCollider3D::RenderDebug();
	m_WireFrame->SetState();
	Matrix	matScale, matTranslate, matWorld;
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();
	Vector3	scale = GetWorldScale();
	//scale.x = m_Transform->GetRadius();
	//scale.y = m_Transform->GetRadius();
	//scale.z = m_Transform->GetRadius();
	scale.x = m_Info.radius;
	scale.y = m_Info.radius;
	scale.z = m_Info.radius;
	matScale.Scaling(scale);
	matTranslate.Translation(GetWorldPos());
	matWorld = matScale * matTranslate;
	CColliderConstantBuffer* buffer = CResourceManager::GetInst()->GetColliderCBuffer();
	buffer->SetColor(m_Color);
	buffer->SetWVP(matWorld * matView * matProj);
	buffer->UpdateBuffer();
	m_Shader->SetShader();
	m_Mesh->Render();
	m_WireFrame->ResetState();
}

CColliderSphere3D* CColliderSphere3D::Clone() const
{
	return new CColliderSphere3D(*this);
}

void CColliderSphere3D::Save(FILE* file)
{
	CCollider3D::Save(file);
	fwrite(&m_Radius, sizeof(float), 1, file);
}

void CColliderSphere3D::Load(FILE* file)
{
	CCollider3D::Load(file);
	fread(&m_Radius, sizeof(float), 1, file);
}

bool CColliderSphere3D::Collision(CCollider* dest)
{
	Vector3	hitPoint;
	bool	result = false;
	switch (((CCollider3D*)dest)->GetCollider3DType())
	{
	case ECollider3D_Type::Box:
		result = CCollisionManager::GetInst()->CollisionSphere3DToCube(hitPoint, this, (CColliderCube*)dest);
		break;
	case ECollider3D_Type::OBB:
		result = CCollisionManager::GetInst()->CollisionSphere3DToOBB3D(hitPoint, this, (CColliderOBB3D*)dest);
		break;
	case ECollider3D_Type::Sphere:
		result = CCollisionManager::GetInst()->CollisionSphere3DToSphere3D(hitPoint, this, (CColliderSphere3D*)dest);
		break;
	}
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, hitPoint.z);
	m_Result.hitPoint = m_HitPoint;
	dest->SetCollisionResultHitPoint(m_HitPoint);
	return result;
}
