#pragma once
#include "../EngineInfo.h"

class CRenderStateManager
{
private:
	friend class CRenderManager;
	std::unordered_map<std::string, CSharedPtr<class CRenderState>>	m_mapState;
	CRenderStateManager();
	~CRenderStateManager();
public:
	bool Init();
	void SetBlendFactor(const std::string& name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& name, bool blendEnable = true,	D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND srcAlphBlend = D3D11_BLEND_ONE,	D3D11_BLEND destAlphBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blendAlphOp = D3D11_BLEND_OP_ADD, UINT8 writeMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& name, bool alphaToCoverageEnable, bool independentBlendEnable);
	bool CreateDepthStencil(const std::string& name, bool depthEnable = true,
		D3D11_DEPTH_WRITE_MASK depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,	
		D3D11_COMPARISON_FUNC depthFunc = D3D11_COMPARISON_LESS,
		bool stencilEnable = false,	UINT8 stencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 stencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC frontFace = 
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC backFace = 
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });
	bool CreateState(const std::string& name, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode = D3D11_CULL_BACK,
		BOOL frontCounterClockwise = FALSE, INT depthBias = 0, FLOAT depthBiasClamp = 0.f,
		FLOAT slopeScaledDepthBias = 0.f, BOOL depthClipEnable = TRUE, BOOL scissorEnable = FALSE, 
		BOOL multisampleEnable = FALSE,	BOOL antialiasedLineEnable = FALSE);
	template <typename T>
	T* FindRenderState(const std::string& name)
	{
		auto	iter = m_mapState.find(name);

		if (iter == m_mapState.end())
			return nullptr;

		return (T*)iter->second.Get();
	}
};

