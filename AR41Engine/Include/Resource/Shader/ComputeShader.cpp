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
	ID3DBlob* errorBlob = nullptr;	// ErrorBlob은 에러가 발생했을 경우 에러의 내용을 저장하기 위한 버퍼이다.
	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "cs_5_0", 
		flag, 0, &m_CSBlob, &errorBlob)))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}
	// 위에서 컴파일한 코드를 이용해서 VertexShader 객체를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateComputeShader(
		m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize(), nullptr, &m_CS)))
	{
		return false;
	}
	return true;
}
/*
	excute 기존에 쉐이더는 렌더링파이프라인에 있는 쉐이더에 지정만 하면 알아서 처리됐지만 컴퓨트쉐이더는 
	별도이기 떄문에 이 함수가 필요하다
	컴퓨트 쉐이더는 알아서 멀티스레드가 돌아간다
	그때 쓰레드 위치를 x,y,z로 관리한다
*/
void CComputeShader::Excute(unsigned int x, unsigned int y, unsigned int z)
{
	CDevice::GetInst()->GetContext()->CSSetShader(m_CS, nullptr, 0);
	CDevice::GetInst()->GetContext()->Dispatch(x, y, z);
	//CDevice::GetInst()->GetContext()->CSSetShader(nullptr, nullptr, 0);
}
