#include "DecalComponent.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../GameObject/GameObject.h"

/*
데칼은 애니메이션메쉬에는 안붙이는게 좋음
	움직이면 데칼영역에서 벗어날수도 있으므로
	발자국같은 스태틱메쉬에만 사용
*/

CDecalComponent::CDecalComponent()
	: m_FadeState(EDecalFadeState::None)
	, m_FadeTime(0.f)
	, m_FadeInTime(0.f)
	, m_FadeOutTime(0.f)
	, m_FadeOutDelayTime(0.f)
	, m_FinishDelete(true)
	, m_Fade(EFadeState::FadeIn)
{
	SetTypeID<CDecalComponent>();
	m_ComponentTypeName = "DecalComponent";
	SetRenderLayerName("Decal");
}

CDecalComponent::CDecalComponent(const CDecalComponent& component)
	: CPrimitiveComponent(component)
	, m_FadeTime(0.f)
	, m_Fade(EFadeState::FadeIn)
{
#ifdef _DEBUG
	m_DebugMesh = component.m_DebugMesh;
	m_DebugMaterial = component.m_DebugMaterial;
#endif // _DEBUG
	m_FadeState = component.m_FadeState;
	m_FadeInTime = component.m_FadeInTime;
	m_FadeOutTime = component.m_FadeOutTime;
	m_FadeOutDelayTime = component.m_FadeOutDelayTime;
	m_FinishDelete = component.m_FinishDelete;
}

CDecalComponent::~CDecalComponent()
{
}

void CDecalComponent::Start()
{
	CPrimitiveComponent::Start();
	switch (m_FadeState)
	{
	case EDecalFadeState::None:
		m_FadeTime = 0.f;
		break;
	case EDecalFadeState::FadeIn:
		m_FadeTime = 0.f;
		break;
	case EDecalFadeState::FadeOut:
		m_FadeTime = m_FadeOutTime;
		break;
	case EDecalFadeState::FadeInOut:
		m_FadeTime = 0.f;
		break;
	}
	m_Fade = EFadeState::FadeIn;
}

bool CDecalComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}
#ifdef _DEBUG
	if (m_Scene)
	{
		m_DebugMesh = (CStaticMesh*)m_Scene->GetResource()->FindMesh("CubeLinePos");
		m_DebugMaterial = m_Scene->GetResource()->FindMaterial("DebugDecal");
	}
	else
	{
		m_DebugMesh = (CStaticMesh*)CResourceManager::GetInst()->FindMesh("CubeLinePos");
		m_DebugMaterial = CResourceManager::GetInst()->FindMaterial("DebugDecal");
	}
#endif // _DEBUG
	SetWorldScale(100.f, 100.f, 100.f);
	SetMesh("CubePos");
	AddMaterial("DefaultDecal");
	return true;
}

void CDecalComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
	switch (m_FadeState)
	{
	case EDecalFadeState::FadeIn:
		m_FadeTime += deltaTime;
		if (m_FadeTime >= m_FadeInTime)
		{
			m_FadeTime = m_FadeInTime;
			if (m_FinishDelete)
			{
				m_Owner->Destroy();
			}
		}
		m_vecMaterial[0]->SetOpacity(m_FadeTime / m_FadeInTime);
		break;
	case EDecalFadeState::FadeOut:
		m_FadeTime -= deltaTime;
		if (m_FadeTime <= 0.f)
		{
			m_FadeTime = 0.f;
			if (m_FinishDelete)
			{
				m_Owner->Destroy();
			}
		}
		m_vecMaterial[0]->SetOpacity(m_FadeTime / m_FadeOutTime);
		break;
	case EDecalFadeState::FadeInOut:
		switch (m_Fade)
		{
		case EFadeState::FadeIn:
			m_FadeTime += deltaTime;
			if (m_FadeTime >= m_FadeInTime)
			{
				m_FadeTime = 0.f;
				m_vecMaterial[0]->SetOpacity(1.f);
				m_Fade = EFadeState::Duration;
			}
			else
			{
				m_vecMaterial[0]->SetOpacity(m_FadeTime / m_FadeInTime);
			}
			break;
		case EFadeState::Duration:
			m_FadeTime += deltaTime;
			if (m_FadeTime >= m_FadeOutDelayTime)
			{
				m_Fade = EFadeState::FadeOut;
				m_FadeTime = m_FadeOutTime;
			}
			break;
		case EFadeState::FadeOut:
			m_FadeTime -= deltaTime;
			if (m_FadeTime <= 0.f)
			{
				m_FadeTime = 0.f;
				if (m_FinishDelete)
				{
					m_Owner->Destroy();
				}
				m_Fade = EFadeState::FadeIn;
			}
			m_vecMaterial[0]->SetOpacity(m_FadeTime / m_FadeOutTime);
			break;
		}
		break;
	}
}

void CDecalComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
}

void CDecalComponent::Render()
{
	CPrimitiveComponent::Render();
}

void CDecalComponent::RenderDebug()
{
	CSceneComponent::Render();
#ifdef _DEBUG
	m_DebugMaterial->SetMaterial();
	m_DebugMesh->Render();
	m_DebugMaterial->ResetMaterial();
#endif // _DEBUG
}

CDecalComponent* CDecalComponent::Clone() const
{
	return new CDecalComponent(*this);
}

void CDecalComponent::Save(FILE* file)
{
	CPrimitiveComponent::Save(file);
}

void CDecalComponent::Load(FILE* file)
{
	CPrimitiveComponent::Load(file);
}
