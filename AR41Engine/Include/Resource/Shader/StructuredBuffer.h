#pragma once
#include "../../EngineInfo.h"

class CStructuredBuffer
{
private:
	D3D11_BUFFER_DESC	m_Desc;
	ID3D11Buffer* m_Buffer;
	ID3D11ShaderResourceView* m_SRV;
	ID3D11UnorderedAccessView* m_UAV;
	std::string			m_Name;
	unsigned int	m_Size;
	unsigned int	m_Count;
	int				m_StructuredBufferShaderType;
	int				m_Register;
	bool			m_Dynamic;
public:
	CStructuredBuffer();
	CStructuredBuffer(const CStructuredBuffer& buffer);
	~CStructuredBuffer();
	ID3D11Buffer* GetBuffer()	const
	{
		return m_Buffer;
	}
	const std::string& GetName()	const
	{
		return m_Name;
	}
	bool Init(const std::string& name, unsigned int size, unsigned int count, int reg, bool dynamic = false, 
		int structuredBufferShaderType = (int)EShaderBufferType::Compute);
	bool Init(const std::string& name, unsigned int size, unsigned int count, int reg, D3D11_USAGE usage, 
		UINT bindFlag, UINT cpuFlag, bool dynamic = false, 
		int structuredBufferShaderType = (int)EShaderBufferType::Compute);
	void UpdateBuffer(void* data, int count);
	CStructuredBuffer* Clone();
	void SetShader();
	void ResetShader();
	void SetShader(int reg, int structuredBufferShaderType);
	void ResetShader(int reg, int structuredBufferShaderType);
	void CopyData(void* data);
	void CopyResource(CStructuredBuffer* buffer);
};

