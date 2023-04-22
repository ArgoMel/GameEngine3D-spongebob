#include "RenderManager.h"
#include "RenderState.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "../Device.h"
#include "../Engine.h"
#include "../Component/SceneComponent.h"
#include "../Component/CameraComponent.h"
#include "../Component/ParticleComponent.h"
#include "../Component/DecalComponent.h"
#include "../Component/Collider3D.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneViewport.h"
#include "../Scene/LightManager.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/ShadowConstantBuffer.h"
#include "../Resource/Shader/FXAAConstantBuffer.h"
#include "../GameObject/SkySphere.h"
DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
	: m_ShadowMapRS{}
	, m_ShaderType(EShaderType::Default)
	, m_IsFxaa(true)
{
	m_RenderStateManager = new CRenderStateManager;
	m_ShadowCBuffer = new CShadowConstantBuffer;
	m_FXAACBuffer = new CFXAAConstantBuffer;
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(m_ShadowCBuffer);
	SAFE_DELETE(m_FXAACBuffer);
	size_t	size = m_vecInstancingPool.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecInstancingPool[i]);
	}
	m_vecInstancingPool.clear();

	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	SAFE_DELETE(m_RenderStateManager);
}

void CRenderManager::SetShaderType(EShaderType type)
{
	m_ShaderType = type;
	std::string	name;
	switch (m_ShaderType)
	{
	case EShaderType::Default:
		name = "LightAccShader";
		break;
	case EShaderType::CelShader:
		name = "LightCelShader";
		break;
	}
	CSceneManager::GetInst()->GetScene()->GetLightManager()->SetLightAccShader(name);
}

