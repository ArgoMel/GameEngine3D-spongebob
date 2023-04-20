#pragma once
#include "GraphicShader.h"

class CUIProgressBarShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CUIProgressBarShader();
    virtual ~CUIProgressBarShader();
public:
    virtual bool Init();
};

