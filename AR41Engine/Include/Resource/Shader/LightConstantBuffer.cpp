#include "LightConstantBuffer.h"

CLightConstantBuffer::CLightConstantBuffer()
    : m_BufferData{}
{
}

CLightConstantBuffer::CLightConstantBuffer(const CLightConstantBuffer& buffer)
    : CConstantBufferData(buffer)
{
    m_BufferData = buffer.m_BufferData;
}

CLightConstantBuffer::~CLightConstantBuffer()
{
}

bool CLightConstantBuffer::Init()
{
    SetConstantBuffer("Light");
    return true;
}

void CLightConstantBuffer::UpdateBuffer()
{
    m_Buffer->UpdateBuffer(&m_BufferData);
}

CLightConstantBuffer* CLightConstantBuffer::Clone()
{
    return new CLightConstantBuffer(*this);
}