CRenderInstancing* CRenderManager::FindInstancing(CMesh* mesh)
{
	auto	iter = m_mapInstancing.find(mesh);
	if (iter == m_mapInstancing.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CRenderManager::CreateRenderTarget()
{
	m_vecInstancingPool.resize(30);	// 인스턴싱용 메모리 풀 생성
	size_t	size = m_vecInstancingPool.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecInstancingPool[i] = new CRenderInstancing;
		m_vecInstancingPool[i]->m_PoolIndex = (int)i;
	}
	for (int i = 0; i < 30; ++i)
	{
		m_EmptyPoolList.push_back(i);
	}
	Resolution	RS = CDevice::GetInst()->GetResolution();
	CResourceManager::GetInst()->CreateTarget("GBuffer1", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);	//알베도
	CResourceManager::GetInst()->CreateTarget("GBuffer2", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);	//노말
	CResourceManager::GetInst()->CreateTarget("GBuffer3", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);	//깊이
	CResourceManager::GetInst()->CreateTarget("GBuffer4", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);	//머티리얼
	CResourceManager::GetInst()->CreateTarget("GBuffer5", RS.width,	RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("GBuffer6", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CRenderTarget* gBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	gBufferTarget->SetPos(Vector3(0.f, 0.f, 0.f));
	gBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	gBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(gBufferTarget);
	m_vecDecalBuffer.push_back(gBufferTarget);

	gBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer2");
	gBufferTarget->SetPos(Vector3(0.f, 100.f, 0.f));
	gBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	gBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(gBufferTarget);
	m_vecDecalBuffer.push_back(gBufferTarget);

	gBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");
	gBufferTarget->SetPos(Vector3(0.f, 200.f, 0.f));
	gBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	gBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(gBufferTarget);

	gBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer4");
	gBufferTarget->SetPos(Vector3(0.f, 300.f, 0.f));
	gBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	gBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(gBufferTarget);
	m_vecDecalBuffer.push_back(gBufferTarget);

	gBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer5");
	gBufferTarget->SetPos(Vector3(0.f, 400.f, 0.f));
	gBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	gBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(gBufferTarget);

	gBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer6");
	gBufferTarget->SetPos(Vector3(0.f, 500.f, 0.f));
	gBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	gBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(gBufferTarget);
	// Light Target
	CResourceManager::GetInst()->CreateTarget("LightDiffuse", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("LightSpecular", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	CResourceManager::GetInst()->CreateTarget("LightEmissive", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CRenderTarget* lightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightDiffuse");
	lightTarget->SetPos(Vector3(100.f, 0.f, 0.f));
	lightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	lightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(lightTarget);

	lightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightSpecular");
	lightTarget->SetPos(Vector3(100.f, 100.f, 0.f));
	lightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	lightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(lightTarget);

	lightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightEmissive");
	lightTarget->SetPos(Vector3(100.f, 200.f, 0.f));
	lightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	lightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(lightTarget);

	// RenderScreenTarget
	CResourceManager::GetInst()->CreateTarget("RenderScreen", RS.width, RS.height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_ScreenBuffer = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("RenderScreen");
	m_ScreenBuffer->SetPos(Vector3(200.f, 0.f, 0.f));
	m_ScreenBuffer->SetScale(Vector3(100.f, 100.f, 1.f));
	m_ScreenBuffer->SetDebugRender(true);

	// ShadowMapTarget
	m_ShadowMapRS.width = 2560;
	m_ShadowMapRS.height = 1440;
	CResourceManager::GetInst()->CreateTarget("ShadowMap", m_ShadowMapRS.width, m_ShadowMapRS.height, 
		DXGI_FORMAT_R32G32B32A32_FLOAT,	DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_ShadowMapTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("ShadowMap");
	m_ShadowMapTarget->SetPos(Vector3(300.f, 100.f, 0.f));
	m_ShadowMapTarget->SetScale(Vector3(300.f, 300.f, 1.f));
	m_ShadowMapTarget->SetDebugRender(true);
	m_ShadowCBuffer->Init();
	m_ShadowCBuffer->SetShadowResolution(Vector2((float)m_ShadowMapRS.width,(float)m_ShadowMapRS.height));

	m_ScreenShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("ScreenShader");
	//m_DeferredRenderShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("DeferredRenderShader");

	// No-MultiSampling
	m_MSShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("MSShader");
	CResourceManager::GetInst()->CreateTargetNoMS("RenderNoMS", RS.width, RS.height, 
		DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_MSBuffer = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("RenderNoMS");
	m_MSBuffer->SetPos(Vector3(200.f, 200.f, 0.f));
	m_MSBuffer->SetScale(Vector3(100.f, 100.f, 1.f));
	//m_MSBuffer->SetDebugRender(true);

	// FXAA
	m_FXAAShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("FXAAShader");
	CResourceManager::GetInst()->CreateTargetNoMS("RenderFXAA", RS.width, RS.height, 
		DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_FXAABuffer = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("RenderFXAA");
	m_FXAABuffer->SetPos(Vector3(200.f, 300.f, 0.f));
	m_FXAABuffer->SetScale(Vector3(100.f, 100.f, 1.f));
	//m_FXAABuffer->SetDebugRender(true);
	m_FXAACBuffer->Init();
}

void CRenderManager::CreateLayer(const std::string& name, int priority)
{
	RenderLayer* layer = new RenderLayer;
	layer->name = name;
	layer->layerPriority = priority;
	m_RenderLayerList.push_back(layer);
	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerPriority(const std::string& name, int priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == name)
		{
			(*iter)->layerPriority = priority;
			break;
		}
	}
	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerAlphaBlend(const std::string& name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == name)
		{
			(*iter)->alphaBlend = FindRenderState<CRenderState>("AlphaBlend");
			break;
		}
	}
}

void CRenderManager::DeleteLayer(const std::string& name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == name)
		{
			SAFE_DELETE((*iter));
			iter = m_RenderLayerList.erase(iter);
			break;
		}
	}
}

void CRenderManager::AddRenderList(CSceneComponent* component)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == component->GetRenderLayerName())
		{
			(*iter)->renderList.push_back(component);
			break;
		}
	}
}

bool CRenderManager::Init()
{
	if (!m_RenderStateManager->Init())
	{
		return false;
	}
	CreateLayer("Particle", 0);
	CreateLayer("Decal", 1);
	CreateLayer("Collider", 2);
	CreateLayer("Player", 3);
	SetLayerAlphaBlend("Player");
	CreateLayer("Default", 4);
	SetLayerAlphaBlend("Default");
	CreateLayer("Back", 9);
	m_AlphaBlend = m_RenderStateManager->FindRenderState<CBlendState>("AlphaBlend");
	m_MRTAlphaBlend = m_RenderStateManager->FindRenderState<CBlendState>("MRTAlphaBlend");
	m_DepthDisable = m_RenderStateManager->FindRenderState<CDepthStencilState>("DepthDisable");
	m_DepthWriteDisable = m_RenderStateManager->FindRenderState<CDepthStencilState>("DepthWriteDisable");
	m_LightAccBlend = m_RenderStateManager->FindRenderState<CBlendState>("LightAccBlend");
	return true;
}

void CRenderManager::Render(float deltaTime)
{
	if (CEngine::GetInst()->GetRender2D())
	{
		Render2D(deltaTime);
	}
	else
	{
		Render3D(deltaTime);
	}
	//UI
	m_AlphaBlend->SetState();
	m_DepthDisable->SetState();
	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();
#ifdef _DEBUG
	CResourceManager::GetInst()->RenderTexture();
#endif // _DEBUG
	m_DepthDisable->ResetState();
	m_AlphaBlend->ResetState();

}

void CRenderManager::SetBlendFactor(const std::string& name, float r, float g, float b, float a)
{
	m_RenderStateManager->SetBlendFactor(name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& name, bool blendEnable, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, 
	D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphBlend, D3D11_BLEND destAlphBlend, D3D11_BLEND_OP blendAlphOp, UINT8 writeMask)
{
	m_RenderStateManager->AddBlendInfo(name, blendEnable, srcBlend, destBlend, blendOp,	
		srcAlphBlend, destAlphBlend, blendAlphOp, writeMask);
}

bool CRenderManager::CreateBlendState(const std::string& name, bool alphaToCoverageEnable, bool independentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(name, alphaToCoverageEnable, independentBlendEnable);
}

bool CRenderManager::CreateDepthStencil(const std::string& name, bool depthEnable, D3D11_DEPTH_WRITE_MASK depthWriteMask,
	D3D11_COMPARISON_FUNC depthFunc, bool stencilEnable, UINT8 stencilReadMask, UINT8 stencilWriteMask, 
	D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace)
{
	return m_RenderStateManager->CreateDepthStencil(name, depthEnable, depthWriteMask, depthFunc, 
		stencilEnable, stencilReadMask, stencilWriteMask, frontFace, backFace);
}

bool CRenderManager::SortLayer(RenderLayer* src, RenderLayer* dest)
{
	return src->layerPriority < dest->layerPriority;	//오름차순
}

bool CRenderManager::SortAlphaObject(CSceneComponent* src, CSceneComponent* dest)
{
	// 뷰공간의 z값을 비교한다.
	Vector3	srcPos = src->GetWorldPos();
	Vector3	destPos = dest->GetWorldPos();
	Matrix	viewMatrix = src->GetScene()->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	srcPos = srcPos.TransformCoord(viewMatrix);
	destPos = destPos.TransformCoord(viewMatrix);
	return srcPos.z < destPos.z;
}

void CRenderManager::Render2D(float deltaTime)
{
	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();
		for (; iter != iterEnd; ++iter)
		{
			auto	iter1 = (*iter)->renderList.begin();
			auto	iter1End = (*iter)->renderList.end();
			if ((*iter)->alphaBlend)
			{
				(*iter)->alphaBlend->SetState();
			}
			for (; iter1 != iter1End;)
			{
				if ((*iter1)->GetTypeName() == "class CColliderPixel" && (*iter1)->GetRefCount() == 1)
				{
					(*iter1)->Destroy();
				}
				if (!(*iter1)->GetActive())
				{
					iter1 = (*iter)->renderList.erase(iter1);
					iter1End = (*iter)->renderList.end();
					continue;
				}
				else if (!(*iter1)->GetEnable())
				{
					++iter1;
					continue;
				}
				(*iter1)->Render();
				++iter1;
			}
			if ((*iter)->alphaBlend)
			{
				(*iter)->alphaBlend->ResetState();
			}
		}
	}
}

void CRenderManager::Render3D(float deltaTime)
{
	CGameObject* skyObj = CSceneManager::GetInst()->GetScene()->GetSky();
	if (skyObj)
	{
		skyObj->GetRootComponent()->Render();
	}

	RenderShadowMap(deltaTime);	// ShadowMap 을 그려낸다.
	RenderGBuffer(deltaTime);	// GBuffer를 그려낸다.
	RenderDecal(deltaTime);	// Decal을 그려낸다.
	RenderLight(deltaTime);	// 조명 처리된 정보를 그려낸다.
	RenderScreen(deltaTime);	// GBuffer와 Light를 합한 최종 화면을 만든다.
	RenderNoMultiSampling(deltaTime);
	RenderFXAA(deltaTime);
	RenderDeferred(deltaTime);	// 완성된 타겟을 백버퍼에 출력한다.
	RenderParticle(deltaTime);	// 파티클 출력
}

void CRenderManager::RenderShadowMap(float deltaTime)
{
	D3D11_VIEWPORT	vp = {};
	D3D11_VIEWPORT	prevVp = {};
	UINT	count = 1;
	CDevice::GetInst()->GetContext()->RSGetViewports(&count, &prevVp);
	vp.Width = (float)m_ShadowMapRS.width;
	vp.Height = (float)m_ShadowMapRS.height;
	vp.MaxDepth = 1.f;
	CDevice::GetInst()->GetContext()->RSSetViewports(1, &vp);
	m_ShadowMapTarget->ClearTarget();
	m_ShadowMapTarget->SetTarget();
	m_NormalRenderList.clear();
	RenderLayer* gBufferLayer = FindLayer("Default");
	auto	iter = gBufferLayer->renderList.begin();
	auto	iterEnd = gBufferLayer->renderList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = gBufferLayer->renderList.erase(iter);
			iterEnd = gBufferLayer->renderList.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		else if ((*iter)->GetFrustumCull())
		{
			++iter;
			continue;
		}
		//if ((*iter)->GetSceneComponentType() == SceneComponentType::Primitive)
		//{
		//	CMesh* mesh = ((CPrimitiveComponent*)iter->Get())->GetMesh();
		//	if (mesh->GetRenderCount() >= 5)
		//	{
		//		CRenderInstancing* instancing = nullptr;
		//		instancing = FindInstancing(mesh);
		//		if (!instancing)
		//		{
		//			if (m_EmptyPoolList.empty())
		//			{
		//				std::vector<CRenderInstancing*>	newPool = m_vecInstancingPool;
		//				m_vecInstancingPool.clear();
		//				m_vecInstancingPool.resize(newPool.size() * 2);
		//				for (size_t i = 0; i < newPool.size(); ++i)
		//				{
		//					m_vecInstancingPool[i] = newPool[i];
		//				}
		//				for (size_t i = newPool.size(); i < newPool.size() * 2; ++i)
		//				{
		//					m_vecInstancingPool[i] = new CRenderInstancing;
		//					m_vecInstancingPool[i]->m_PoolIndex = (int)i;
		//					m_EmptyPoolList.push_back((int)i);
		//				}
		//			}
		//			int index = m_EmptyPoolList.front();
		//			m_EmptyPoolList.pop_front();
		//			m_mapInstancing.insert(std::make_pair(mesh, m_vecInstancingPool[index]));
		//			m_vecInstancingPool[index]->m_Key = mesh;
		//			instancing = m_vecInstancingPool[index];
		//		}
		//		instancing->m_LayerName = "Default";
		//		instancing->AddRenderList((CPrimitiveComponent*)iter->Get());
		//	}
		//	else
		//	{
		//		m_NormalRenderList.push_back(*iter);
		//	}
		//}
		//else
		//{
			m_NormalRenderList.push_back(*iter);
		//}
		++iter;
	}
	// 인스턴싱이 아닌 물체를 그려낸다.
	auto	iter1 = m_NormalRenderList.begin();
	auto	iter1End = m_NormalRenderList.end();
	for (; iter1 != iter1End; ++iter1)
	{
		(*iter1)->RenderShadowMap();
	}
	auto	iter2 = m_mapInstancing.begin();
	auto	iter2End = m_mapInstancing.end();
	for (; iter2 != iter2End;)
	{
		if (iter2->second->GetInstancingCount() == 0)
		{
			m_EmptyPoolList.push_back(iter2->second->GetPoolIndex());
			iter2->second->m_Key = nullptr;
			iter2 = m_mapInstancing.erase(iter2);
			iter2End = m_mapInstancing.end();
			continue;
		}
		++iter2;
	}
	iter2 = m_mapInstancing.begin();
	iter2End = m_mapInstancing.end();
	for (; iter2 != iter2End; ++iter2)
	{
		if (iter2->second->m_LayerName != "Default")
		{
			continue;
		}
		iter2->second->RenderShadowMap();
	}
	m_ShadowMapTarget->ResetTarget();
	CDevice::GetInst()->GetContext()->RSSetViewports(1, &prevVp);
}

void CRenderManager::RenderGBuffer(float deltaTime)
{
	// GBuffer Target을 지워준다.
	size_t	size = m_vecGBuffer.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecGBuffer[i]->ClearTarget();
	}
	// GBuffer Target을 지정한다.
	std::vector<ID3D11RenderTargetView*>	vecTargetView;
	std::vector<ID3D11RenderTargetView*>	vecPrevTargetView;
	vecPrevTargetView.resize(size);
	for (size_t i = 0; i < size; ++i)
	{
		vecTargetView.push_back(m_vecGBuffer[i]->GetTargetView());
	}
	ID3D11DepthStencilView* depthView = nullptr;
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((UINT)size, &vecPrevTargetView[0], &depthView);
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)size, &vecTargetView[0], depthView);

	// GBuffer에 그릴 내용을 출력한다.
	auto	iter1 = m_NormalRenderList.begin();
	auto	iter1End = m_NormalRenderList.end();
	for (; iter1 != iter1End; ++iter1)
	{
		(*iter1)->Render();
	}
	auto	iter2 = m_mapInstancing.begin();
	auto	iter2End = m_mapInstancing.end();
	for (; iter2 != iter2End; ++iter2)
	{
		if (iter2->second->m_LayerName != "Default")
		{
			continue;
		}
		iter2->second->Render();
	}

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)size, &vecPrevTargetView[0], depthView);
	SAFE_RELEASE(depthView);
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_RELEASE(vecPrevTargetView[i]);
	}
}

