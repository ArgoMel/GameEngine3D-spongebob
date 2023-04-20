#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()
	: m_SampleMask(0xffffffff)
	, m_PrevSampleMask(0)
	, m_BlendFactor{}
	, m_PrevBlendFactor{}
{
	m_Type = ERenderStateType::Blend;
	SetTypeID<CBlendState>();
}

CBlendState::~CBlendState()
{
}

void CBlendState::AddBlendInfo(bool blendEnable, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp,
	D3D11_BLEND srcAlphBlend, D3D11_BLEND destAlphBlend, D3D11_BLEND_OP blendAlphOp, UINT8 writeMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC	desc = {};
	desc.BlendEnable = blendEnable;
	desc.SrcBlend = srcBlend;
	desc.DestBlend = destBlend;
	desc.BlendOp = blendOp;
	desc.SrcBlendAlpha = srcAlphBlend;
	desc.DestBlendAlpha = destAlphBlend;
	desc.BlendOpAlpha = blendAlphOp;
	desc.RenderTargetWriteMask = writeMask;
	m_vecDesc.push_back(desc);
}

bool CBlendState::CreateState(bool alphaToCoverageEnable, bool independentBlendEnable)
{
	if (m_vecDesc.empty())
	{
		return false;
	}
	D3D11_BLEND_DESC	desc = {};
	desc.AlphaToCoverageEnable = alphaToCoverageEnable;
	desc.IndependentBlendEnable = independentBlendEnable;
	for (int i = 0; i < 8; ++i)
	{
		desc.RenderTarget[i].BlendEnable = false;
		desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
		desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	memcpy(desc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&desc, (ID3D11BlendState**)&m_State)))
	{
		return false;
	}
	return true;
}

void CBlendState::SetState()
{
	CDevice::GetInst()->GetContext()->OMGetBlendState((ID3D11BlendState**)&m_PrevState, m_PrevBlendFactor, 
		&m_PrevSampleMask);
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_State, m_BlendFactor, m_SampleMask);
}

void CBlendState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_PrevState, m_PrevBlendFactor, m_PrevSampleMask);
	SAFE_RELEASE(m_PrevState);
}
