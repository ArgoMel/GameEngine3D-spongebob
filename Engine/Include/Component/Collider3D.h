#pragma once
#include "Collider.h"

class CCollider3D : public CCollider
{
protected:
    friend class CGameObject;
    friend class CScene;
    class CRenderState* m_WireFrame;
    ECollider3D_Type          m_Collider3DType;
    CCollider3D();
    CCollider3D(const CCollider3D& component);
    virtual ~CCollider3D();
public:
    ECollider3D_Type GetCollider3DType()    const
    {
        return m_Collider3DType;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual void RenderDebug();
    virtual CCollider3D* Clone()    const = 0;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest) = 0;
};