void CRenderManager::RenderDecal(float deltaTime)
{
	size_t	size = m_vecDecalBuffer.size();
	// GBuffer Target을 지정한다.
	std::vector<ID3D11RenderTargetView*>	vecTargetView;
	std::vector<ID3D11RenderTargetView*>	vecPrevTargetView;
	vecPrevTargetView.resize(size);
	for (size_t i = 0; i < size; ++i)
	{
		vecTargetView.push_back(m_vecDecalBuffer[i]->GetTargetView());
	}
	ID3D11DepthStencilView* depthView = nullptr;
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((UINT)size, &vecPrevTargetView[0], &depthView);
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)size, &vecTargetView[0], depthView);
	
	m_DepthWriteDisable->SetState();
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecGBuffer[4]->SetTargetShader(18);
	m_vecGBuffer[5]->SetTargetShader(19);
	m_MRTAlphaBlend->SetState();

	RenderLayer* decalLayer = FindLayer("Decal");
	std::list<CSceneComponent*>	renderList;
	auto	iter = decalLayer->renderList.begin();
	auto	iterEnd = decalLayer->renderList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = decalLayer->renderList.erase(iter);
			iterEnd = decalLayer->renderList.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		else if ((*iter)->GetFrustumCull())
		{
			++iter;
			continue;
		}
		(*iter)->Render();
		++iter;
	}

	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecGBuffer[4]->ResetTargetShader(18);
	m_vecGBuffer[5]->ResetTargetShader(19);
	m_MRTAlphaBlend->ResetState();
	m_DepthWriteDisable->ResetState();

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)size, &vecPrevTargetView[0], depthView);
	SAFE_RELEASE(depthView);
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_RELEASE(vecPrevTargetView[i]);
	}
}

