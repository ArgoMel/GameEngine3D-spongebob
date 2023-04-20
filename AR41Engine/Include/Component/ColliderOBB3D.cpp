#include "ColliderOBB3D.h"
#include "CameraComponent.h"
#include "ColliderCube.h"
#include "ColliderSphere3D.h"
#include "ColliderPixel.h"
#include "../Engine.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../CollisionManager.h"

CColliderOBB3D::CColliderOBB3D()
{
	SetTypeID<CColliderOBB3D>();
	SetRenderLayerName("Collider");
	m_ComponentTypeName = "ColliderOBB3D";
	m_Collider3DType = ECollider3D_Type::OBB;
	m_BoxHalfSize.x = 100.f;
	m_BoxHalfSize.y = 100.f;
	m_BoxHalfSize.z = 100.f;
}

CColliderOBB3D::CColliderOBB3D(const CColliderOBB3D& component) 
	: CCollider3D(component)
{
	m_BoxHalfSize = component.m_BoxHalfSize;
}

CColliderOBB3D::~CColliderOBB3D()
{
}

void CColliderOBB3D::Start()
{
	CCollider3D::Start();
}

bool CColliderOBB3D::Init()
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

void CColliderOBB3D::Update(float deltaTime)
{
	CCollider3D::Update(deltaTime);
}

void CColliderOBB3D::PostUpdate(float deltaTime)
{
	CCollider3D::PostUpdate(deltaTime);
	Vector3	size = m_BoxHalfSize;
	size.x *= GetWorldScale().x;
	size.y *= GetWorldScale().y;
	size.z *= GetWorldScale().z;
	m_Info.center.x = GetWorldPos().x;
	m_Info.center.y = GetWorldPos().y;
	m_Info.center.z = GetWorldPos().z;
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		Vector3	axis = GetWorldAxis((AXIS)i);
		m_Info.axis[i] = Vector3(axis.x, axis.y, axis.z);
		m_Info.length[i] = size[i];
	}
	Vector3	pos[8];
	//-++
	//+-+
	//++-
	//--+
	//-+-
	//+--
	//+++
	//---
	pos[0] = m_Info.center - m_Info.axis[(int)AXIS::AXIS_X] * m_Info.length[(int)AXIS::AXIS_X] 
						   + m_Info.axis[(int)AXIS::AXIS_Y] * m_Info.length[(int)AXIS::AXIS_Y] 
						   + m_Info.axis[(int)AXIS::AXIS_Z] * m_Info.length[(int)AXIS::AXIS_Z];
	pos[1] = m_Info.center + m_Info.axis[(int)AXIS::AXIS_X] * m_Info.length[(int)AXIS::AXIS_X] 
						   - m_Info.axis[(int)AXIS::AXIS_Y] * m_Info.length[(int)AXIS::AXIS_Y] 
						   + m_Info.axis[(int)AXIS::AXIS_Z] * m_Info.length[(int)AXIS::AXIS_Z];
	pos[2] = m_Info.center + m_Info.axis[(int)AXIS::AXIS_X] * m_Info.length[(int)AXIS::AXIS_X] 
						   + m_Info.axis[(int)AXIS::AXIS_Y] * m_Info.length[(int)AXIS::AXIS_Y] 
						   - m_Info.axis[(int)AXIS::AXIS_Z] * m_Info.length[(int)AXIS::AXIS_Z];
	pos[3] = m_Info.center - m_Info.axis[(int)AXIS::AXIS_X] * m_Info.length[(int)AXIS::AXIS_X] 
						   - m_Info.axis[(int)AXIS::AXIS_Y] * m_Info.length[(int)AXIS::AXIS_Y] 
						   + m_Info.axis[(int)AXIS::AXIS_Z] * m_Info.length[(int)AXIS::AXIS_Z];
	pos[4] = m_Info.center - m_Info.axis[(int)AXIS::AXIS_X] * m_Info.length[(int)AXIS::AXIS_X] 
						   + m_Info.axis[(int)AXIS::AXIS_Y] * m_Info.length[(int)AXIS::AXIS_Y] 
						   - m_Info.axis[(int)AXIS::AXIS_Z] * m_Info.length[(int)AXIS::AXIS_Z];
	pos[5] = m_Info.center + m_Info.axis[(int)AXIS::AXIS_X] * m_Info.length[(int)AXIS::AXIS_X] 
						   - m_Info.axis[(int)AXIS::AXIS_Y] * m_Info.length[(int)AXIS::AXIS_Y] 
						   - m_Info.axis[(int)AXIS::AXIS_Z] * m_Info.length[(int)AXIS::AXIS_Z];
	pos[6] = m_Info.center + m_Info.axis[(int)AXIS::AXIS_X] * m_Info.length[(int)AXIS::AXIS_X] 
						   + m_Info.axis[(int)AXIS::AXIS_Y] * m_Info.length[(int)AXIS::AXIS_Y] 
						   + m_Info.axis[(int)AXIS::AXIS_Z] * m_Info.length[(int)AXIS::AXIS_Z];
	pos[7] = m_Info.center - m_Info.axis[(int)AXIS::AXIS_X] * m_Info.length[(int)AXIS::AXIS_X] 
						   - m_Info.axis[(int)AXIS::AXIS_Y] * m_Info.length[(int)AXIS::AXIS_Y] 
						   - m_Info.axis[(int)AXIS::AXIS_Z] * m_Info.length[(int)AXIS::AXIS_Z];
	Vector3	min;
	Vector3	max;
	min.x = pos[0].x;
	min.y = pos[0].y;
	min.z = pos[0].z;
	max.x = pos[0].x;
	max.y = pos[0].y;
	max.z = pos[0].z;
	for (int i = 1; i < 8; ++i)
	{
		min.x = min.x > pos[i].x ? pos[i].x : min.x;
		min.y = min.y > pos[i].y ? pos[i].y : min.y;
		min.z = min.z > pos[i].z ? pos[i].z : min.z;
		max.x = max.x < pos[i].x ? pos[i].x : max.x;
		max.y = max.y < pos[i].y ? pos[i].y : max.y;
		max.z = max.z < pos[i].z ? pos[i].z : max.z;
	}
	min.x -= GetWorldPos().x;
	min.y -= GetWorldPos().y;
	min.z -= GetWorldPos().z;
	max.x -= GetWorldPos().x;
	max.y -= GetWorldPos().y;
	max.z -= GetWorldPos().z;
	m_Transform->SetMin(min);
	m_Transform->SetMax(max);
}

