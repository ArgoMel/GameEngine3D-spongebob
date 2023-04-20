#include "TileMapConstantBuffer.h"

CTileMapConstantBuffer::CTileMapConstantBuffer() 
	: m_BufferData{}
{
}

CTileMapConstantBuffer::CTileMapConstantBuffer(const CTileMapConstantBuffer& buffer) 
	: CConstantBufferData(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CTileMapConstantBuffer::~CTileMapConstantBuffer()
{
}

bool CTileMapConstantBuffer::Init()
{
	SetConstantBuffer("TileMap");
	return true;
}

void CTileMapConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CTileMapConstantBuffer* CTileMapConstantBuffer::Clone()
{
	return new CTileMapConstantBuffer(*this);
}