void CRenderManager::RenderLight(float deltaTime)
{
	// Light Target을 지워준다.
	size_t	size = m_vecLightBuffer.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecLightBuffer[i]->ClearTarget();
	}
	// Light Target을 지정한다.
	std::vector<ID3D11RenderTargetView*>	vecTargetView;
	std::vector<ID3D11RenderTargetView*>	vecPrevTargetView;
	vecPrevTargetView.resize(size);
	for (size_t i = 0; i < size; ++i)
	{
		vecTargetView.push_back(m_vecLightBuffer[i]->GetTargetView());
	}
	ID3D11DepthStencilView* depthView = nullptr;
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((UINT)size, &vecPrevTargetView[0], &depthView);
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)size, &vecTargetView[0], depthView);
	m_DepthDisable->SetState();
	m_LightAccBlend->SetState();
	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[1]->SetTargetShader(15);
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecGBuffer[3]->SetTargetShader(17);
	CSceneManager::GetInst()->GetScene()->GetLightManager()->Render();
	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecGBuffer[1]->ResetTargetShader(15);
	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecGBuffer[3]->ResetTargetShader(17);
	m_LightAccBlend->ResetState();
	m_DepthDisable->ResetState();
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)size, &vecPrevTargetView[0], depthView);
	SAFE_RELEASE(depthView);
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_RELEASE(vecPrevTargetView[i]);
	}
}

