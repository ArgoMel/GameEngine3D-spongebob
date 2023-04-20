#pragma once
#include "GraphicShader.h"

class CSpriteShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CSpriteShader();
    virtual ~CSpriteShader();
public:
    virtual bool Init();
};

