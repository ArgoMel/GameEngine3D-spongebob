#pragma once
#include "GraphicShader.h"

class CShadowMapInstancingShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CShadowMapInstancingShader();
    virtual ~CShadowMapInstancingShader();
public:
    virtual bool Init();
};

