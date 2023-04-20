#pragma once
#include "Mesh.h"

class CSpriteMesh :public CMesh
{
protected:
    friend class CMeshManager;
    CSpriteMesh();
    virtual ~CSpriteMesh();
};

