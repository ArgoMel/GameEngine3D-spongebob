#pragma once
#include "GraphicShader.h"

class CLightCelShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CLightCelShader();
    virtual ~CLightCelShader();
public:
    virtual bool Init();
};

