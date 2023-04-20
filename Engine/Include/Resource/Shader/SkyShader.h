#pragma once
#include "GraphicShader.h"

class CSkyShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CSkyShader();
    virtual ~CSkyShader();
public:
    virtual bool Init();
};

