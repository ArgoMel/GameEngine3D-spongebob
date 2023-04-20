#pragma once
#include "GraphicShader.h"

class COceanShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    COceanShader();
    virtual ~COceanShader();
public:
    virtual bool Init();
};

