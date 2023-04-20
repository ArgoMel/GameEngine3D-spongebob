#pragma once
#include "GraphicShader.h"

class CTerrainShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CTerrainShader();
    virtual ~CTerrainShader();
public:
    virtual bool Init();
};

