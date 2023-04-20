#pragma once
#include "GraphicShader.h"

class CScreenShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CScreenShader();
    virtual ~CScreenShader();
public:
    virtual bool Init();
};

