#pragma once
#include "GraphicShader.h"

class CPSShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CPSShader();
    virtual ~CPSShader();
public:
    virtual bool Init();
};

