#pragma once
#include "RenderState.h"

class CBlendState : public CRenderState
{
protected:
	friend class CRenderStateManager;
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecDesc;
	unsigned int		m_SampleMask;
	unsigned int		m_PrevSampleMask;
	float				m_BlendFactor[4];
	float				m_PrevBlendFactor[4];
	CBlendState();
	virtual ~CBlendState();
public:
	void SetBlendFactor(float r, float g, float b, float a)
	{
		m_BlendFactor[0] = a;
		m_BlendFactor[1] = r;
		m_BlendFactor[2] = g;
		m_BlendFactor[3] = b;
	}
	void AddBlendInfo(bool blendEnable = true, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND srcAlphBlend = D3D11_BLEND_ONE, D3D11_BLEND destAlphBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blendAlphOp = D3D11_BLEND_OP_ADD, UINT8 writeMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateState(bool alphaToCoverageEnable, bool independentBlendEnable);
	virtual void SetState();
	virtual void ResetState();
};

