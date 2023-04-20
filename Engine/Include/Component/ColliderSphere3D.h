#pragma once
#include "Collider3D.h"

class CColliderSphere3D : public CCollider3D
{
protected:
    friend class CGameObject;
    friend class CScene;
    float           m_Radius;
    Sphere3DInfo    m_Info;
    CColliderSphere3D();
    CColliderSphere3D(const CColliderSphere3D& component);
    virtual ~CColliderSphere3D();
public:
    const Sphere3DInfo& GetInfo()  const
    {
        return m_Info;
    }
    void SetRadius(float radius)
    {
        m_Radius = radius;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual void RenderDebug();
    virtual CColliderSphere3D* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest);
};