void CRenderManager::RenderScreen(float deltaTime)
{
	m_ScreenBuffer->ClearTarget();
	m_ScreenBuffer->SetTarget();
	m_ScreenShader->SetShader();
	m_DepthDisable->SetState();

	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecLightBuffer[0]->SetTargetShader(18);
	m_vecLightBuffer[1]->SetTargetShader(19);
	m_vecLightBuffer[2]->SetTargetShader(20);
	m_ShadowMapTarget->SetTargetShader(22);

	Matrix matView = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetShadowViewMatrix();
	Matrix matProj = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetShadowProjMatrix();
	Matrix	matVP = matView * matProj;
	m_ShadowCBuffer->SetShadowVP(matVP);
	m_ShadowCBuffer->UpdateBuffer();

	ID3D11DeviceContext* context = CDevice::GetInst()->GetContext();
	UINT	offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, &offset);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	context->Draw(4, 0);

	m_DepthDisable->ResetState();
	m_ShadowMapTarget->ResetTargetShader(22);
	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecLightBuffer[0]->ResetTargetShader(18);
	m_vecLightBuffer[1]->ResetTargetShader(19);
	m_vecLightBuffer[2]->ResetTargetShader(20);
	m_ScreenBuffer->ResetTarget();
}

void CRenderManager::RenderNoMultiSampling(float deltaTime)
{
	m_MSBuffer->ClearTarget();
	m_MSBuffer->SetTarget();
	m_MSShader->SetShader();
	m_DepthDisable->SetState();
	m_ScreenBuffer->SetTargetShader(8);
	ID3D11DeviceContext* context = CDevice::GetInst()->GetContext();
	UINT   offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, &offset);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	context->Draw(4, 0);
	m_DepthDisable->ResetState();
	m_ScreenBuffer->ResetTargetShader(8);
	m_MSBuffer->ResetTarget();
}

