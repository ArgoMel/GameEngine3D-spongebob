#include "TransformConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer()
	: m_BufferData{}
{
}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& buffer)
	: CConstantBufferData(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{
}

bool CTransformConstantBuffer::Init()
{
	SetConstantBuffer("Transform");
	return true;
}

void CTransformConstantBuffer::UpdateBuffer()
{
	m_BufferData.matWV = m_BufferData.matWorld * m_BufferData.matView;
	m_BufferData.matWVP = m_BufferData.matWV * m_BufferData.matProj;

	m_BufferData.matInvWorld = m_BufferData.matWorld;
	m_BufferData.matInvWorld.Inverse();
	m_BufferData.matInvView = m_BufferData.matView;
	m_BufferData.matInvView.Inverse();
	m_BufferData.matInvProj = m_BufferData.matProj;
	m_BufferData.matInvProj.Inverse();
	m_BufferData.matInvWVP = m_BufferData.matWVP;
	m_BufferData.matInvWVP.Inverse();
	m_BufferData.matInvVP = m_BufferData.matView * m_BufferData.matProj;
	m_BufferData.matInvVP.Inverse();
	// Shader에서 사용할 수 있게 하기 위해서 전치행렬을 만들어서 넘겨준다.
	m_BufferData.matWorld.Transpose();
	m_BufferData.matView.Transpose();
	m_BufferData.matProj.Transpose();
	m_BufferData.matWV.Transpose();
	m_BufferData.matWVP.Transpose();
	m_BufferData.matInvWorld.Transpose();
	m_BufferData.matInvView.Transpose();
	m_BufferData.matInvProj.Transpose();
	m_BufferData.matInvVP.Transpose();
	m_BufferData.matInvWVP.Transpose();

	m_Buffer->UpdateBuffer(&m_BufferData);
}

CTransformConstantBuffer* CTransformConstantBuffer::Clone()
{
	return new CTransformConstantBuffer(*this);
}
