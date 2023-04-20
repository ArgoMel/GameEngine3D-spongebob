#pragma once
#include "GraphicShader.h"

class CLightAccShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CLightAccShader();
    virtual ~CLightAccShader();
public:
    virtual bool Init();
};

