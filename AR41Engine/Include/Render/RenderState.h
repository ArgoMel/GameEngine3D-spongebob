#pragma once
#include "../Ref.h"

class CRenderState : public CRef
{
protected:
	friend class CRenderStateManager;
	ID3D11DeviceChild* m_State;
	ID3D11DeviceChild* m_PrevState;
	ERenderStateType	m_Type;
	CRenderState();
	virtual ~CRenderState();
public:
	ERenderStateType GetType()	const
	{
		return m_Type;
	}
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};

