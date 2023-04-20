#include "Collider3D.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Engine.h"

CCollider3D::CCollider3D()
	: m_WireFrame(nullptr)
{
	SetTypeID<CCollider3D>();
	m_ComponentTypeName = "Collider3D";
	m_ColliderType = ECollider_Type::Collider3D;
}

CCollider3D::CCollider3D(const CCollider3D& component)
	: CCollider(component)
	, m_WireFrame(component.m_WireFrame)
{
	m_Collider3DType = component.m_Collider3DType;
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::Start()
{
	CCollider::Start();
}

bool CCollider3D::Init()
{
	if (!CCollider::Init())
	{
		return false;
	}
	m_WireFrame = CRenderManager::GetInst()->FindRenderState<CRenderState>("WireFrame");
	return true;
}

void CCollider3D::Update(float deltaTime)
{
	CCollider::Update(deltaTime);
}

void CCollider3D::PostUpdate(float deltaTime)
{
	CCollider::PostUpdate(deltaTime);
}

void CCollider3D::Render()
{
	CCollider::Render();
}

void CCollider3D::RenderDebug()
{
	CCollider::Render();
}

void CCollider3D::Save(FILE* file)
{
	CCollider::Save(file);
	fwrite(&m_Collider3DType, sizeof(ECollider3D_Type), 1, file);
}

void CCollider3D::Load(FILE* file)
{
	CCollider::Load(file);
	fread(&m_Collider3DType, sizeof(ECollider3D_Type), 1, file);
}
