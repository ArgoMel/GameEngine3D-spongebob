#pragma once
#include "Collider2D.h"
class CColliderSphere2D : public CCollider2D
{
protected:
    friend class CGameObject;
    friend class CScene;
    Sphere2DInfo    m_Info;
    float           m_Radius;
    CColliderSphere2D();
    CColliderSphere2D(const CColliderSphere2D& component);
    virtual ~CColliderSphere2D();
public:
    const Sphere2DInfo& GetInfo()  const
    {
        return m_Info;
    }
    void SetRadius(float radius)
    {
        m_Radius = radius;
    }
    void AddRadius(float radius)
    {
        m_Radius += radius;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CColliderSphere2D* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest);
    virtual bool CollisionMouse(const Vector2& mouseWorldPos);
};

