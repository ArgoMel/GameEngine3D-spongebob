#pragma once
#include "GraphicShader.h"

class CDecalShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CDecalShader();
    virtual ~CDecalShader();
public:
    virtual bool Init();
};

