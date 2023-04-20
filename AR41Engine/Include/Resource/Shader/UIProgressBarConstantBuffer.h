#pragma once
#include "ConstantBufferData.h"

class CUIProgressBarConstantBuffer : public CConstantBufferData
{
private:
	UIProgressBarCBuffer	m_BufferData;
public:
	void SetBarDir(EProgressBarDir dir)
	{
		m_BufferData.barDir = (int)dir;
	}
	void SetPercent(float percent)
	{
		m_BufferData.percent = percent;
	}
	CUIProgressBarConstantBuffer();
	CUIProgressBarConstantBuffer(const CUIProgressBarConstantBuffer& buffer);
	virtual ~CUIProgressBarConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CUIProgressBarConstantBuffer* Clone();
};