void CRenderManager::RenderFXAA(float deltaTime)
{
	m_FXAABuffer->ClearTarget();
	m_FXAABuffer->SetTarget();
	m_FXAAShader->SetShader();
	m_DepthDisable->SetState();
	m_MSBuffer->SetTargetShader(21); // 1.얘를 FXAA 로 넘겨줘야한다.
	ID3D11DeviceContext* context = CDevice::GetInst()->GetContext();
	UINT   offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, &offset);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	context->Draw(4, 0);
	m_DepthDisable->ResetState();
	m_MSBuffer->ResetTargetShader(21);
	m_FXAABuffer->ResetTarget();
}

void CRenderManager::RenderDeferred(float deltaTime)
{
	//m_DeferredRenderShader->SetShader();
	m_FXAAShader->SetShader();
	m_DepthDisable->SetState();
	if(m_IsFxaa)
	{
		m_FXAABuffer->SetTargetShader(21);
	}
	else
	{
		m_ScreenBuffer->SetTargetShader(21);
	}
	ID3D11DeviceContext* context = CDevice::GetInst()->GetContext();
	UINT	offset = 0;
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, &offset);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	context->Draw(4, 0);
	m_DepthDisable->ResetState();
	if (m_IsFxaa)
	{
		m_FXAABuffer->ResetTargetShader(21);
	}
	else
	{
		m_ScreenBuffer->ResetTargetShader(21);
	}
	// 디버그 모드일 경우 데칼 디버깅용 육면체를 출력한다.
