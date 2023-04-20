#pragma once
#include "ComputeShader.h"

class CAnimationUpdateShader : public CComputeShader
{
protected:
    friend class CShaderManager;
    CAnimationUpdateShader();
    virtual ~CAnimationUpdateShader();
public:
    virtual bool Init();
};

