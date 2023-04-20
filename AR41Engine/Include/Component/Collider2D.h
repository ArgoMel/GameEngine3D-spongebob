#pragma once
#include "Collider.h"
class CCollider2D : public CCollider
{
protected:
    friend class CGameObject;
    friend class CScene;
    ECollider2D_Type          m_Collider2DType;
    CCollider2D();
    CCollider2D(const CCollider2D& component);
    virtual ~CCollider2D();
public:
    ECollider2D_Type GetCollider2DType()    const
    {
        return m_Collider2DType;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CCollider2D* Clone()     const = 0;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest) = 0;
    virtual bool CollisionMouse(const Vector2& mouseWorldPos) = 0;
};

