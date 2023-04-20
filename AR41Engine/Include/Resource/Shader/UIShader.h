#pragma once
#include "GraphicShader.h"

class CUIShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CUIShader();
    virtual ~CUIShader();
public:
    virtual bool Init();
};

