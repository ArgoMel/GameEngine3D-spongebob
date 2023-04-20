#include "ColliderBox2D.h"
#include "ColliderSphere2D.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"
#include "CameraComponent.h"
#include "../Engine.h"
#include "../CollisionManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CColliderBox2D::CColliderBox2D()
{
	SetTypeID<CColliderBox2D>();
	m_ComponentTypeName = "ColliderBox2D";
	m_Collider2DType = ECollider2D_Type::Box2D;
	m_BoxSize.x = 100.f;
	m_BoxSize.y = 100.f;
}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& component)
	: CCollider2D(component)
{
	m_BoxSize = component.m_BoxSize;
}

CColliderBox2D::~CColliderBox2D()
{
}

void CColliderBox2D::Start()
{
	CCollider2D::Start();
}

bool CColliderBox2D::Init()
{
	if (!CCollider2D::Init())
	{
		return false;
	}
	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box2DLineMesh");
	}
	return true;
}

void CColliderBox2D::Update(float deltaTime)
{
	CCollider2D::Update(deltaTime);
}

void CColliderBox2D::PostUpdate(float deltaTime)
{
	CCollider2D::PostUpdate(deltaTime);
	Vector2	size = m_BoxSize;
	size.x *= GetWorldScale().x;
	size.y *= GetWorldScale().y;
	Vector3	min;
	Vector3	max;
	min.x = GetWorldPos().x - size.x * 0.5f;
	min.y = GetWorldPos().y - size.y * 0.5f;
	max.x = min.x + size.x;
	max.y = min.y + size.y;
	m_Info.left = min.x;
	m_Info.bottom = min.y;
	m_Info.right = max.x;
	m_Info.top = max.y;
	min.x -= GetWorldPos().x;
	min.y -= GetWorldPos().y;
	max.x -= GetWorldPos().x;
	max.y -= GetWorldPos().y;
	m_Transform->SetMin(min);
	m_Transform->SetMax(max);
}

void CColliderBox2D::Render()
{
	CCollider2D::Render();
	Matrix	matScale, matTranslate, matWorld;
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();
	Vector3	scale = GetWorldScale();
	scale.x *= m_BoxSize.x;
	scale.y *= m_BoxSize.y;
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

CColliderBox2D* CColliderBox2D::Clone() const
{
	return new CColliderBox2D(*this);
}

void CColliderBox2D::Save(FILE* file)
{
	CCollider2D::Save(file);
	fwrite(&m_BoxSize, sizeof(Vector2), 1, file);
}

void CColliderBox2D::Load(FILE* file)
{
	CCollider2D::Load(file);
	fread(&m_BoxSize, sizeof(Vector2), 1, file);
	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box2DLineMesh");
	}
}

bool CColliderBox2D::Collision(CCollider* dest)
{
	Vector2	hitPoint;
	bool	result = false;
	switch (((CCollider2D*)dest)->GetCollider2DType())
	{
	case ECollider2D_Type::Box2D:
		result = CCollisionManager::GetInst()->CollisionBox2DToBox2D(hitPoint, this, (CColliderBox2D*)dest);
		break;
	case ECollider2D_Type::OBB2D:
		result = CCollisionManager::GetInst()->CollisionBox2DToOBB2D(hitPoint, this, (CColliderOBB2D*)dest);
		break;
	case ECollider2D_Type::Sphere2D:
		result = CCollisionManager::GetInst()->CollisionBox2DToSphere2D(hitPoint, this, (CColliderSphere2D*)dest);
		break;
	case ECollider2D_Type::Pixel:
		result = CCollisionManager::GetInst()->CollisionBox2DToPixel(hitPoint, this, (CColliderPixel*)dest);
		break;
	}
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
	m_Result.hitPoint = m_HitPoint;
	dest->SetCollisionResultHitPoint(m_HitPoint);
	return result;
}

bool CColliderBox2D::CollisionMouse(const Vector2& mouseWorldPos)
{
	Vector2	hitPoint;
	m_MouseCollision = CCollisionManager::GetInst()->CollisionPointToBox2D(hitPoint, mouseWorldPos, m_Info);
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
	return m_MouseCollision;
}
