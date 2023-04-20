#include "ShadowConstantBuffer.h"

CShadowConstantBuffer::CShadowConstantBuffer()
	: m_BufferData{}
{
	m_BufferData.bias = 0.1f;
}

CShadowConstantBuffer::CShadowConstantBuffer(const CShadowConstantBuffer& buffer)
	: CConstantBufferData(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CShadowConstantBuffer::~CShadowConstantBuffer()
{
}

bool CShadowConstantBuffer::Init()
{
	SetConstantBuffer("Shadow");
	return false;
}

void CShadowConstantBuffer::UpdateBuffer()
{
	m_BufferData.matShadowVP.Transpose();
	m_BufferData.matShadowInvVP.Transpose();
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CShadowConstantBuffer* CShadowConstantBuffer::Clone()
{
	return new CShadowConstantBuffer(*this);
}
