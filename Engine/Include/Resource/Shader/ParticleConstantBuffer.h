#pragma once
#include "ConstantBufferData.h"

class CParticleConstantBuffer : public CConstantBufferData
{
private:
	friend class CParticle;
	friend class CParticleComponent;
	ParticleCBuffer	m_BufferData;
public:
	ParticleCBuffer GetBufferData()	const
	{
		return m_BufferData;
	}
	void SetParticleEnable(bool enable)
	{
		m_BufferData.particleSpawnEnable = enable ? 1 : 0;
	}
	void SetParticleStartMin(const Vector3& min)
	{
		m_BufferData.particleStartMin = min;
	}
	void SetParticleStartMax(const Vector3& max)
	{
		m_BufferData.particleStartMax = max;
	}
	void SetParticleSpawnCountMax(unsigned int count)
	{
		m_BufferData.particleSpawnCountMax = count;
	}
	void SetParticleScaleMin(const Vector3& min)
	{
		m_BufferData.particleScaleMin = min;
	}
	void SetParticleScaleMax(const Vector3& max)
	{
		m_BufferData.particleScaleMax = max;
	}
	void SetParticleLifeTimeMin(float min)
	{
		m_BufferData.particleLifeTimeMin = min;
	}
	void SetParticleLifeTimeMax(float max)
	{
		m_BufferData.particleLifeTimeMax = max;
	}
	void SetParticleColorMin(const Vector4& min)
	{
		m_BufferData.particleColorMin = min;
	}
	void SetParticleColorMax(const Vector4& max)
	{
		m_BufferData.particleColorMax = max;
	}
	void SetParticleSpeedMin(float min)
	{
		m_BufferData.particleSpeedMin = min;
	}
	void SetParticleSpeedMax(float max)
	{
		m_BufferData.particleSpeedMax = max;
	}
	void SetParticleMoveEnable(bool enable)
	{
		m_BufferData.particleMoveEnable = enable ? 1 : 0;
	}
	void SetParticleGravityEnable(bool enable)
	{
		m_BufferData.particleGravity = enable ? 1 : 0;
	}
	void SetParticleMoveDir(const Vector3& moveDir)
	{
		m_BufferData.particleMoveDir = moveDir;
	}
	void SetParticleMoveDirEnable(bool enable)
	{
		m_BufferData.particleMoveDirEnable = enable ? 1 : 0;
	}
	void SetParticleMoveAngle(const Vector3& moveAngle)
	{
		m_BufferData.particleMoveAngle = moveAngle;
	}
	void SetParticleCamDir(EParticle_Cam_Dir dir)
	{
		m_BufferData.particleCamDir=(unsigned int)dir;
	}
	CParticleConstantBuffer();
	CParticleConstantBuffer(const CParticleConstantBuffer& buffer);
	virtual ~CParticleConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CParticleConstantBuffer* Clone();
};

