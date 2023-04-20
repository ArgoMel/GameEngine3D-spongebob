#include "Particle.h"
#include "../ResourceManager.h"
#include "../Shader/StructuredBuffer.h"
#include "../Shader/ParticleConstantBuffer.h"
#include "../Shader/ParticleUpdateShader.h"
#include "../Shader/ParticleRenderShader.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CParticle::CParticle()
	: m_Scene(nullptr)
	, m_CBuffer(nullptr)
	, m_SpawnTime(0.01f)
{
}

CParticle::~CParticle()
{
	size_t	size = m_vecStructuredBuffer.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}
	SAFE_DELETE(m_CBuffer);
}

bool CParticle::Init()
{
	m_CBuffer = new CParticleConstantBuffer;
	m_CBuffer->Init();
	SetUpdateShader("ParticleUpdateShader");
	SetRenderShader("ParticleRenderShader");
	AddStructuredBuffer("ParticleInfo", sizeof(ParticleInfo),m_CBuffer->m_BufferData.particleSpawnCountMax, 0);
	AddStructuredBuffer("ParticleInfoShare", sizeof(ParticleInfoShare), 1, 1);
	return true;
}

void CParticle::SetUpdateShader(const std::string& name)
{
	if (m_Scene)
	{
		m_UpdateShader = (CParticleUpdateShader*)m_Scene->GetResource()->FindShader(name);
	}
	else
	{
		m_UpdateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindShader(name);
	}
}

void CParticle::SetRenderShader(const std::string& name)
{
	if (m_Scene)
	{
		m_RenderShader = (CParticleRenderShader*)m_Scene->GetResource()->FindShader(name);
	}
	else
	{
		m_RenderShader = (CParticleRenderShader*)CResourceManager::GetInst()->FindShader(name);
	}
}

void CParticle::AddStructuredBuffer(const std::string& name, unsigned int size, unsigned int count, int reg, 
	bool dynamic, int structuredBufferShaderType)
{
	CStructuredBuffer* buffer = new CStructuredBuffer;
	if (!buffer->Init(name, size, count, reg, dynamic, structuredBufferShaderType))
	{
		SAFE_DELETE(buffer);
		return;
	}
	m_vecStructuredBuffer.push_back(buffer);
}

void CParticle::ResizeStructuredBuffer(const std::string& name, unsigned int size, unsigned int count, int reg, 
	bool dynamic, int structuredBufferShaderType)
{
	size_t	bufferCount = m_vecStructuredBuffer.size();
	for (size_t i = 0; i < bufferCount; ++i)
	{
		if (m_vecStructuredBuffer[i]->GetName() == name)
		{
			m_vecStructuredBuffer[i]->Init(name, size, count, reg, dynamic, structuredBufferShaderType);
		}
	}
}

void CParticle::CloneStructuredBuffer(std::vector<class CStructuredBuffer*>& vecBuffer)
{
	size_t	size = m_vecStructuredBuffer.size();
	vecBuffer.clear();
	for (size_t i = 0; i < size; ++i)
	{
		vecBuffer.push_back(m_vecStructuredBuffer[i]->Clone());
	}
}

CParticleConstantBuffer* CParticle::CloneCBuffer() const
{
	return m_CBuffer->Clone();
}

void CParticle::SetParticleSpawnCountMax(int count)
{
	m_CBuffer->SetParticleSpawnCountMax(count);
	ResizeStructuredBuffer("ParticleInfo", sizeof(ParticleInfo), count, 0);
}

void CParticle::SetParticleSpawnTime(float time)
{
	m_SpawnTime = time;
}

void CParticle::SetMaterial(const std::string& name)
{
	if (m_Scene)
	{
		m_Material = (CMaterial*)m_Scene->GetResource()->FindMaterial(name);
	}
	else
	{
		m_Material = (CMaterial*)CResourceManager::GetInst()->FindMaterial(name);
	}
}

void CParticle::SetMaterial(CMaterial* material)
{
	m_Material = material;
}

void CParticle::SetParticleEnable(bool enable)
{
	m_CBuffer->SetParticleEnable(enable);
}

void CParticle::SetParticleStartMin(const Vector3& min)
{
	m_CBuffer->SetParticleStartMin(min);
}

void CParticle::SetParticleStartMax(const Vector3& max)
{
	m_CBuffer->SetParticleStartMax(max);
}

void CParticle::SetParticleScaleMin(const Vector3& min)
{
	m_CBuffer->SetParticleScaleMin(min);
}

void CParticle::SetParticleScaleMax(const Vector3& max)
{
	m_CBuffer->SetParticleScaleMax(max);
}

void CParticle::SetParticleLifeTimeMin(float min)
{
	m_CBuffer->SetParticleLifeTimeMin(min);
}

void CParticle::SetParticleLifeTimeMax(float max)
{
	m_CBuffer->SetParticleLifeTimeMax(max);
}

void CParticle::SetParticleColorMin(const Vector4& min)
{
	m_CBuffer->SetParticleColorMin(min);
}

void CParticle::SetParticleColorMax(const Vector4& max)
{
	m_CBuffer->SetParticleColorMax(max);
}

void CParticle::SetParticleSpeedMin(float min)
{
	m_CBuffer->SetParticleSpeedMin(min);
}

void CParticle::SetParticleSpeedMax(float max)
{
	m_CBuffer->SetParticleSpeedMax(max);
}

void CParticle::SetParticleMoveEnable(bool enable)
{
	m_CBuffer->SetParticleMoveEnable(enable);
}

void CParticle::SetParticleGravityEnable(bool enable)
{
	m_CBuffer->SetParticleGravityEnable(enable);
}

void CParticle::SetParticleMoveDir(const Vector3& moveDir)
{
	m_CBuffer->SetParticleMoveDir(moveDir);
}

void CParticle::SetParticleMoveDirEnable(bool enable)
{
	m_CBuffer->SetParticleMoveDirEnable(enable);
}

void CParticle::SetParticleMoveAngle(const Vector3& moveAngle)
{
	m_CBuffer->SetParticleMoveAngle(moveAngle);
}

void CParticle::SetParticleCamDir(EParticle_Cam_Dir dir)
{
	m_CBuffer->SetParticleCamDir(dir);
}
