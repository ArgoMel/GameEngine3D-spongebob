#pragma once
#include "GraphicShader.h"

class CParticleRenderShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CParticleRenderShader();
    virtual ~CParticleRenderShader();
public:
    virtual bool Init();
};

