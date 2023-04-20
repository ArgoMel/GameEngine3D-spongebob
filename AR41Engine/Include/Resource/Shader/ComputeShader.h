#pragma once
#include "Shader.h"
class CComputeShader :public CShader
{
private:
    friend class CShaderManager;
    ID3D11ComputeShader* m_CS;
    ID3DBlob* m_CSBlob;
protected:
    CComputeShader();
    virtual ~CComputeShader() = 0;
public:
    virtual bool Init() = 0;
    virtual void SetShader();
    bool LoadComputeShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
    void Excute(unsigned int x, unsigned int y, unsigned int z);
};

