#pragma once
#include "GraphicShader.h"

class CBillboardShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CBillboardShader();
    virtual ~CBillboardShader();
public:
    virtual bool Init();
};

