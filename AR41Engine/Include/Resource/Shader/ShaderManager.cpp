#include "ShaderManager.h"
#include "SpriteColorShader.h"
#include "SpriteShader.h"
#include "ColliderShader.h"
#include "ColliderImageShader.h"
#include "ConstantBuffer.h"
#include "ColliderConstantBuffer.h"
#include "UIShader.h"
#include "UIProgressBarShader.h"
#include "TileMapShader.h"
#include "TileMapConstantBuffer.h"
#include "TileMapBackShader.h"
#include "MeshShader.h"
#include "MeshInstancingShader.h"
#include "DebugShader.h"
#include "LightAccShader.h"
#include "LightCelShader.h"
#include "ScreenShader.h"
#include "DeferredRenderShader.h"
#include "AnimationUpdateShader.h"
#include "SkyShader.h"
#include "TerrainShader.h"
#include "BillboardShader.h"
#include "ParticleUpdateShader.h"
#include "ParticleRenderShader.h"
#include "DecalShader.h"
#include "DecalDebugShader.h"
#include "ShadowMapShader.h"
#include "ShadowMapStaticShader.h"
#include "ShadowMapInstancingShader.h"
#include "ShadowMapStaticInstancingShader.h"
#include "FXAAShader.h"
#include "MSShader.h"
#include "OceanShader.h"

CShaderManager::CShaderManager()
{
	m_ColliderCBuffer = new CColliderConstantBuffer;
}

CShaderManager::~CShaderManager()
{
	SAFE_DELETE(m_ColliderCBuffer);
}

bool CShaderManager::Init()
{
    CreateShader<CSpriteColorShader>("SpriteColorShader", true);
	CreateShader<CSpriteShader>("SpriteShader", true);
	CreateShader<CColliderShader>("ColliderShader", true);
	CreateShader<CColliderImageShader>("ColliderImageShader", true);
	CreateShader<CUIShader>("UIShader", true);
	CreateShader<CUIProgressBarShader>("UIProgressBarShader", true);
	CreateShader<CTileMapShader>("TileMapShader", true);
	CreateShader<CTileMapBackShader>("TileMapBackShader", true);
	CreateShader<CMeshShader>("MeshShader", true);
	CreateShader<CMeshInstancingShader>("MeshInstancingShader", true);
	CreateShader<CDebugShader>("DebugShader", true);
	CreateShader<CLightAccShader>("LightAccShader", true);
	CreateShader<CLightCelShader>("LightCelShader", true);
	CreateShader<CScreenShader>("ScreenShader", true);
	CreateShader<CDeferredRenderShader>("DeferredRenderShader", true);
	CreateShader<CAnimationUpdateShader>("AnimationUpdateShader", true);
	CreateShader<CSkyShader>("SkyShader", true);
	CreateShader<CTerrainShader>("TerrainShader", true);
	CreateShader<CBillboardShader>("BillboardShader", true);
	CreateShader<CParticleUpdateShader>("ParticleUpdateShader", true);
	CreateShader<CParticleRenderShader>("ParticleRenderShader", true);
	CreateShader<CDecalDebugShader>("DecalDebugShader", true);
	CreateShader<CDecalShader>("DecalShader", true);
	CreateShader<CShadowMapShader>("ShadowMapShader", true);
	CreateShader<CShadowMapStaticShader>("ShadowMapStaticShader", true);
	CreateShader<CShadowMapInstancingShader>("ShadowMapInstancingShader", true);
	CreateShader<CShadowMapStaticInstancingShader>("ShadowMapStaticInstancingShader", true);
	CreateShader<CFXAAShader>("FXAAShader", true);
	CreateShader<CMSShader>("MSShader", true);
	CreateShader<COceanShader>("OceanShader", true);

	CreateConstantBuffer("Transform", sizeof(TransformCBuffer), 0);
	CreateConstantBuffer("Material", sizeof(MaterialCBuffer), 1);
	CreateConstantBuffer("Animation2D", sizeof(Animation2DCBuffer), 2);
	CreateConstantBuffer("Global", sizeof(GlobalCBuffer), 3);
	CreateConstantBuffer("Collider", sizeof(ColliderCBuffer), 12);
	CreateConstantBuffer("UI", sizeof(UICBuffer), 9);
	CreateConstantBuffer("UIProgressBar", sizeof(UIProgressBarCBuffer), 11);
	CreateConstantBuffer("TileMap", sizeof(TileMapCBuffer), 10);
	CreateConstantBuffer("Light", sizeof(LightCBuffer), 4);
	CreateConstantBuffer("Instancing", sizeof(InstancingCBuffer), 5, (int)EShaderBufferType::Vertex);
	CreateConstantBuffer("Animation", sizeof(AnimationCBuffer), 7, (int)EShaderBufferType::Compute);
	CreateConstantBuffer("Terrain", sizeof(TerrainCBuffer), 10,
		(int)EShaderBufferType::Vertex | (int)EShaderBufferType::Pixel);
	CreateConstantBuffer("ParticleUpdate", sizeof(ParticleCBuffer), 8, (int)EShaderBufferType::Compute);
	CreateConstantBuffer("Shadow", sizeof(ShadowCBuffer), 13, (int)EShaderBufferType::Graphic);
	CreateConstantBuffer("FXAA", sizeof(FXAACBuffer), 6, (int)EShaderBufferType::Pixel);
	m_ColliderCBuffer->Init();
	return true;
}

CShader* CShaderManager::FindShader(const std::string& name)
{
	auto	iter = m_mapShader.find(name);
	if (iter == m_mapShader.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& name)
{
	auto	iter = m_mapShader.find(name);
	if (iter != m_mapShader.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapShader.erase(iter);
		}
	}
}

bool CShaderManager::CreateConstantBuffer(const std::string& name, int size, int reg, int shaderBufferType)
{
	CConstantBuffer* buffer = FindConstantBuffer(name);
	if (buffer)
	{
		return true;
	}
	buffer = new CConstantBuffer;
	buffer->SetName(name);
	if (!buffer->Init(size, reg, shaderBufferType))
	{
		SAFE_RELEASE(buffer);
		return false;
	}
	m_mapCBuffer.insert(std::make_pair(name, buffer));
	return true;
}

CConstantBuffer* CShaderManager::FindConstantBuffer(const std::string& name)
{
	auto	iter = m_mapCBuffer.find(name);
	if (iter == m_mapCBuffer.end())
	{
		return nullptr;
	}
	return iter->second;
}
