#pragma once
#include "ConstantBufferData.h"

class CInstancingConstantBuffer : public CConstantBufferData
{
private:
	InstancingCBuffer	m_BufferData;
public:
	void SetBoneCount(int count)
	{
		m_BufferData.boneCount = count;
	}
	CInstancingConstantBuffer();
	CInstancingConstantBuffer(const CInstancingConstantBuffer& buffer);
	virtual ~CInstancingConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CInstancingConstantBuffer* Clone();
};