void CColliderOBB3D::Render()
{
	CCollider3D::Render();
}

void CColliderOBB3D::RenderDebug()
{
	CCollider3D::RenderDebug();
	Matrix	matScale, matRot, matTranslate, matWorld;
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();
	Vector3	scale = Vector3(m_Info.length[0] * 2.f, m_Info.length[1] * 2.f, m_Info.length[2] * 2.f);
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

CColliderOBB3D* CColliderOBB3D::Clone() const
{
	return new CColliderOBB3D(*this);
}

void CColliderOBB3D::Save(FILE* File)
{
	CCollider3D::Save(File);

	fwrite(&m_BoxHalfSize, sizeof(Vector3), 1, File);
}

void CColliderOBB3D::Load(FILE* File)
{
	CCollider3D::Load(File);

	fread(&m_BoxHalfSize, sizeof(Vector3), 1, File);
}

bool CColliderOBB3D::Collision(CCollider* Dest)
{
	Vector3	HitPoint;
	bool Result = false;

	switch (((CCollider3D*)Dest)->GetCollider3DType())
	{
	case ECollider3D_Type::Box:
		Result = CCollisionManager::GetInst()->CollisionOBB3DToCube(HitPoint, this, (CColliderCube*)Dest);
		break;
	case ECollider3D_Type::OBB:
		Result = CCollisionManager::GetInst()->CollisionOBB3DToOBB3D(HitPoint, this, (CColliderOBB3D*)Dest);
		break;
	case ECollider3D_Type::Sphere:
		Result = CCollisionManager::GetInst()->CollisionSphere3DToOBB3D(HitPoint, (CColliderSphere3D*)Dest, this);
		break;

	}

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
	m_Result.hitPoint = m_HitPoint;
	Dest->SetCollisionResultHitPoint(m_HitPoint);
	return Result;
}

