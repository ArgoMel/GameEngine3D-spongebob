#pragma once
#include "GraphicShader.h"

class CDebugShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CDebugShader();
    virtual ~CDebugShader();
public:
    virtual bool Init();
};

