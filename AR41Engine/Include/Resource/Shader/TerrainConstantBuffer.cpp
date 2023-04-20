#include "TerrainConstantBuffer.h"

CTerrainConstantBuffer::CTerrainConstantBuffer()
	: m_BufferData{}
{
}

CTerrainConstantBuffer::CTerrainConstantBuffer(const CTerrainConstantBuffer& buffer)
	: CConstantBufferData(buffer)
{
	m_BufferData = buffer.m_BufferData;
}

CTerrainConstantBuffer::~CTerrainConstantBuffer()
{
}

bool CTerrainConstantBuffer::Init()
{
	SetConstantBuffer("Terrain");
	m_BufferData.detailLevel = 80.f;
	m_BufferData.splatCount = 1;
	m_BufferData.time = 0.f;
	m_BufferData.waveSpeed = 1000.f;
	return true;
}

void CTerrainConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CTerrainConstantBuffer* CTerrainConstantBuffer::Clone()
{
	return new CTerrainConstantBuffer(*this);
}
