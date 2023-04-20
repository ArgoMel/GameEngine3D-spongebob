#include "ConstantBuffer.h"
#include "../../Device.h"

CConstantBuffer::CConstantBuffer()
	: m_Buffer(nullptr)
	, m_Size(0)
	, m_Register(0)
	, m_ShaderBufferType(0)
{
}

CConstantBuffer::~CConstantBuffer()
{
	SAFE_RELEASE(m_Buffer);
}

bool CConstantBuffer::Init(int size, int reg, int shaderBufferType)
{
	m_Size = size;
	m_Register = reg;
	m_ShaderBufferType = shaderBufferType;
	D3D11_BUFFER_DESC	desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, &m_Buffer)))
	{
		return false;
	}
	return true;
}

void CConstantBuffer::UpdateBuffer(void* data)
{
	D3D11_MAPPED_SUBRESOURCE	map = {};
	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, data, m_Size);
	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);
	if (m_ShaderBufferType & (int)EShaderBufferType::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetConstantBuffers(m_Register, 1, &m_Buffer);
	}
	if (m_ShaderBufferType & (int)EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetConstantBuffers(m_Register, 1, &m_Buffer);
	}
	if (m_ShaderBufferType & (int)EShaderBufferType::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetConstantBuffers(m_Register, 1, &m_Buffer);
	}
	if (m_ShaderBufferType & (int)EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetConstantBuffers(m_Register, 1, &m_Buffer);
	}
	if (m_ShaderBufferType & (int)EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetConstantBuffers(m_Register, 1, &m_Buffer);
	}
	if (m_ShaderBufferType & (int)EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetConstantBuffers(m_Register, 1, &m_Buffer);
	}
}