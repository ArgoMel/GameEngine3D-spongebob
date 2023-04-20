#pragma once
#include "SceneComponent.h"

enum EFrustumPlane
{
	Frustum_Left,
	Frustum_Right,
	Frustum_Top,
	Frustum_Bottom,
	Frustum_Near,
	Frustum_Far,
	Frustum_Max
};

class CCameraComponent : public CSceneComponent
{
protected:
	friend class CCameraManager;
	friend class CGameObject;
	friend class CScene;
	ECameraType	m_CameraType;
	Matrix		m_matView;
	Matrix		m_matProj;
	Matrix		m_matShadowView;
	Matrix		m_matShadowProj;
	Vector4		m_FrustumPlane[Frustum_Max];
	Vector3		m_FrustumPos[8];
	float		m_CameraViewDistance;
	float		m_CameraSpeed;
	bool		m_CameraHorizon;
	bool		m_CameraVertical;
	CCameraComponent();
	CCameraComponent(const CCameraComponent& component);
	virtual ~CCameraComponent();
public:
	const Matrix& GetViewMatrix()	const
	{
		return m_matView;
	}
	const Matrix& GetProjMatrix()	const
	{
		return m_matProj;
	}
	const Matrix& GetShadowViewMatrix()	const
	{
		return m_matShadowView;
	}
	const Matrix& GetShadowProjMatrix()	const
	{
		return m_matShadowProj;
	}
	ECameraType GetCameraType()	const
	{
		return m_CameraType;
	}
	float GetCameraViewDistance()	const
	{
		return m_CameraViewDistance;
	}
	float GetCameraSpeed()	const
	{
		return m_CameraSpeed;
	}
	bool GetCameraHorizon()	const
	{
		return m_CameraHorizon;
	}
	bool GetCameraVertical()	const
	{
		return m_CameraVertical;
	}
	void SetCameraType(ECameraType type)
	{
		m_CameraType = type;
		ComputeProjectionMatrix();
	}
	void SetCameraViewDistance(float distance)
	{
		m_CameraViewDistance = distance;
		ComputeProjectionMatrix();
	}
	void SetCameraSpeed(float f)
	{
		m_CameraSpeed = f;
	}
	void SetCameraHorizon(bool b)
	{
		m_CameraHorizon = b;
	}
	void SetCameraVertical(bool b)
	{
		m_CameraVertical = b;
	}
	void ComputeProjectionMatrix();
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CCameraComponent* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	bool FrustumInPoint(const Vector3& point);
	bool FrustumInSphere(const Vector3& center, float radius);
};

