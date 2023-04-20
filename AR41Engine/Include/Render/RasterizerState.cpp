#include "RasterizerState.h"
#include "../Device.h"

CRasterizerState::CRasterizerState()
	: m_StencilRef(0)
	, m_PrevStencilRef(0)
{
	m_Type = ERenderStateType::Rasterizer;
	SetTypeID<CRasterizerState>();
}

CRasterizerState::~CRasterizerState()
{
}

bool CRasterizerState::CreateState(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode, BOOL frontCounterClockwise, 
	INT depthBias, FLOAT depthBiasClamp, FLOAT slopeScaledDepthBias, BOOL depthClipEnable, 
	BOOL scissorEnable, BOOL multisampleEnable, BOOL antialiasedLineEnable)
{
	D3D11_RASTERIZER_DESC	desc = {};
	desc.FillMode = fillMode;
	desc.CullMode = cullMode;
	desc.FrontCounterClockwise = frontCounterClockwise;
	desc.DepthBias = depthBias;
	desc.DepthBiasClamp = depthBiasClamp;
	desc.SlopeScaledDepthBias = slopeScaledDepthBias;
	desc.DepthClipEnable = depthClipEnable;
	desc.ScissorEnable = scissorEnable;
	desc.MultisampleEnable = multisampleEnable;
	desc.AntialiasedLineEnable = antialiasedLineEnable;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRasterizerState(&desc, (ID3D11RasterizerState**)&m_State)))
	{
		return false;
	}
	return true;
}

void CRasterizerState::SetState()
{
	CDevice::GetInst()->GetContext()->RSGetState((ID3D11RasterizerState**)&m_PrevState);
	CDevice::GetInst()->GetContext()->RSSetState((ID3D11RasterizerState*)m_State);
}

void CRasterizerState::ResetState()
{
	CDevice::GetInst()->GetContext()->RSSetState((ID3D11RasterizerState*)m_PrevState);
	SAFE_RELEASE(m_PrevState);
}
