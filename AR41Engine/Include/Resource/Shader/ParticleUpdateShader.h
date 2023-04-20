#pragma once
#include "ComputeShader.h"

class CParticleUpdateShader : public CComputeShader
{
protected:
    friend class CShaderManager;
    CParticleUpdateShader();
    virtual ~CParticleUpdateShader();
public:
    virtual bool Init();
};
