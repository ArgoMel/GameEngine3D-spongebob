#pragma once
#include "Collider3D.h"

class CColliderOBB3D : public CCollider3D
{
protected:
    friend class CGameObject;
    friend class CScene;
    Vector3 m_BoxHalfSize;
    OBB3DInfo m_Info;
    CColliderOBB3D();
    CColliderOBB3D(const CColliderOBB3D& component);
    virtual ~CColliderOBB3D();
public:
    const OBB3DInfo& GetInfo()  const
    {
        return m_Info;
    }
    void SetBoxHalfSize(const Vector3& size)
    {
        m_BoxHalfSize = size;
    }
    void SetBoxHalfSize(float width, float height, float depth)
    {
        m_BoxHalfSize.x = width;
        m_BoxHalfSize.y = height;
        m_BoxHalfSize.z = depth;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual void RenderDebug();
    virtual CColliderOBB3D* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest);
};

