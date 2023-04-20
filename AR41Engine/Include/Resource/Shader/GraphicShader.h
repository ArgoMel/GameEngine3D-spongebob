#pragma once
#include "Shader.h"
class CGraphicShader : public CShader
{
private:
    friend class CShaderManager;
    ID3D11InputLayout* m_InputLayout;
    std::vector<D3D11_INPUT_ELEMENT_DESC>   m_vecInputDesc;
    ID3D11VertexShader* m_VS;   //���� ���̴�
    ID3DBlob* m_VSBlob;         // VertexShader �ڵ带 ������ ���� ����
    ID3D11HullShader* m_HS;     //���� ���̴�
    ID3DBlob* m_HSBlob;
    ID3D11DomainShader* m_DS;   //���� ���̴�
    ID3DBlob* m_DSBlob;
    ID3D11GeometryShader* m_GS; //���� ���̴�
    ID3DBlob* m_GSBlob;
    ID3D11PixelShader* m_PS;    //���� ���̴�
    ID3DBlob* m_PSBlob;
    unsigned int    m_InputSize;
protected:
    CGraphicShader();
    virtual ~CGraphicShader() = 0;
public:
    virtual bool Init() = 0;
    virtual void SetShader();
    void AddInputDesc(const char* semanticName, unsigned int semanticIndex,DXGI_FORMAT fmt, unsigned int slot, unsigned int size,
        D3D11_INPUT_CLASSIFICATION slotClass,unsigned int instanceDataStepRate);
    bool CreateInputLayout();
    bool LoadVertexShader(const char* entryName, const TCHAR* fileName,const std::string& pathName);
    bool LoadHullShader(const char* entryName, const TCHAR* fileName,const std::string& pathName);
    bool LoadDomainShader(const char* entryName, const TCHAR* fileName,const std::string& pathName);
    bool LoadGeometryShader(const char* entryName, const TCHAR* fileName,const std::string& pathName);
    bool LoadPixelShader(const char* entryName, const TCHAR* fileName,const std::string& pathName);
};

