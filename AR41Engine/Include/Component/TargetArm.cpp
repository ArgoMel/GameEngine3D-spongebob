#include "TargetArm.h"
#include "../Input.h"

CTargetArm::CTargetArm()
	: m_TargetDistance(300.f)
	, m_TargetDistanceAxis(AXIS::AXIS_Z)
	, m_WheelZoomInOutEnable(true)
	, m_WheelSensitivity(1.f)
	, m_WheelZoomMin(50.f)
	, m_WheelZoomMax(5000.f)
	, m_WheelTickMove(30.f)
{
	SetTypeID<CTargetArm>();
	m_ComponentTypeName = "TargetArm";
}

CTargetArm::CTargetArm(const CTargetArm& component)
	: CSceneComponent(component)
{
	m_TargetDistance = component.m_TargetDistance;
	m_TargetOffset = component.m_TargetOffset;
	m_TargetDistanceAxis = component.m_TargetDistanceAxis;
	m_WheelZoomInOutEnable = component.m_WheelZoomInOutEnable;
	m_WheelSensitivity = component.m_WheelSensitivity;
	m_WheelZoomMin = component.m_WheelZoomMin;
	m_WheelZoomMax = component.m_WheelZoomMax;
	m_WheelTickMove = component.m_WheelTickMove;
}

CTargetArm::~CTargetArm()
{
}

void CTargetArm::SetTargetDistance(float distance)
{
	m_TargetDistance = distance;
}

void CTargetArm::Destroy()
{
	CSceneComponent::Destroy();
}

void CTargetArm::Start()
{
	CSceneComponent::Start();
	SetRelativePosition(m_TargetOffset);
	auto	iter = m_vecChild.begin();
	auto	iterEnd = m_vecChild.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetRelativePosition(GetWorldAxis(m_TargetDistanceAxis) * -1.f * m_TargetDistance);
	}
}

bool CTargetArm::Init()
{
	CSceneComponent::Init();
	return true;
}

void CTargetArm::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
	if (m_WheelZoomInOutEnable)
	{
		short	wheel = CInput::GetInst()->GetMouseWheel();
		if (wheel != 0)
		{
			float	move = m_WheelSensitivity * m_WheelTickMove * wheel * -1.f;
			m_TargetDistance += move;
			if (m_TargetDistance < m_WheelZoomMin)
			{
				m_TargetDistance = m_WheelZoomMin;
			}
			else if (m_TargetDistance > m_WheelZoomMax)
			{
				m_TargetDistance = m_WheelZoomMax;
			}
			auto	iter = m_vecChild.begin();
			auto	iterEnd = m_vecChild.end();
			for (; iter != iterEnd; ++iter)
			{
				(*iter)->SetRelativePosition(Vector3(0.f, 0.f, -m_TargetDistance));
			}
		}
	}
}

void CTargetArm::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CTargetArm::Render()
{
	CSceneComponent::Render();
}

CTargetArm* CTargetArm::Clone() const
{
	return new CTargetArm(*this);
}

void CTargetArm::Save(FILE* file)
{
	CSceneComponent::Save(file);
	fwrite(&m_TargetOffset, sizeof(Vector3), 1, file);
	fwrite(&m_TargetDistance, sizeof(float), 1, file);
	fwrite(&m_TargetDistanceAxis, sizeof(AXIS), 1, file);
}

void CTargetArm::Load(FILE* file)
{
	CSceneComponent::Load(file);
	fread(&m_TargetOffset, sizeof(Vector3), 1, file);
	fread(&m_TargetDistance, sizeof(float), 1, file);
	fread(&m_TargetDistanceAxis, sizeof(AXIS), 1, file);
}
