#pragma once
#include "GraphicShader.h"

class CDecalDebugShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CDecalDebugShader();
    virtual ~CDecalDebugShader();
public:
    virtual bool Init();
};

