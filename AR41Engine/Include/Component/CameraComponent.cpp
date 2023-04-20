#include "CameraComponent.h"
#include "LightComponent.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/LightManager.h"
#include "../Render/RenderManager.h"
//#include "../Input.h" 하면 이중참조

CCameraComponent::CCameraComponent()
	: m_CameraViewDistance(50000.f)
	, m_CameraType(ECameraType::Camera3D)
	, m_CameraSpeed(50.f)
	, m_CameraHorizon(true)
	, m_CameraVertical(true)
{
	SetTypeID<CCameraComponent>();
	m_ComponentTypeName = "CameraComponent";

	m_FrustumPos[0] = Vector3(-1.f, 1.f, 0.f);
	m_FrustumPos[1] = Vector3(1.f, 1.f, 0.f);
	m_FrustumPos[2] = Vector3(-1.f, -1.f, 0.f);
	m_FrustumPos[3] = Vector3(1.f, -1.f, 0.f);
	m_FrustumPos[4] = Vector3(-1.f, 1.f, 1.f);
	m_FrustumPos[5] = Vector3(1.f, 1.f, 1.f);
	m_FrustumPos[6] = Vector3(-1.f, -1.f, 1.f);
	m_FrustumPos[7] = Vector3(1.f, -1.f, 1.f);
}

CCameraComponent::CCameraComponent(const CCameraComponent& component)
	: CSceneComponent(component)
	, m_CameraSpeed(component.m_CameraSpeed)
	, m_CameraHorizon(component.m_CameraHorizon)
	, m_CameraVertical(component.m_CameraVertical)
{
	m_CameraViewDistance = component.m_CameraViewDistance;
	m_matView = component.m_matView;
	m_matProj = component.m_matProj;
	m_CameraType = component.m_CameraType;
	ComputeProjectionMatrix();
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::ComputeProjectionMatrix()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();
	switch (m_CameraType)
	{
	case ECameraType::Camera2D:
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)RS.width, 0.f,	(float)RS.height, 0.f,
			m_CameraViewDistance);
		break;
	case ECameraType::Camera3D:
		m_matProj = DirectX::XMMatrixPerspectiveFovLH(DegreeToRadian(90.f), 
			(float)RS.width / (float)RS.height, 5.f, m_CameraViewDistance);
		Resolution	shadowRS;
		shadowRS = CRenderManager::GetInst()->GetShadowMapResolution();
		m_matShadowProj = DirectX::XMMatrixPerspectiveFovLH(DegreeToRadian(90.f),
			(float)shadowRS.width / (float)shadowRS.height, 5.f, m_CameraViewDistance);
		break;
	case ECameraType::CameraUI:
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)RS.width, 0.f,	(float)RS.height, 0.f, 
			m_CameraViewDistance);
		break;
	}
}

void CCameraComponent::Destroy()
{
	CSceneComponent::Destroy();
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();
	ComputeProjectionMatrix();
}

bool CCameraComponent::Init()
{
	CSceneComponent::Init();
	return true;
}

void CCameraComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CCameraComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
	/*
		카메라 뷰스페이스 변환 : 월드상의 모든 물체를 카메라가 0, 0, 0으로 이동하는 만큼
		이동시키고 카메라가 바라보는 방향(Z축)이 Z + 방향을 바라볼 수 있게 회전시키는 만큼 이동된
		모든 물체들을 원점 기준으로 그만큼 공전시켜준다.

		그러면 카메라는 뷰공간으로 변환이 일어날 경우 뷰공간에서의 카메라의 x, y, z 축은
		X : 1, 0, 0
		Y : 0, 1, 0
		Z : 0, 0, 1
		이 될것이다.

		카메라의 월드축 : WX, WY, WZ 축이라고 한다.
		WX* 뷰행렬 = 1, 0, 0
		WY * 뷰행렬 = 0, 1, 0
		WZ * 뷰행렬 = 0, 0, 1

		WXX WXY WXZ   11 12 13   1 0 0
		WYX WYY WYZ * 21 22 23 = 0 1 0
		WZX WZY WZZ   31 32 33   0 0 1

		월드축을 이용하여 구성한 행렬을 뷰행렬을 곱해주면 항등행렬이 나온다.
		뷰행렬은 월드축을 이용하여 구성한 행렬의 역행렬이 된다는 것이다.

		월드축을 이용하여 구성한 행렬은 월드축이기 때문에 서로 정직교한다.
		이렇게 서로 정직교하는 축을 이용하여 구성한 행렬을 직교행렬이라고 한다.

		직교행렬의 특징 : 역행렬과 전치행렬이 같다.
		뷰행렬은 월드축을 이용하여 구성한 행렬의 전치행렬과 같다는 의미이다.
		WXX WXY WXZ    WXX WYX WZX
		WYX WYY WYZ = > WXY WYY WZY
		WZX WZY WZZ    WXZ WYZ WZZ

		카메라의 위치 : PX, PY, PZ

		이동행렬
		1   0   0	0
		0   1   0	0
		0   0   1	0
		-PX -PY -PZ 1

		회전행렬
		WXX WYX WZX 0
		WXY WYY WZY 0
		WXZ WYZ WZZ 0
		0   0   0   1

		1    0   0	 0   WXX WYX WZX 0   WXX   WYX    WZX  0
		0    1   0	 0 * WXY WYY WZY 0 = WXY   WYY    WZY  0
		0    0   1	 0	 WXZ WYZ WZZ 0   WXZ   WYZ    WZZ  0
		-PX  -PY -PZ 1	 0   0   0   1  -P.WX -P.WY  -P.WZ 1
		*/
	if (m_CameraType != ECameraType::CameraUI)
	{
		m_matView.Identity();	//항등
		for (int i = 0; i < 3; ++i)
		{
			Vector3	axis = GetWorldAxis((AXIS)i);
			memcpy(&m_matView[i][0], &axis, sizeof(Vector3));
		}
		m_matView.Transpose();	//전치
		Vector3	pos = GetWorldPos();
		for (int i = 0; i < 3; ++i)
		{
			m_matView[3][i] = -pos.Dot(GetWorldAxis((AXIS)i));
		}
		if(m_Scene)
		{
			CLightComponent* globalLight = m_Scene->GetLightManager()->GetGlobalLightComponent();
			Vector3	lightDir[(int)AXIS::AXIS_MAX];
			for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
			{
				lightDir[i] = globalLight->GetWorldAxis((AXIS)i);
			}
			Vector3	lightPos = lightDir[2] * -1000.f;
			m_matShadowView.Identity();
			for (int i = 0; i < 3; ++i)
			{
				memcpy(&m_matShadowView[i][0], &lightDir[i], sizeof(Vector3));
			}
			m_matShadowView.Transpose();
			for (int i = 0; i < 3; ++i)
			{
				m_matShadowView[3][i] = -lightPos.Dot(lightDir[i]);
			}
		}
	}
	// 절두체를 구성하는 6개의 평면정보를 만든다.
	Vector3	pos[8];
	Matrix	matVP = m_matView * m_matProj;
	matVP.Inverse();
	for (int i = 0; i < 8; ++i)
	{
		pos[i] = m_FrustumPos[i].TransformCoord(matVP);
	}
	// Left
	m_FrustumPlane[Frustum_Left] = DirectX::XMPlaneFromPoints(pos[4].Convert(), pos[0].Convert(), pos[2].Convert());
	// Right
	m_FrustumPlane[Frustum_Right] = DirectX::XMPlaneFromPoints(pos[1].Convert(), pos[5].Convert(), pos[7].Convert());
	// Top
	m_FrustumPlane[Frustum_Top] = DirectX::XMPlaneFromPoints(pos[4].Convert(), pos[5].Convert(), pos[1].Convert());
	// Bottom
	m_FrustumPlane[Frustum_Bottom] = DirectX::XMPlaneFromPoints(pos[2].Convert(), pos[3].Convert(), pos[7].Convert());
	// Near
	m_FrustumPlane[Frustum_Near] = DirectX::XMPlaneFromPoints(pos[0].Convert(), pos[1].Convert(), pos[3].Convert());
	// Far
	m_FrustumPlane[Frustum_Far] = DirectX::XMPlaneFromPoints(pos[5].Convert(), pos[4].Convert(), pos[6].Convert());
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

CCameraComponent* CCameraComponent::Clone() const
{
	return new CCameraComponent(*this);
}

void CCameraComponent::Save(FILE* file)
{
	CSceneComponent::Save(file);
	fwrite(&m_CameraType, sizeof(ECameraType), 1,file);
	fwrite(&m_CameraViewDistance, sizeof(float), 1, file);
	fwrite(&m_matView, sizeof(Matrix), 1, file);
	fwrite(&m_matProj, sizeof(Matrix), 1, file);
}

void CCameraComponent::Load(FILE* file)
{
	CSceneComponent::Load(file);
	fread(&m_CameraType, sizeof(ECameraType), 1, file);
	fread(&m_CameraViewDistance, sizeof(float), 1, file);
	fread(&m_matView, sizeof(Matrix), 1, file);
	fread(&m_matProj, sizeof(Matrix), 1, file);
}

bool CCameraComponent::FrustumInPoint(const Vector3& point)
{
	for (int i = 0; i < Frustum_Max; ++i)
	{
		float	dot = DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_FrustumPlane[i].Convert(), point.Convert()));
		if (dot > 0.f)
		{
			return false;
		}
	}
	return true;
}

bool CCameraComponent::FrustumInSphere(const Vector3& center, float radius)
{
	for (int i = 0; i < Frustum_Max; ++i)
	{
		float	dot = DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(m_FrustumPlane[i].Convert(), center.Convert()));
		if (dot > radius)
		{
			return false;
		}
	}
	return true;
}
