#pragma once
#include "ConstantBufferData.h"

class CLightConstantBuffer : public CConstantBufferData
{
private:
	LightCBuffer	m_BufferData;
public:
	void SetLightInfo(const LightCBuffer& info)
	{
		m_BufferData = info;
	}
	void SetLightType(ELightType type)
	{
		m_BufferData.LightType = (int)type;
	}
	void SetLightColor(const Vector4& color)
	{
		m_BufferData.Color = color;
	}
	void SetLightPos(const Vector3& pos)
	{
		m_BufferData.Pos = pos;
	}
	void SetLightDir(const Vector3& dir)
	{
		m_BufferData.Dir = dir;
	}
	void SetLightDistance(float distance)
	{
		m_BufferData.Distance = distance;
	}
	void SetLightAngleIn(float angleIn)
	{
		m_BufferData.AngleIn = angleIn;
	}
	void SetLightAngleOut(float angleOut)
	{
		m_BufferData.AngleOut = angleOut;
	}
	void SetLightAtt1(float att1)
	{
		m_BufferData.Att1 = att1;
	}
	void SetLightAtt2(float att2)
	{
		m_BufferData.Att2 = att2;
	}
	void SetLightAtt3(float att3)
	{
		m_BufferData.Att3 = att3;
	}
	void SetLightIntensity(float intensity)
	{
		m_BufferData.Intensity = intensity;
	}
	CLightConstantBuffer();
	CLightConstantBuffer(const CLightConstantBuffer& buffer);
	virtual ~CLightConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CLightConstantBuffer* Clone();
};

