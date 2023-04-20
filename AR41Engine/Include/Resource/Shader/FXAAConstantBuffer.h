#pragma once
#include "ConstantBufferData.h"

class CFXAAConstantBuffer : public CConstantBufferData
{
private:
	FXAACBuffer	m_BufferData;
public:
	CFXAAConstantBuffer();
	CFXAAConstantBuffer(const CFXAAConstantBuffer& buffer);
	virtual ~CFXAAConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CFXAAConstantBuffer* Clone();
};

