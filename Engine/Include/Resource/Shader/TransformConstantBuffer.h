#pragma once
#include "ConstantBufferData.h"
class CTransformConstantBuffer :public CConstantBufferData
{
private:
	TransformCBuffer	m_BufferData;
public:
	void SetWorldMatrix(const Matrix& matWorld)
	{
		m_BufferData.matWorld = matWorld;
	}
	void SetViewMatrix(const Matrix& matView)
	{
		m_BufferData.matView = matView;
	}
	void SetProjMatrix(const Matrix& matProj)
	{
		m_BufferData.matProj = matProj;
		m_BufferData.proj11 = matProj._11;
		m_BufferData.proj22 = matProj._22;
	}
	void SetPivot(const Vector3& pivot)
	{
		m_BufferData.pivot = pivot;
	}
	void SetMeshSize(const Vector3& meshSize)
	{
		m_BufferData.meshSize = meshSize;
	}
	CTransformConstantBuffer();
	CTransformConstantBuffer(const CTransformConstantBuffer& buffer);
	virtual ~CTransformConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CTransformConstantBuffer* Clone();
};

