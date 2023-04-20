#include "GraphicShader.h"
#include "../../Device.h"
#include "../../PathManager.h"

CGraphicShader::CGraphicShader()
	: m_VS(nullptr)
	, m_VSBlob(nullptr)
	, m_HS(nullptr)
	, m_HSBlob(nullptr)
	, m_DS(nullptr)
	, m_DSBlob(nullptr)
	, m_GS(nullptr)
	, m_GSBlob(nullptr)
	, m_PS(nullptr)
	, m_PSBlob(nullptr)
	, m_InputSize(0)
	, m_InputLayout(nullptr)
{
}

CGraphicShader::~CGraphicShader()
{
	SAFE_RELEASE(m_InputLayout);
	SAFE_RELEASE(m_VS);
	SAFE_RELEASE(m_PS);
	SAFE_RELEASE(m_HS);
	SAFE_RELEASE(m_DS);
	SAFE_RELEASE(m_GS);
	SAFE_RELEASE(m_VSBlob);
	SAFE_RELEASE(m_PSBlob);
	SAFE_RELEASE(m_HSBlob);
	SAFE_RELEASE(m_DSBlob);
	SAFE_RELEASE(m_GSBlob);
}

void CGraphicShader::SetShader()
{
	CDevice::GetInst()->GetContext()->VSSetShader(m_VS, nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(m_PS, nullptr, 0);
	CDevice::GetInst()->GetContext()->HSSetShader(m_HS, nullptr, 0);
	CDevice::GetInst()->GetContext()->DSSetShader(m_DS, nullptr, 0);
	CDevice::GetInst()->GetContext()->GSSetShader(m_GS, nullptr, 0);
	CDevice::GetInst()->GetContext()->IASetInputLayout(m_InputLayout);
}

void CGraphicShader::AddInputDesc(const char* semanticName, unsigned int semanticIndex, DXGI_FORMAT fmt, unsigned int slot, 
	unsigned int size, D3D11_INPUT_CLASSIFICATION slotClass, unsigned int instanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC	desc = {};
	desc.SemanticName = semanticName;
	desc.SemanticIndex = semanticIndex;
	desc.Format = fmt;
	desc.InputSlot = slot;
	desc.InputSlotClass = slotClass;
	desc.InstanceDataStepRate = instanceDataStepRate;
	desc.AlignedByteOffset = m_InputSize;
	m_InputSize += size;
	m_vecInputDesc.push_back(desc);
}

bool CGraphicShader::CreateInputLayout()
{
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(&m_vecInputDesc[0], (UINT)m_vecInputDesc.size(),
		m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), &m_InputLayout)))
	{
		return false;
	}
	return true;
}

bool CGraphicShader::LoadVertexShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(m_VS);
	SAFE_RELEASE(m_VSBlob);
	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	if (info)
	{
		lstrcpy(fullPath, info->path);
	}
	lstrcat(fullPath, fileName);
	ID3DBlob* errorBlob = nullptr;		// ErrorBlob은 에러가 발생했을 경우 에러의 내용을 저장하기 위한 버퍼이다.
	if (FAILED(D3DCompileFromFile(fullPath, nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		entryName, "vs_5_0",flag, 0, &m_VSBlob,&errorBlob)))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(
		m_VSBlob->GetBufferPointer(),m_VSBlob->GetBufferSize(), nullptr, &m_VS)))
	{
		return false;
	}
	return true;
}

bool CGraphicShader::LoadHullShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(m_HS);
	SAFE_RELEASE(m_HSBlob);
	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	if (info)
	{
		lstrcpy(fullPath, info->path);
	}
	lstrcat(fullPath, fileName);
	ID3DBlob* errorBlob = nullptr;
	if (FAILED(D3DCompileFromFile(fullPath, nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		entryName, "hs_5_0",flag, 0, &m_HSBlob, &errorBlob)))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateHullShader(
		m_HSBlob->GetBufferPointer(), m_HSBlob->GetBufferSize(), nullptr, &m_HS)))
	{
		return false;
	}
	return true;
}

bool CGraphicShader::LoadDomainShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(m_DS);
	SAFE_RELEASE(m_DSBlob);
	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	if (info)
	{
		lstrcpy(fullPath, info->path);
	}
	lstrcat(fullPath, fileName);
	ID3DBlob* errorBlob = nullptr;
	if (FAILED(D3DCompileFromFile(fullPath, nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName, "ds_5_0",flag, 0, &m_DSBlob, &errorBlob)))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDomainShader(
		m_DSBlob->GetBufferPointer(), m_DSBlob->GetBufferSize(), nullptr, &m_DS)))
	{
		return false;
	}
	return true;
}

bool CGraphicShader::LoadGeometryShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(m_GS);
	SAFE_RELEASE(m_GSBlob);
	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	if (info)
	{
		lstrcpy(fullPath, info->path);
	}
	lstrcat(fullPath, fileName);
	ID3DBlob* errorBlob = nullptr;
	if (FAILED(D3DCompileFromFile(fullPath, nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		entryName, "gs_5_0",flag, 0, &m_GSBlob, &errorBlob)))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateGeometryShader(
		m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr, &m_GS)))
	{
		return false;
	}
	return true;
}

bool CGraphicShader::LoadPixelShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(m_PS);
	SAFE_RELEASE(m_PSBlob);
	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG
	TCHAR	fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	if (info)
	{
		lstrcpy(fullPath, info->path);
	}
	lstrcat(fullPath, fileName);
	ID3DBlob* errorBlob = nullptr;
	if (FAILED(D3DCompileFromFile(fullPath, nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		entryName, "ps_5_0",flag, 0, &m_PSBlob, &errorBlob)))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(
		m_PSBlob->GetBufferPointer(),m_PSBlob->GetBufferSize(), nullptr, &m_PS)))
	{
		return false;
	}
	return true;
}
