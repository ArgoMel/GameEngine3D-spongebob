#include "ColliderSphere2D.h"
#include "CameraComponent.h"
#include "ColliderBox2D.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"
#include "../Engine.h"
#include "../CollisionManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CColliderSphere2D::CColliderSphere2D()
{
	SetTypeID<CColliderSphere2D>();
	m_ComponentTypeName = "ColliderSphere2D";
	m_Collider2DType = ECollider2D_Type::Sphere2D;
	m_Radius = 50.f;
}

CColliderSphere2D::CColliderSphere2D(const CColliderSphere2D& component)
	: CCollider2D(component)
{
	m_Radius = component.m_Radius;
}

CColliderSphere2D::~CColliderSphere2D()
{
}

void CColliderSphere2D::Start()
{
	CCollider2D::Start();
}

bool CColliderSphere2D::Init()
{
	if (!CCollider2D::Init())
	{
		return false;
	}
	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Sphere2DLineMesh");
	}
	return true;
}

void CColliderSphere2D::Update(float deltaTime)
{
	CCollider2D::Update(deltaTime);
}

void CColliderSphere2D::PostUpdate(float deltaTime)
{
	CCollider2D::PostUpdate(deltaTime);
	m_Info.center.x = GetWorldPos().x;
	m_Info.center.y = GetWorldPos().y;
	float scale = GetWorldScale().x > GetWorldScale().y ? GetWorldScale().x : GetWorldScale().y;
	m_Info.radius = m_Radius * scale;
	Vector3	min, max;
	min.x = GetWorldPos().x - m_Info.radius;
	min.y = GetWorldPos().y - m_Info.radius;
	max.x = min.x + m_Info.radius * 2.f;
	max.y = min.y + m_Info.radius * 2.f;
	min.x -= GetWorldPos().x;
	min.y -= GetWorldPos().y;
	max.x -= GetWorldPos().x;
	max.y -= GetWorldPos().y;
	m_Transform->SetMin(min);
	m_Transform->SetMax(max);
}

void CColliderSphere2D::Render()
{
	CCollider2D::Render();
	Matrix	matScale, matTranslate, matWorld;
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();
	Vector3	scale = GetWorldScale();
	scale.x = m_Info.radius;
	scale.y = m_Info.radius;
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

CColliderSphere2D* CColliderSphere2D::Clone() const
{
	return new CColliderSphere2D(*this);
}

void CColliderSphere2D::Save(FILE* file)
{
	CCollider2D::Save(file);
	fwrite(&m_Radius, sizeof(float), 1, file);
}

void CColliderSphere2D::Load(FILE* file)
{
	CCollider2D::Load(file);
	fread(&m_Radius, sizeof(float), 1, file);
}

bool CColliderSphere2D::Collision(CCollider* dest)
{
	Vector2	hitPoint;
	bool	result = false;
	switch (((CCollider2D*)dest)->GetCollider2DType())
	{
	case ECollider2D_Type::Box2D:
		result = CCollisionManager::GetInst()->CollisionBox2DToSphere2D(hitPoint, (CColliderBox2D*)dest, this);
		break;
	case ECollider2D_Type::OBB2D:
		result = CCollisionManager::GetInst()->CollisionSphere2DToOBB2D(hitPoint, this, (CColliderOBB2D*)dest);
		break;
	case ECollider2D_Type::Sphere2D:
		result = CCollisionManager::GetInst()->CollisionSphere2DToSphere2D(hitPoint, this, (CColliderSphere2D*)dest);
		break;
	case ECollider2D_Type::Pixel:
		result = CCollisionManager::GetInst()->CollisionSphere2DToPixel(hitPoint, this, (CColliderPixel*)dest);
		break;
	}
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
	m_Result.hitPoint = m_HitPoint;
	dest->SetCollisionResultHitPoint(m_HitPoint);
	return result;
}

bool CColliderSphere2D::CollisionMouse(const Vector2& mouseWorldPos)
{
	Vector2	hitPoint;
	m_MouseCollision = CCollisionManager::GetInst()->CollisionPointToSphere2D(hitPoint, mouseWorldPos, m_Info);
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
	return m_MouseCollision;
}
