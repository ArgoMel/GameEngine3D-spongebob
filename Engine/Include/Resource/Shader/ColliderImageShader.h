#pragma once
#include "GraphicShader.h"

class CColliderImageShader : public CGraphicShader
{
protected:
    friend class CShaderManager;
    CColliderImageShader();
    virtual ~CColliderImageShader();
public:
    virtual bool Init();
};

