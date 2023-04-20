#pragma once
#include "GraphicShader.h"
class CColliderShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CColliderShader();
    virtual ~CColliderShader();
public:
    virtual bool Init();
};

