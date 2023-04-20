#pragma once
#include "GraphicShader.h"

class CDeferredRenderShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CDeferredRenderShader();
    virtual ~CDeferredRenderShader();
public:
    virtual bool Init();
};

