#pragma once
#include "GraphicShader.h"

class CTileMapBackShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CTileMapBackShader();
    virtual ~CTileMapBackShader();
public:
    virtual bool Init();
};

