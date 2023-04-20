#include "ColliderConstantBuffer.h"

CColliderConstantBuffer::CColliderConstantBuffer()
	: m_BufferData{}
{
}

CColliderConstantBuffer::CColliderConstantBuffer(const CColliderConstantBuffer& buffer)
	: CConstantBufferData(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CColliderConstantBuffer::~CColliderConstantBuffer()
{
}

bool CColliderConstantBuffer::Init()
{
	SetConstantBuffer("Collider");
	return true;
}

void CColliderConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CColliderConstantBuffer* CColliderConstantBuffer::Clone()
{
	return new CColliderConstantBuffer(*this);
}
