#pragma once
#include "ConstantBufferData.h"

class CShadowConstantBuffer : public CConstantBufferData
{
private:
	ShadowCBuffer	m_BufferData;
public:
	ShadowCBuffer GetShadowCBuffer()
	{
		return m_BufferData;
	}
	void SetShadowVP(const Matrix& matShadowVP)
	{
		m_BufferData.matShadowVP = matShadowVP;
		m_BufferData.matShadowInvVP = matShadowVP;
		m_BufferData.matShadowInvVP.Inverse();
	}
	void SetShadowResolution(const Vector2& RS)
	{
		m_BufferData.resolution = RS;
	}
	void SetShadowBias(float bias)
	{
		m_BufferData.bias = bias;
	}
	CShadowConstantBuffer();
	CShadowConstantBuffer(const CShadowConstantBuffer& buffer);
	virtual ~CShadowConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CShadowConstantBuffer* Clone();
};

