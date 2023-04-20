#pragma once
#include "../../EngineInfo.h"
#include "ConstantBuffer.h"

class CConstantBufferData
{
protected:
	CSharedPtr<class CConstantBuffer>	m_Buffer;
	void SetConstantBuffer(const std::string& name);
public:
	CConstantBufferData();
	CConstantBufferData(const CConstantBufferData& buffer);
	virtual ~CConstantBufferData();
	virtual bool Init() = 0;
	virtual void UpdateBuffer() = 0;
	virtual CConstantBufferData* Clone() = 0;
};
