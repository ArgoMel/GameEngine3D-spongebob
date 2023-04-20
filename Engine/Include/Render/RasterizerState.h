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
		D3D11_CULL_MODE cullMode = D3D11_CULL_BACK,	//�ø����
		BOOL frontCounterClockwise = FALSE,			//�ð�������� ����
		INT depthBias = 0, FLOAT depthBiasClamp = 0.f,	//���� ��������
		FLOAT slopeScaledDepthBias = 0.f,
		BOOL depthClipEnable = TRUE,				//���̰����ؼ� �ڸ��� ����(Ŭ����)
		BOOL scissorEnable = FALSE, BOOL multisampleEnable = FALSE,
		BOOL antialiasedLineEnable = FALSE);
	virtual void SetState();
	virtual void ResetState();
};

