#pragma once
#include "ObjectComponent.h"
#include "../Thread/ThreadQueue.h"

class CNavigationAgent3D : public CObjectComponent
{
private:
    friend class CGameObject;
    friend class CScene;
    CSharedPtr<class CSceneComponent> m_UpdateComponent;
    CThreadQueue	m_FindQueue;
    float           m_MoveSpeed;
    std::list<Vector3>  m_PathList;
protected:
    CNavigationAgent3D();
    CNavigationAgent3D(const CNavigationAgent3D& obj);
    virtual ~CNavigationAgent3D();
public:
    class CSceneComponent* GetUpdateComponent() const
    {
        return m_UpdateComponent;
    }
    void SetUpdateComponent(class CSceneComponent* component);
    void SetMoveSpeed(float speed);
    void AddPathList(const std::list<Vector3>& pathList);
    bool Move(const Vector3& targetPos);
    bool MovePicking();
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CNavigationAgent3D* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

