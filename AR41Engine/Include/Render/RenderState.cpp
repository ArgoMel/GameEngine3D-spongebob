#include "RenderState.h"

CRenderState::CRenderState() 
	: m_State(nullptr)
	, m_PrevState(nullptr)
	, m_Type(ERenderStateType::Blend)
{
}

CRenderState::~CRenderState()
{
	SAFE_RELEASE(m_State);
	SAFE_RELEASE(m_PrevState);
}
