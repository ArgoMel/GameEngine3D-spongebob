#pragma once
#include "ConstantBufferData.h"

class CColliderConstantBuffer : public CConstantBufferData
{
private:
	ColliderCBuffer	m_BufferData;
public:
	CColliderConstantBuffer();
	CColliderConstantBuffer(const CColliderConstantBuffer& buffer);
	virtual ~CColliderConstantBuffer();
	void SetColor(const Vector4& color)
	{
		m_BufferData.color = color;
	}
	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
		m_BufferData.matWVP.Transpose();
	}
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CColliderConstantBuffer* Clone();

};

