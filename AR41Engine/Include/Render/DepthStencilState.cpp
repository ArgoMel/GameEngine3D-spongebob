#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState()
{
	m_Type = ERenderStateType::DepthStencil;
	SetTypeID<CDepthStencilState>();
	m_StencilRef = 0xffffffff;
	m_PrevStencilRef = 0xffffffff;
}

CDepthStencilState::~CDepthStencilState()
{
}

bool CDepthStencilState::CreateDepthStencil(bool depthEnable, D3D11_DEPTH_WRITE_MASK depthWriteMask, 
	D3D11_COMPARISON_FUNC depthFunc, bool stencilEnable, UINT8 stencilReadMask, UINT8 stencilWriteMask, 
	D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace)
{
	D3D11_DEPTH_STENCIL_DESC	desc = {};
	desc.DepthEnable = depthEnable;
	desc.DepthWriteMask = depthWriteMask;
	desc.DepthFunc = depthFunc;
	desc.StencilEnable = stencilEnable;
	desc.StencilReadMask = stencilReadMask;
	desc.StencilWriteMask = stencilWriteMask;
	desc.FrontFace = frontFace;
	desc.BackFace = backFace;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilState(&desc, (ID3D11DepthStencilState**)&m_State)))
	{
		return false;
	}
	return true;
}

void CDepthStencilState::SetState()
{
	CDevice::GetInst()->GetContext()->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_PrevState, &m_PrevStencilRef);
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)m_State, m_StencilRef);
}

void CDepthStencilState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)m_PrevState, m_PrevStencilRef);
	SAFE_RELEASE(m_PrevState);
}
