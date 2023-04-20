#pragma once
#include "Collider3D.h"

class CColliderCube : public CCollider3D
{
protected:
    friend class CGameObject;
    friend class CScene;
    Vector3 m_CubeSize;
    CubeInfo m_Info;
    CColliderCube();
    CColliderCube(const CColliderCube& component);
    virtual ~CColliderCube();
public:
    Vector3 GetCubeSize() const
    {
        return m_CubeSize;
    }
    CubeInfo GetCubeInfo() const
    {
        return m_Info;
    }
    void SetCubeSize(const Vector3& size)
    {
        m_CubeSize = size;
    }
    void SetCubeSize(float width, float height, float depth)
    {
        m_CubeSize.x = width;
        m_CubeSize.y = height;
        m_CubeSize.z = depth;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual void RenderDebug();
    virtual CColliderCube* Clone() const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest);
};

