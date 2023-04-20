#pragma once
#include "GraphicShader.h"

class CShadowMapStaticInstancingShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CShadowMapStaticInstancingShader();
    virtual ~CShadowMapStaticInstancingShader();
public:
    virtual bool Init();
};

