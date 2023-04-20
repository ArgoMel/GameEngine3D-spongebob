#include "CameraComponent.h"
#include "LightComponent.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/LightManager.h"
#include "../Render/RenderManager.h"
//#include "../Input.h" �ϸ� ��������

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
		ī�޶� �佺���̽� ��ȯ : ������� ��� ��ü�� ī�޶� 0, 0, 0���� �̵��ϴ� ��ŭ
		�̵���Ű�� ī�޶� �ٶ󺸴� ����(Z��)�� Z + ������ �ٶ� �� �ְ� ȸ����Ű�� ��ŭ �̵���
		��� ��ü���� ���� �������� �׸�ŭ ���������ش�.

		�׷��� ī�޶�� ��������� ��ȯ�� �Ͼ ��� ����������� ī�޶��� x, y, z ����
		X : 1, 0, 0
		Y : 0, 1, 0
		Z : 0, 0, 1
		�� �ɰ��̴�.

		ī�޶��� ������ : WX, WY, WZ ���̶�� �Ѵ�.
		WX* ����� = 1, 0, 0
		WY * ����� = 0, 1, 0
		WZ * ����� = 0, 0, 1

		WXX WXY WXZ   11 12 13   1 0 0
		WYX WYY WYZ * 21 22 23 = 0 1 0
		WZX WZY WZZ   31 32 33   0 0 1

		�������� �̿��Ͽ� ������ ����� ������� �����ָ� �׵������ ���´�.
		������� �������� �̿��Ͽ� ������ ����� ������� �ȴٴ� ���̴�.

		�������� �̿��Ͽ� ������ ����� �������̱� ������ ���� �������Ѵ�.
		�̷��� ���� �������ϴ� ���� �̿��Ͽ� ������ ����� ��������̶�� �Ѵ�.

		��������� Ư¡ : ����İ� ��ġ����� ����.
		������� �������� �̿��Ͽ� ������ ����� ��ġ��İ� ���ٴ� �ǹ��̴�.
		WXX WXY WXZ    WXX WYX WZX
		WYX WYY WYZ = > WXY WYY WZY
		WZX WZY WZZ    WXZ WYZ WZZ

		ī�޶��� ��ġ : PX, PY, PZ

		�̵����
		1   0   0	0
		0   1   0	0
		0   0   1	0
		-PX -PY -PZ 1

		ȸ�����
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
		m_matView.Identity();	//�׵�
		for (int i = 0; i < 3; ++i)
		{
			Vector3	axis = GetWorldAxis((AXIS)i);
			memcpy(&m_matView[i][0], &axis, sizeof(Vector3));
		}
		m_matView.Transpose();	//��ġ
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
	// ����ü�� �����ϴ� 6���� ��������� �����.
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
