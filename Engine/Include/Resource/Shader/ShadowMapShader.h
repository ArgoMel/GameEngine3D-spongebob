#pragma once
#include "GraphicShader.h"

class CShadowMapShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CShadowMapShader();
    virtual ~CShadowMapShader();
public:
    virtual bool Init();
};

