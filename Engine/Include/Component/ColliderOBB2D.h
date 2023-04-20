#pragma once
#include "Collider2D.h"
class CColliderOBB2D : public CCollider2D
{
protected:
    friend class CGameObject;
    friend class CScene;
    Vector2     m_BoxHalfSize;
    OBB2DInfo   m_Info;
    CColliderOBB2D();
    CColliderOBB2D(const CColliderOBB2D& component);
    virtual ~CColliderOBB2D();
public:
    const OBB2DInfo& GetInfo()  const
    {
        return m_Info;
    }
    void SetBoxHalfSize(const Vector2& size)
    {
        m_BoxHalfSize = size;
    }
    void SetBoxHalfSize(float width, float height)
    {
        m_BoxHalfSize.x = width;
        m_BoxHalfSize.y = height;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CColliderOBB2D* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest);
    virtual bool CollisionMouse(const Vector2& mouseWorldPos);
};

