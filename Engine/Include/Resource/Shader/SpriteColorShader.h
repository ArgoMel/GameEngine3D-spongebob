#pragma once
#include "GraphicShader.h"

class CSpriteColorShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CSpriteColorShader();
    virtual ~CSpriteColorShader();
public:
    virtual bool Init();
};

