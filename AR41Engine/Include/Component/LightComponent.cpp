#include "LightComponent.h"
#include "CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/LightManager.h"
#include "../Scene/CameraManager.h"

CLightComponent::CLightComponent()
	: m_CBuffer(nullptr)
	, m_LightInfo{}
{
	SetTypeID<CLightComponent>();
	m_ComponentTypeName = "LightComponent";
	m_LightInfo.Color = Vector4::white;
}

CLightComponent::CLightComponent(const CLightComponent& component)
	: CSceneComponent(component)
{
	m_LightInfo = component.m_LightInfo;
	if (component.m_CBuffer)
	{
		m_CBuffer = component.m_CBuffer->Clone();
	}
}

CLightComponent::~CLightComponent()
{
	SAFE_DELETE(m_CBuffer);
}

void CLightComponent::Destroy()
{
	CSceneComponent::Destroy();
}

void CLightComponent::Start()
{
	CSceneComponent::Start();
	m_CBuffer->SetLightInfo(m_LightInfo);
	m_Scene->GetLightManager()->AddLight(this);
}

bool CLightComponent::Init()
{
	CSceneComponent::Init();
	m_CBuffer = new CLightConstantBuffer;
	m_CBuffer->Init();
	return true;
}

void CLightComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CLightComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
	CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();
	if (m_LightInfo.LightType != (int)ELightType::Direction)
	{
		Vector3	pos = GetWorldPos();
		// 뷰 공간으로 변환한다.
		pos = pos.TransformCoord(camera->GetViewMatrix());
		m_LightInfo.Pos = pos;
		m_CBuffer->SetLightPos(pos);
	}
	if (m_LightInfo.LightType != (int)ELightType::Point)
	{
		Vector3	dir = GetWorldAxis(AXIS::AXIS_Z);
		dir = dir.TransformNormal(camera->GetViewMatrix());
		dir.Normalize();
		m_LightInfo.Dir = dir;
		m_CBuffer->SetLightDir(dir);
	}
}

void CLightComponent::Render()
{
	CSceneComponent::Render();
}

CLightComponent* CLightComponent::Clone() const
{
	return new CLightComponent(*this);
}

void CLightComponent::Save(FILE* file)
{
	CSceneComponent::Save(file);
	fwrite(&m_LightInfo, sizeof(LightCBuffer), 1, file);
}

void CLightComponent::Load(FILE* file)
{
	CSceneComponent::Load(file);
	fread(&m_LightInfo, sizeof(LightCBuffer), 1, file);
}

void CLightComponent::SetShader()
{
	m_CBuffer->UpdateBuffer();
}
