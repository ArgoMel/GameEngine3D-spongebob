#pragma once
#include "Collider2D.h"
class CColliderBox2D : public CCollider2D
{
protected:
    friend class CGameObject;
    friend class CScene;
    Vector2     m_BoxSize;
    Box2DInfo   m_Info;
    CColliderBox2D();
    CColliderBox2D(const CColliderBox2D& component);
    virtual ~CColliderBox2D();
public:
    Vector2 GetBoxSize()    const
    {
        return m_BoxSize;
    }
    const Box2DInfo& GetInfo()  const
    {
        return m_Info;
    }
    void SetBoxSize(const Vector2& Size)
    {
        m_BoxSize = Size;
    }
    void SetBoxSize(float Width, float Height)
    {
        m_BoxSize.x = Width;
        m_BoxSize.y = Height;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CColliderBox2D* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest);
    virtual bool CollisionMouse(const Vector2& mouseWorldPos);
};

