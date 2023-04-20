#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

CRenderStateManager::CRenderStateManager()
{
}

CRenderStateManager::~CRenderStateManager()
{
}

bool CRenderStateManager::Init()
{
	AddBlendInfo("AlphaBlend");
	CreateBlendState("AlphaBlend", true, false);
	AddBlendInfo("MRTAlphaBlend");
	CreateBlendState("MRTAlphaBlend", false, true);	//false하면 알파끼리 깊이 비교하는데 그걸 안함
//1,1,1,1 원복색깔 그대로 받아와서 블렌드한다는 뜻이다
	AddBlendInfo("LightAccBlend",true, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	CreateBlendState("LightAccBlend", true, false);
	CreateDepthStencil("DepthDisable", false);
	CreateDepthStencil("DepthLessEqual", true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);
	CreateDepthStencil("DepthWriteDisable", true, D3D11_DEPTH_WRITE_MASK_ZERO);
	CreateState("FrontFaceCull", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	CreateState("BackFaceCull", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_BACK);
	CreateState("WireFrame", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
	return true;
}

void CRenderStateManager::SetBlendFactor(const std::string& name, float r, float g, float b, float a)
{
	CBlendState* state = FindRenderState<CBlendState>(name);
	if (!state)
	{
		state = new CBlendState;
		state->SetName(name);
		m_mapState.insert(std::make_pair(name, state));
	}
	state->SetBlendFactor(r, g, b, a);
}

void CRenderStateManager::AddBlendInfo(const std::string& name, bool blendEnable, D3D11_BLEND srcBlend, 
	D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphBlend, D3D11_BLEND destAlphBlend, 
	D3D11_BLEND_OP blendAlphOp, UINT8 writeMask)
{
	CBlendState* state = FindRenderState<CBlendState>(name);
	if (!state)
	{
		state = new CBlendState;
		state->SetName(name);
		m_mapState.insert(std::make_pair(name, state));
	}
	state->AddBlendInfo(blendEnable, srcBlend, destBlend, blendOp, srcAlphBlend, destAlphBlend, 
		blendAlphOp, writeMask);
}

bool CRenderStateManager::CreateBlendState(const std::string& name, bool alphaToCoverageEnable, bool independentBlendEnable)
{
	CBlendState* state = FindRenderState<CBlendState>(name);
	if (!state)
	{
		return false;
	}
	if (!state->CreateState(alphaToCoverageEnable, independentBlendEnable))
	{
		SAFE_RELEASE(state);
		return false;
	}
	return true;
}

bool CRenderStateManager::CreateDepthStencil(const std::string& name, bool depthEnable, 
	D3D11_DEPTH_WRITE_MASK depthWriteMask, D3D11_COMPARISON_FUNC depthFunc, bool stencilEnable, 
	UINT8 stencilReadMask, UINT8 stencilWriteMask, D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace)
{
	CDepthStencilState* state = FindRenderState<CDepthStencilState>(name);
	if (state)
	{
		return true;
	}
	state = new CDepthStencilState;
	state->SetName(name);
	if (!state->CreateDepthStencil(depthEnable, depthWriteMask, depthFunc, 
		stencilEnable, stencilReadMask, stencilWriteMask, frontFace, backFace))
	{
		SAFE_RELEASE(state);
		return false;
	}
	m_mapState.insert(std::make_pair(name, state));
	return true;
}

bool CRenderStateManager::CreateState(const std::string& name, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode, 
	BOOL frontCounterClockwise, INT depthBias, FLOAT depthBiasClamp, FLOAT slopeScaledDepthBias, BOOL depthClipEnable, 
	BOOL scissorEnable, BOOL multisampleEnable, BOOL antialiasedLineEnable)
{
	CRasterizerState* state = FindRenderState<CRasterizerState>(name);
	if (state)
	{
		return true;
	}
	state = new CRasterizerState;
	state->SetName(name);
	if (!state->CreateState(fillMode, cullMode, frontCounterClockwise, depthBias, depthBiasClamp, 
		slopeScaledDepthBias, depthClipEnable, scissorEnable, multisampleEnable, antialiasedLineEnable))
	{
		SAFE_RELEASE(state);
		return false;
	}
	m_mapState.insert(std::make_pair(name, state));
	return true;
}
