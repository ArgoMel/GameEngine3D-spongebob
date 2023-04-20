#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Resource/Particle/Particle.h"
#include "../Resource/Shader/ParticleConstantBuffer.h"
#include "../Resource/Shader/ParticleRenderShader.h"
#include "../Resource/Shader/ParticleUpdateShader.h"
#include "../Resource/Shader/StructuredBuffer.h"

CParticleComponent::CParticleComponent()
	: m_CBuffer(nullptr)
	, m_SpawnTime(0.f)
	, m_SpawnCountMax(100)
	, m_SpawnTimeMax(0.01f)
{
	SetTypeID<CParticleComponent>();
	m_ComponentTypeName = "ParticleComponent";
	SetRenderLayerName("Particle");
}

CParticleComponent::CParticleComponent(const CParticleComponent& component)
	: CPrimitiveComponent(component)
{
	m_SpawnTime = 0.f;
	m_SpawnCountMax = component.m_SpawnCountMax;
	m_Particle = component.m_Particle;
	m_SpawnTimeMax = component.m_SpawnTimeMax;
	if (m_Particle)
	{
		m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);
		m_CBuffer = m_Particle->CloneCBuffer();
	}
}

CParticleComponent::~CParticleComponent()
{
	size_t	count = m_vecStructuredBuffer.size();
	for (size_t i = 0; i < count; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}
	SAFE_DELETE(m_CBuffer);
}

void CParticleComponent::DeleteCurrentParticle()
{
	if (m_CBuffer)
	{
		m_CBuffer->SetParticleLifeTimeMin(0.f);
		m_CBuffer->SetParticleLifeTimeMax(0.f);
	}
}

void CParticleComponent::ChangeParticle(const std::string& name)
{
	SetParticle(name);
	m_vecMaterial.clear();
	AddMaterial(m_Particle->m_Material);
}

void CParticleComponent::SetSpawnTime(float time)
{
	m_SpawnTimeMax = time;
}

void CParticleComponent::SetSpawnCountMax(int count)
{
	if (m_CBuffer)
	{
		m_CBuffer->SetParticleSpawnCountMax(count);
	}
	m_SpawnCountMax = count;
	m_vecStructuredBuffer[0]->Init("ParticleInfo", sizeof(ParticleInfo), count, 0);
}

void CParticleComponent::SetCamDir(EParticle_Cam_Dir dir)
{
	if (m_CBuffer)
	{
		m_CBuffer->SetParticleCamDir(dir);
	}
}

void CParticleComponent::SetParticle(const std::string& name)
{
	CParticle* particle = nullptr;
	if (m_Scene)
	{
		particle = m_Scene->GetResource()->FindParticle(name);
	}
	else
	{
		particle = CResourceManager::GetInst()->FindParticle(name);
	}
	SetParticle(particle);
}

void CParticleComponent::SetParticle(CParticle* particle)
{
	m_Particle = particle;
	if (m_Particle)
	{
		size_t	count = m_vecStructuredBuffer.size();
		for (size_t i = 0; i < count; ++i)
		{
			SAFE_DELETE(m_vecStructuredBuffer[i]);
		}
		SAFE_DELETE(m_CBuffer);
		m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);
		m_CBuffer = m_Particle->CloneCBuffer();
		m_SpawnTimeMax = m_Particle->m_SpawnTime;
	}
}

void CParticleComponent::SetMaterial(int slot, const std::string& name)
{
	CPrimitiveComponent::SetMaterial(slot, name);
}

void CParticleComponent::SetMaterial(int slot, CMaterial* material)
{
	CPrimitiveComponent::SetMaterial(slot, material);
}

void CParticleComponent::Start()
{
	CPrimitiveComponent::Start();
	if (m_vecMaterial.empty() && m_Particle && m_Particle->m_Material)
	{
		AddMaterial(m_Particle->m_Material);
	}
}

bool CParticleComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}
	SetMesh("ParticlePoint");
	SetParticle("LoadingBubble");
	return true;
}

void CParticleComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
	if (m_Particle)
	{
		m_SpawnTime += deltaTime;
		if (m_SpawnTime >= m_SpawnTimeMax)
		{
			m_SpawnTime = 0.f;
			m_CBuffer->SetParticleEnable(true);
		}
		else
		{
			m_CBuffer->SetParticleEnable(false);
		}
	}
}

void CParticleComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
	if (m_Particle)
	{
		Vector3	startMin = GetWorldPos() + m_Particle->m_CBuffer->m_BufferData.particleStartMin;
		Vector3	startMax = GetWorldPos() + m_Particle->m_CBuffer->m_BufferData.particleStartMax;
		m_CBuffer->SetParticleStartMin(startMin);
		m_CBuffer->SetParticleStartMax(startMax);
		m_CBuffer->UpdateBuffer();
		size_t	count = m_vecStructuredBuffer.size();
		for (size_t i = 0; i < count; ++i)
		{
			m_vecStructuredBuffer[i]->SetShader();
		}
		unsigned int	groupCount = m_CBuffer->m_BufferData.particleSpawnCountMax / 64 + 1;
		m_Particle->m_UpdateShader->Excute(groupCount, 1, 1);
		for (size_t i = 0; i < count; ++i)
		{
			m_vecStructuredBuffer[i]->ResetShader();
		}
	}
}

void CParticleComponent::Render()
{
	if (m_Particle)
	{
		CSceneComponent::Render();
		m_vecMaterial[0]->SetMaterial();
		size_t	count = m_vecStructuredBuffer.size();
		for (size_t i = 0; i < count; ++i)
		{
			m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)EShaderBufferType::Geometry);
		}
		m_Particle->m_RenderShader->SetShader();
		m_Mesh->RenderInstancing(m_CBuffer->m_BufferData.particleSpawnCountMax);
		m_vecMaterial[0]->ResetMaterial();
		count = m_vecStructuredBuffer.size();
		for (size_t i = 0; i < count; ++i)
		{
			m_vecStructuredBuffer[i]->ResetShader(30 + (int)i, (int)EShaderBufferType::Geometry);
		}
	}
}

CParticleComponent* CParticleComponent::Clone() const
{
	return new CParticleComponent(*this);
}

void CParticleComponent::Save(FILE* file)
{
	CSceneComponent::Save(file);
	int	Length = (int)m_Mesh->GetName().length();
	fwrite(&Length, 4, 1, file);
	fwrite(m_Mesh->GetName().c_str(), 1, Length, file);

	Length = (int)m_Particle->GetName().length();
	fwrite(&Length, 4, 1, file);
	fwrite(m_Particle->GetName().c_str(), 1, Length, file);

	fwrite(&m_SpawnTimeMax, sizeof(float), 1, file);
	fwrite(&m_SpawnCountMax, sizeof(int), 1, file);
}

void CParticleComponent::Load(FILE* file)
{
	CSceneComponent::Load(file);
	int	Length = 0;
	char	Name[256] = {};
	fread(&Length, 4, 1, file);
	assert(Length >= 0);
	fread(Name, 1, Length, file);
	SetMesh(Name);

	fread(&Length, 4, 1, file);
	char	ParticleName[256] = {};
	assert(Length >= 0);
	fread(ParticleName, 1, Length, file);
	SetParticle(ParticleName);

	fread(&m_SpawnTimeMax, sizeof(float), 1, file);
	fread(&m_SpawnCountMax, sizeof(int), 1, file);
	SetSpawnCountMax(m_SpawnCountMax);
}
