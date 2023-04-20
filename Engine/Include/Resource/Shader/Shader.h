#pragma once
#include "../../Ref.h"

class CShader :public CRef
{
protected:
    friend class CShaderManager;
    ShaderType  m_ShaderType;
    CShader();
    virtual ~CShader() = 0;
public:
    ShaderType GetShaderType()  const
    {
        return m_ShaderType;
    }
    virtual bool Init() = 0;
    virtual void SetShader() = 0;
};
