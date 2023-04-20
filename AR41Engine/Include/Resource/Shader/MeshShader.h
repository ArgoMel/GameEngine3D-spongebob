#pragma once
#include "GraphicShader.h"

class CMeshShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CMeshShader();
    virtual ~CMeshShader();
public:
    virtual bool Init();
};

