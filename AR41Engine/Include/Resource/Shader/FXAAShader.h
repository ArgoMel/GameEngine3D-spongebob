#pragma once
#include "GraphicShader.h"

class CFXAAShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CFXAAShader();
    virtual ~CFXAAShader();
public:
    virtual bool Init();
};

