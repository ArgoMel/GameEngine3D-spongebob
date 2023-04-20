#include "StructuredBuffer.h"
#include "../../Device.h"

CStructuredBuffer::CStructuredBuffer()	
	: m_Desc{}
	, m_Buffer(nullptr)
	, m_SRV(nullptr)
	, m_UAV(nullptr)
	, m_Size(0)
	, m_Count(0)
	, m_StructuredBufferShaderType(0)
	, m_Register(0)
	, m_Dynamic(false)
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& buffer)
{
	*this = buffer;
	CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer);
	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;
	assert(m_Buffer);
	CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV);
	if (!m_Dynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;
		CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV);
	}
}

CStructuredBuffer::~CStructuredBuffer()
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);
}

bool CStructuredBuffer::Init(const std::string& name, unsigned int size, unsigned int count, int reg, bool dynamic, 
	int structuredBufferShaderType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);
	m_Dynamic = dynamic;
	m_Name = name;
	m_Size = size;
	m_Count = count;
	m_Register = reg;
	m_StructuredBufferShaderType = structuredBufferShaderType;
	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;
	if (m_Dynamic)
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.Usage = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
	}
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
	{
		return false;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV)))
	{
		return false;
	}
	if (!m_Dynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV)))
		{
			return false;
		}
	}
	return true;
}

bool CStructuredBuffer::Init(const std::string& name, unsigned int size, unsigned int count, int reg, D3D11_USAGE usage, 
	UINT bindFlag, UINT cpuFlag, bool dynamic, int structuredBufferShaderType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);
	m_Dynamic = dynamic;
	m_Name = name;
	m_Size = size;
	m_Count = count;
	m_Register = reg;
	m_StructuredBufferShaderType = structuredBufferShaderType;
	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;
	m_Desc.BindFlags = bindFlag;
	m_Desc.Usage = usage;
	m_Desc.CPUAccessFlags = cpuFlag;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
	{
		return false;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV)))
	{
		return false;
	}
	D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.Flags = 0;
	UAVDesc.Buffer.NumElements = m_Count;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV)))
	{
		return false;
	}
	return true;
}

void CStructuredBuffer::UpdateBuffer(void* data, int count)
{
	if (!m_Dynamic)
	{
		return;
	}
	D3D11_MAPPED_SUBRESOURCE	map = {};
	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, data, m_Size * count);
	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);
}

CStructuredBuffer* CStructuredBuffer::Clone()
{
	return new CStructuredBuffer(*this);
}

void CStructuredBuffer::SetShader()
{
	if (m_Dynamic)
	{
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(m_Register, 1, &m_SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(m_Register, 1, &m_SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(m_Register, 1, &m_SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Domain)
		{
			CDevice::GetInst()->GetContext()->DSSetShaderResources(m_Register, 1, &m_SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(m_Register, 1, &m_SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(m_Register, 1, &m_SRV);
		}
	}
	else
	{
		unsigned int count = -1;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_Register, 1, &m_UAV, &count);
	}
}

void CStructuredBuffer::ResetShader()
{
	if (m_Dynamic)
	{
		ID3D11ShaderResourceView* SRV = nullptr;
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Vertex)
		{
			CDevice::GetInst()->GetContext()->VSSetShaderResources(m_Register, 1, &SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Pixel)
		{
			CDevice::GetInst()->GetContext()->PSSetShaderResources(m_Register, 1, &SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Hull)
		{
			CDevice::GetInst()->GetContext()->HSSetShaderResources(m_Register, 1, &SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Domain)
		{
			CDevice::GetInst()->GetContext()->DSSetShaderResources(m_Register, 1, &SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Geometry)
		{
			CDevice::GetInst()->GetContext()->GSSetShaderResources(m_Register, 1, &SRV);
		}
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Compute)
		{
			CDevice::GetInst()->GetContext()->CSSetShaderResources(m_Register, 1, &SRV);
		}
	}
	else
	{
		unsigned int count = -1;
		ID3D11UnorderedAccessView* UAV = nullptr;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_Register, 1, &UAV, &count);
	}
}

void CStructuredBuffer::SetShader(int reg, int structuredBufferShaderType)
{
	if (structuredBufferShaderType & (int)EShaderBufferType::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &m_SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &m_SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &m_SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetShaderResources(reg, 1, &m_SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &m_SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &m_SRV);
	}
}

void CStructuredBuffer::ResetShader(int reg, int structuredBufferShaderType)
{
	ID3D11ShaderResourceView* SRV = nullptr;
	if (structuredBufferShaderType & (int)EShaderBufferType::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetShaderResources(reg, 1, &SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetShaderResources(reg, 1, &SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetShaderResources(reg, 1, &SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetShaderResources(reg, 1, &SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetShaderResources(reg, 1, &SRV);
	}
	if (structuredBufferShaderType & (int)EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetShaderResources(reg, 1, &SRV);
	}
}

void CStructuredBuffer::CopyData(void* data)
{
	D3D11_MAPPED_SUBRESOURCE	map = {};
	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_READ, 0, &map);
	memcpy(data, map.pData, m_Size * m_Count);
	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);
}

void CStructuredBuffer::CopyResource(CStructuredBuffer* buffer)
{
	CDevice::GetInst()->GetContext()->CopyResource(buffer->m_Buffer, m_Buffer);
}
