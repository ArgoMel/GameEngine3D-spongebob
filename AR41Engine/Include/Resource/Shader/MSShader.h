#pragma once
#include "GraphicShader.h"

class CMSShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CMSShader();
    virtual ~CMSShader();
public:
    virtual bool Init();
};

