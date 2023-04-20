#pragma once
#include "SceneComponent.h"
class CTargetArm : public CSceneComponent
{
protected:
	friend class CGameObject;
	friend class CScene;
	Vector3		m_TargetOffset;
	AXIS		m_TargetDistanceAxis;
	float		m_TargetDistance;
	float		m_WheelSensitivity;	//감도 0~1
	float		m_WheelTickMove;	//델타타임 대용
	float		m_WheelZoomMin;
	float		m_WheelZoomMax;
	bool		m_WheelZoomInOutEnable;
	CTargetArm();
	CTargetArm(const CTargetArm& component);
	virtual ~CTargetArm();
public:
	Vector3 GetTargetOffset()	const
	{
		return m_TargetOffset;
	}
	AXIS GetTargetDistanceAxis()	const
	{
		return m_TargetDistanceAxis;
	}
	float GetTargetDistance()	const
	{
		return m_TargetDistance;
	}
	float GetWheelSensitivity()	const
	{
		return m_WheelSensitivity;
	}
	float GetWheelTickMove()	const
	{
		return m_WheelTickMove;
	}
	float GetWheelZoomMin()	const
	{
		return m_WheelZoomMin;
	}
	float GetWheelZoomMax()	const
	{
		return m_WheelZoomMax;
	}
	bool GetWheelZoomInOutEnable()	const
	{
		return m_WheelZoomInOutEnable;
	}
	void SetTargetOffset(const Vector3& offset)
	{
		m_TargetOffset = offset;
	}
	void SetTargetOffset(float x, float y, float z)
	{
		m_TargetOffset = Vector3(x, y, z);
	}
	void SetTargetDistanceAxis(AXIS axis)
	{
		m_TargetDistanceAxis = axis;
	}
	void SetMouseWheelSensitivity(float sensitivity)
	{
		m_WheelSensitivity = sensitivity;
	}
	void SetWheelTickMove(float Move)
	{
		m_WheelTickMove = Move;
	}
	void SetZoomMin(float min)
	{
		if (min > m_WheelZoomMax)
		{
			min = m_WheelZoomMax - 100.f;	//100.f는 약 1미터로 약속
		}
		m_WheelZoomMin = min < 50.f ? 50.f : min;
	}
	void SetZoomMax(float max)
	{
		if (max < m_WheelZoomMin)
		{
			max = m_WheelZoomMin + 100.f;
		}
		m_WheelZoomMax = max > 5000.f ? 5000.f : max;
	}
	void SetWheelZoomInOutEnable(bool enable)
	{
		m_WheelZoomInOutEnable = enable;
	}
	void SetTargetDistance(float distance);
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CTargetArm* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
};

