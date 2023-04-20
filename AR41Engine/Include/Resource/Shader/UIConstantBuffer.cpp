#include "UIConstantBuffer.h"

CUIConstantBuffer::CUIConstantBuffer() 
	: m_BufferData{}
{
}

CUIConstantBuffer::CUIConstantBuffer(const CUIConstantBuffer& buffer)
	: CConstantBufferData(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CUIConstantBuffer::~CUIConstantBuffer()
{
}

bool CUIConstantBuffer::Init()
{
	SetConstantBuffer("UI");
	return true;
}

void CUIConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CUIConstantBuffer* CUIConstantBuffer::Clone()
{
	return new CUIConstantBuffer(*this);
}