#include "ComputeShader.h"
#include "../../Device.h"
#include "../../PathManager.h"

CComputeShader::CComputeShader()
	: m_CS(nullptr)
	, m_CSBlob(nullptr)
{
	m_ShaderType = ShaderType::Compute;
}

CComputeShader::~CComputeShader()
{
	SAFE_RELEASE(m_CS);
	SAFE_RELEASE(m_CSBlob);
}

bool CComputeShader::Init()
{
	return false;
}

void CComputeShader::SetShader()
{
}

bool CComputeShader::LoadComputeShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(m_CS);
	SAFE_RELEASE(m_CSBlob);
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
	ID3DBlob* errorBlob = nullptr;	// ErrorBlob�� ������ �߻����� ��� ������ ������ �����ϱ� ���� �����̴�.
	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "cs_5_0", 
		flag, 0, &m_CSBlob, &errorBlob)))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}
	// ������ �������� �ڵ带 �̿��ؼ� VertexShader ��ü�� ������.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateComputeShader(
		m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize(), nullptr, &m_CS)))
	{
		return false;
	}
	return true;
}
/*
	excute ������ ���̴��� ���������������ο� �ִ� ���̴��� ������ �ϸ� �˾Ƽ� ó�������� ��ǻƮ���̴��� 
	�����̱� ������ �� �Լ��� �ʿ��ϴ�
	��ǻƮ ���̴��� �˾Ƽ� ��Ƽ�����尡 ���ư���
	�׶� ������ ��ġ�� x,y,z�� �����Ѵ�
*/
void CComputeShader::Excute(unsigned int x, unsigned int y, unsigned int z)
{
	CDevice::GetInst()->GetContext()->CSSetShader(m_CS, nullptr, 0);
	CDevice::GetInst()->GetContext()->Dispatch(x, y, z);
	//CDevice::GetInst()->GetContext()->CSSetShader(nullptr, nullptr, 0);
}
