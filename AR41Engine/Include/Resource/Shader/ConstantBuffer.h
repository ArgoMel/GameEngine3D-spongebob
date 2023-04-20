#pragma once
#include "../../Ref.h"

class CConstantBuffer :public CRef
{
private:
	friend class CShaderManager;
	ID3D11Buffer* m_Buffer;
	int		m_Size;
	int		m_Register;
	int		m_ShaderBufferType;
	CConstantBuffer();
	~CConstantBuffer();
public:
	bool Init(int size, int reg, int shaderBufferType);
	void UpdateBuffer(void* data);
};