#pragma once
#include "RenderState.h"

class CRasterizerState : public CRenderState
{
protected:
	friend class CRenderStateManager;
	unsigned int	m_StencilRef;
	unsigned int	m_PrevStencilRef;
	CRasterizerState();
	virtual ~CRasterizerState();
public:
	bool CreateState(D3D11_FILL_MODE fillMode,
		D3D11_CULL_MODE cullMode = D3D11_CULL_BACK,	//컬링모드
		BOOL frontCounterClockwise = FALSE,			//시계방향인지 여부
		INT depthBias = 0, FLOAT depthBiasClamp = 0.f,	//깊이 관련인자
		FLOAT slopeScaledDepthBias = 0.f,
		BOOL depthClipEnable = TRUE,				//깊이과련해서 자를지 말지(클립핑)
		BOOL scissorEnable = FALSE, BOOL multisampleEnable = FALSE,
		BOOL antialiasedLineEnable = FALSE);
	virtual void SetState();
	virtual void ResetState();
};

