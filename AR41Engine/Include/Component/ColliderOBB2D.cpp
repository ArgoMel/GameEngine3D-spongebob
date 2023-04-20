#include "ColliderOBB2D.h"
#include "ColliderBox2D.h"
#include "ColliderSphere2D.h"
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
#include "../Engine.h"

CColliderOBB2D::CColliderOBB2D()
{
	SetTypeID<CColliderOBB2D>();
	m_ComponentTypeName = "ColliderOBB2D";
	m_Collider2DType = ECollider2D_Type::OBB2D;
	m_BoxHalfSize.x = 50.f;
	m_BoxHalfSize.y = 50.f;
}

CColliderOBB2D::CColliderOBB2D(const CColliderOBB2D& component)
	: CCollider2D(component)
{
	m_BoxHalfSize = component.m_BoxHalfSize;
}

CColliderOBB2D::~CColliderOBB2D()
{
}

void CColliderOBB2D::Start()
{
	CCollider2D::Start();
}

bool CColliderOBB2D::Init()
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

void CColliderOBB2D::Update(float deltaTime)
{
	CCollider2D::Update(deltaTime);
}

void CColliderOBB2D::PostUpdate(float deltaTime)
{
	CCollider2D::PostUpdate(deltaTime);
	Vector2	size = m_BoxHalfSize;
	size.x *=GetWorldScale().x;
	size.y *=GetWorldScale().y;
	m_Info.center.x = GetWorldPos().x;
	m_Info.center.y = GetWorldPos().y;
	for (int i = 0; i < (int)AXIS2D::AXIS2D_MAX; ++i)
	{
		Vector3	axis = GetWorldAxis((AXIS)i);
		m_Info.axis[i] = Vector2(axis.x, axis.y);
		m_Info.length[i] = size[i];
	}
	Vector2	pos[4];
	pos[0] = m_Info.center - m_Info.axis[(int)AXIS2D::AXIS2D_X] * m_Info.length[(int)AXIS2D::AXIS2D_X] 
						   + m_Info.axis[(int)AXIS2D::AXIS2D_Y] * m_Info.length[(int)AXIS2D::AXIS2D_Y];
	pos[1] = m_Info.center + m_Info.axis[(int)AXIS2D::AXIS2D_X] * m_Info.length[(int)AXIS2D::AXIS2D_X] 
						   + m_Info.axis[(int)AXIS2D::AXIS2D_Y] * m_Info.length[(int)AXIS2D::AXIS2D_Y];
	pos[2] = m_Info.center - m_Info.axis[(int)AXIS2D::AXIS2D_X] * m_Info.length[(int)AXIS2D::AXIS2D_X] 
						   - m_Info.axis[(int)AXIS2D::AXIS2D_Y] * m_Info.length[(int)AXIS2D::AXIS2D_Y];
	pos[3] = m_Info.center + m_Info.axis[(int)AXIS2D::AXIS2D_X] * m_Info.length[(int)AXIS2D::AXIS2D_X] 
						   - m_Info.axis[(int)AXIS2D::AXIS2D_Y] * m_Info.length[(int)AXIS2D::AXIS2D_Y];
	Vector3	min, max;
	min.x = pos[0].x;
	min.y = pos[0].y;
	max.x = pos[0].x;
	max.y = pos[0].y;
	for (int i = 1; i < 4; ++i)
	{
		min.x = min.x > pos[i].x ? pos[i].x : min.x;
		min.y = min.y > pos[i].y ? pos[i].y : min.y;
		max.x = max.x < pos[i].x ? pos[i].x : max.x;
		max.y = max.y < pos[i].y ? pos[i].y : max.y;
	}
	min.x -= GetWorldPos().x;
	min.y -= GetWorldPos().y;
	max.x -= GetWorldPos().x;
	max.y -= GetWorldPos().y;
	m_Transform->SetMin(min);
	m_Transform->SetMax(max);
}

void CColliderOBB2D::Render()
{
	CCollider2D::Render();
	Matrix	matScale, matRot, matTranslate, matWorld;
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();
	Vector3	scale = Vector3(m_Info.length[0] * 2.f, m_Info.length[1] * 2.f, 1.f);
	matScale.Scaling(scale);
	matRot.Rotation(GetWorldRot());
	matTranslate.Translation(GetWorldPos());
	matWorld = matScale * matRot * matTranslate;
	CColliderConstantBuffer* buffer = CResourceManager::GetInst()->GetColliderCBuffer();
	buffer->SetColor(m_Color);
	buffer->SetWVP(matWorld * matView * matProj);
	buffer->UpdateBuffer();
	m_Shader->SetShader();
	m_Mesh->Render();
}

CColliderOBB2D* CColliderOBB2D::Clone() const
{
	return new CColliderOBB2D(*this);
}

void CColliderOBB2D::Save(FILE* file)
{
	CCollider2D::Save(file);
	fwrite(&m_BoxHalfSize, sizeof(Vector2), 1, file);
}

void CColliderOBB2D::Load(FILE* file)
{
	CCollider2D::Load(file);
	fread(&m_BoxHalfSize, sizeof(Vector2), 1, file);
	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box2DLineMesh");
	}
}

bool CColliderOBB2D::Collision(CCollider* dest)
{
	Vector2	hitPoint;
	bool	result = false;
	switch (((CCollider2D*)dest)->GetCollider2DType())
	{
	case ECollider2D_Type::Box2D:
		result = CCollisionManager::GetInst()->CollisionBox2DToOBB2D(hitPoint, (CColliderBox2D*)dest, this);
		break;
	case ECollider2D_Type::OBB2D:
		result = CCollisionManager::GetInst()->CollisionOBB2DToOBB2D(hitPoint, this, (CColliderOBB2D*)dest);
		break;
	case ECollider2D_Type::Sphere2D:
		result = CCollisionManager::GetInst()->CollisionSphere2DToOBB2D(hitPoint, (CColliderSphere2D*)dest, this);
		break;
	case ECollider2D_Type::Pixel:
		result = CCollisionManager::GetInst()->CollisionOBB2DToPixel(hitPoint, this, (CColliderPixel*)dest);
		break;
	}
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
	m_Result.hitPoint = m_HitPoint;
	dest->SetCollisionResultHitPoint(m_HitPoint);
	return result;
}

bool CColliderOBB2D::CollisionMouse(const Vector2& mouseWorldPos)
{
	Vector2	hitPoint;
	m_MouseCollision = CCollisionManager::GetInst()->CollisionPointToOBB2D(hitPoint, mouseWorldPos, m_Info);
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
	return m_MouseCollision;
}
