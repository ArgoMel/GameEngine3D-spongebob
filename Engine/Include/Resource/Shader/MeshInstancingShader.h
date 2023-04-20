#pragma once
#include "GraphicShader.h"

class CMeshInstancingShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CMeshInstancingShader();
    virtual ~CMeshInstancingShader();
public:
    virtual bool Init();
};

