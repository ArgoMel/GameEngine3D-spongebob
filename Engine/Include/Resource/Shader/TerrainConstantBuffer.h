#pragma once
#include "ConstantBufferData.h"

class CTerrainConstantBuffer : public CConstantBufferData
{
private:
	TerrainCBuffer	m_BufferData;
public:
	float GetDetailLevel() const
	{
		return m_BufferData.detailLevel;
	}
	int GetSplatCount() const
	{
		return m_BufferData.splatCount;
	}
	float GetTime() const
	{
		return m_BufferData.time;
	}
	float GetWaveSpeed() const
	{
		return m_BufferData.waveSpeed;
	}
	void SetDetailLevel(float level)
	{
		m_BufferData.detailLevel = level;
	}
	void SetSplatCount(int count)
	{
		m_BufferData.splatCount = count;
	}
	void SetTime(float time)
	{
		m_BufferData.time = time;
	}
	void SetWaveSpeed(float height)
	{
		m_BufferData.waveSpeed = height;
	}
	CTerrainConstantBuffer();
	CTerrainConstantBuffer(const CTerrainConstantBuffer& buffer);
	virtual ~CTerrainConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CTerrainConstantBuffer* Clone();
};

