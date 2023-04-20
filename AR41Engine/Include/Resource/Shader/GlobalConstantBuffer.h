#pragma once
#include "ConstantBufferData.h"

class CGlobalConstantBuffer : public CConstantBufferData
{
private:
	GlobalCBuffer	m_BufferData;
public:
	void SetDeltaTime(float deltaTime)
	{
		m_BufferData.deltaTime = deltaTime;
	}
	void SetAccTime(float accTime)
	{
		m_BufferData.accTime = accTime;
	}
	void SetResolution(float width, float height)
	{
		m_BufferData.resolution = Vector2(width, height);
	}
	void SetNoiseResolution(float width, float height)
	{
		m_BufferData.noiseResolution = Vector2(width, height);
	}
	void SetShadowMapResolution(float width, float height)
	{
		m_BufferData.shadowMapResolution = Vector2(width, height);
	}
	void SetCameraAxisX(const Vector3& axis)
	{
		m_BufferData.cameraAxisX = axis;
	}
	void SetCameraAxisY(const Vector3& axis)
	{
		m_BufferData.cameraAxisY = axis;
	}
	CGlobalConstantBuffer();
	CGlobalConstantBuffer(const CGlobalConstantBuffer& buffer);
	virtual ~CGlobalConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CGlobalConstantBuffer* Clone();
};

