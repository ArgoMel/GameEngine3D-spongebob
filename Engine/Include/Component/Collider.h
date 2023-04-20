#pragma once
#include "SceneComponent.h"

class CCollider : public CSceneComponent
{
protected:
    friend class CGameObject;
    friend class CScene;
    friend class CCollisionManager;
    CSharedPtr<class CMesh> m_Mesh;
    CSharedPtr<class CShader> m_Shader;
    ECollider_Type          m_ColliderType;
    Vector4                 m_Color;
    Vector3                 m_HitPoint;
    CollisionResult         m_Result;
    CollisionResult         m_MouseResult;
    CollisionProfile* m_Profile;
    bool                    m_MouseCollision;
    std::list<CCollider*>   m_PrevCollisionList;    // ���� �����ӿ� �浹�ϰ� �ִ� ��ü��
    std::list<int>          m_CurrentSectionList;   // ���� �浹ü�� ��� �浹������ �����ִ��� �Ǵ��ϱ� ���� ����.
    std::function<void(const CollisionResult&)> m_CollisionCallback[(int)ECollision_Result::Max];
    std::function<void(const CollisionResult&)> m_CollisionMouseCallback[(int)ECollision_Result::Max];
    CCollider();
    CCollider(const CCollider& component);
    virtual ~CCollider();
public:
    ECollider_Type GetColliderType()    const
    {
        return m_ColliderType;
    }
    CollisionProfile* GetCollisionProfile()  const
    {
        return m_Profile;
    }
    void SetCollisionResultHitPoint(const Vector3& hitPoint)
    {
        m_Result.hitPoint = hitPoint;
    }
    void SetCollisionResultDest(CCollider* dest)
    {
        m_Result.dest = dest;
    }
    void AddSectionIndex(int index)
    {
        m_CurrentSectionList.push_back(index);
    }
    void AddPrevCollisionCollider(CCollider* collider)
    {
        m_PrevCollisionList.push_back(collider);
    }
    void DeletePrevCollisionCollider(CCollider* collider);
    bool CheckPrevCollision(CCollider* collider);
    void CheckPrevCollisionColliderSection();   // ���� �����ӿ� �浹�ߴ��͵�� ���� ���� ������ ��ġ���� �Ǵ��Ѵ�.
    void SendPrevCollisionEnd();    // ���� �����ӿ� �浹�Ǵ� ��ü�鿡�� �浹������ �뺸�Ѵ�
    void CallCollisionCallback(ECollision_Result type);
    void CallCollisionMouseCallback(ECollision_Result type);
    void ClearFrame();
    int CheckOverlapSection(CCollider* dest);
    void SetCollisionProfile(const std::string& name);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CCollider* Clone()    const = 0;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool Collision(CCollider* dest) = 0;
    template <typename T>
    void SetCollisionCallback(ECollision_Result type, T* obj, void(T::* func)(const CollisionResult&))
    {
        m_CollisionCallback[(int)type] = std::bind(func, obj, std::placeholders::_1);
    }
    template <typename T>
    void SetCollisionMouseCallback(ECollision_Result type, T* obj, void(T::* func)(const CollisionResult&))
    {
        m_CollisionMouseCallback[(int)type] = std::bind(func, obj, std::placeholders::_1);
    }
};

