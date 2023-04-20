#include "MaterialConstantBuffer.h"

CMaterialConstantBuffer::CMaterialConstantBuffer()
	: m_BufferData{}
{
}

CMaterialConstantBuffer::CMaterialConstantBuffer(const CMaterialConstantBuffer& buffer)
	: CConstantBufferData(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CMaterialConstantBuffer::~CMaterialConstantBuffer()
{
}

bool CMaterialConstantBuffer::Init()
{
	SetConstantBuffer("Material");
	return true;
}

void CMaterialConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CMaterialConstantBuffer* CMaterialConstantBuffer::Clone()
{
	return new CMaterialConstantBuffer(*this);
}
