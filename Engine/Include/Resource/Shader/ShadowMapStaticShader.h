#pragma once
#include "GraphicShader.h"

class CShadowMapStaticShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CShadowMapStaticShader();
    virtual ~CShadowMapStaticShader();
public:
    virtual bool Init();
};