#ifdef _DEBUG
	{
		RenderLayer* decalLayer = FindLayer("Decal");
		auto	iter = decalLayer->renderList.begin();
		auto	iterEnd = decalLayer->renderList.end();
		for (; iter != iterEnd;)
		{
			if (!(*iter)->GetActive())
			{
				iter = decalLayer->renderList.erase(iter);
				iterEnd = decalLayer->renderList.end();
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}
			((CDecalComponent*)(*iter).Get())->RenderDebug();
			++iter;
		}
	}
	{
		RenderLayer* colliderLayer = FindLayer("Collider");
		auto   iter = colliderLayer->renderList.begin();
		auto   iterEnd = colliderLayer->renderList.end();
		for (; iter != iterEnd;)
		{
			if (!(*iter)->GetActive())
			{
				iter = colliderLayer->renderList.erase(iter);
				iterEnd = colliderLayer->renderList.end();
				continue;
			}
			else if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}
			((CCollider3D*)(*iter).Get())->RenderDebug();
			++iter;
		}
	}
#endif // _DEBUG
}

void CRenderManager::RenderParticle(float DeltaTime)
{
	RenderLayer* particleLayer = FindLayer("Particle");
	std::list<CSceneComponent*>	renderList;
	m_AlphaBlend->SetState();
	m_vecGBuffer[2]->SetTargetShader(14);
	particleLayer->renderList.sort(CRenderManager::SortAlphaObject);
	auto	iter = particleLayer->renderList.begin();
	auto	iterEnd = particleLayer->renderList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = particleLayer->renderList.erase(iter);
			iterEnd = particleLayer->renderList.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		else if ((*iter)->GetFrustumCull())
		{
			++iter;
			continue;
		}
// 	파티클은 인스터싱 제외
		if ((*iter)->GetSceneComponentType() == SceneComponentType::Primitive && !(*iter)->CheckTypeID<CParticleComponent>())
		{
			CMesh* mesh = ((CPrimitiveComponent*)iter->Get())->GetMesh();
			if (mesh->GetRenderCount() >= 5)
			{
				CRenderInstancing* instancing = nullptr;
				instancing = FindInstancing(mesh);
				if (!instancing)
				{
					if (m_EmptyPoolList.empty())
					{
						std::vector<CRenderInstancing*>	newPool = m_vecInstancingPool;
						m_vecInstancingPool.clear();
						size_t size = newPool.size();
						m_vecInstancingPool.resize(size * 2);
						for (size_t i = 0; i < size; ++i)
						{
							m_vecInstancingPool[i] = newPool[i];
						}
						for (size_t i = size; i < size * 2; ++i)
						{
							m_vecInstancingPool[i] = new CRenderInstancing;
							m_vecInstancingPool[i]->m_PoolIndex = (int)i;
							m_EmptyPoolList.push_back((int)i);
						}
					}
					int index = m_EmptyPoolList.front();
					m_EmptyPoolList.pop_front();
					m_mapInstancing.insert(std::make_pair(mesh, m_vecInstancingPool[index]));
					m_vecInstancingPool[index]->m_Key = mesh;
					instancing = m_vecInstancingPool[index];
				}
				instancing->m_LayerName = "Particle";
				instancing->AddRenderList((CPrimitiveComponent*)iter->Get());
			}
			else
			{
				renderList.push_back(*iter);
			}
		}
		else
		{
			renderList.push_back(*iter);
		}
		++iter;
	}
	// 인스턴싱이 아닌 물체를 그려낸다.
	auto	iter1 = renderList.begin();
	auto	iter1End = renderList.end();
	for (; iter1 != iter1End; ++iter1)
	{
		(*iter1)->Render();
	}
	auto	iter2 = m_mapInstancing.begin();
	auto	iter2End = m_mapInstancing.end();
	for (; iter2 != iter2End;)
	{
		if (iter2->second->GetInstancingCount() == 0)
		{
			m_EmptyPoolList.push_back(iter2->second->GetPoolIndex());
			iter2->second->m_Key = nullptr;
			iter2 = m_mapInstancing.erase(iter2);
			iter2End = m_mapInstancing.end();
			continue;
		}
		++iter2;
	}
	iter2 = m_mapInstancing.begin();
	iter2End = m_mapInstancing.end();
	for (; iter2 != iter2End; ++iter2)
	{
		if (iter2->second->m_LayerName != "Particle")
		{
			++iter2;
			continue;
		}
		iter2->second->Render();
	}
	m_AlphaBlend->ResetState();
	m_vecGBuffer[2]->ResetTargetShader(14);
}

RenderLayer* CRenderManager::FindLayer(const std::string& name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	RenderLayer* gBufferLayer = nullptr;
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == name)
		{
			return *iter;
		}
	}
	return nullptr;
}
