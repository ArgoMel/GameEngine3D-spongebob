#include "Animation2DConstantBuffer.h"

CAnimation2DConstantBuffer::CAnimation2DConstantBuffer()
	: m_BufferData{}
{
}

CAnimation2DConstantBuffer::CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& buffer)
	: CConstantBufferData(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CAnimation2DConstantBuffer::~CAnimation2DConstantBuffer()
{
}

bool CAnimation2DConstantBuffer::Init()
{
	SetConstantBuffer("Animation2D");
	return true;
}

void CAnimation2DConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CAnimation2DConstantBuffer* CAnimation2DConstantBuffer::Clone()
{
	return new CAnimation2DConstantBuffer(*this);
}
