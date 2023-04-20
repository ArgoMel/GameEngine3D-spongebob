#pragma once
#include "GraphicShader.h"

class CTileMapShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CTileMapShader();
    virtual ~CTileMapShader();
public:
    virtual bool Init();
};

