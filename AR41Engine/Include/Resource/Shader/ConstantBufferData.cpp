#include "ConstantBufferData.h"
#include "../ResourceManager.h"

CConstantBufferData::CConstantBufferData()
{
}

CConstantBufferData::CConstantBufferData(const CConstantBufferData& buffer)
{
	m_Buffer = buffer.m_Buffer;
}

CConstantBufferData::~CConstantBufferData()
{
}

void CConstantBufferData::SetConstantBuffer(const std::string& name)
{
	m_Buffer = CResourceManager::GetInst()->FindConstantBuffer(name);
}
