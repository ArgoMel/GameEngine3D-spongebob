#pragma once
#include "ObjectComponent.h"
#include "../Thread/ThreadQueue.h"

class CNavigationAgent : public CObjectComponent
{
private:
    friend class CGameObject;
    friend class CScene;
    CSharedPtr<class CSceneComponent> m_UpdateComponent;
    CThreadQueue	m_FindQueue;
    float           m_MoveSpeed;
    std::list<Vector2>  m_PathList;
protected:
    CNavigationAgent();
    CNavigationAgent(const CNavigationAgent& obj);
    virtual ~CNavigationAgent();
public:
    class CSceneComponent* GetUpdateComponent() const
    {
        return m_UpdateComponent;
    }
    void SetUpdateComponent(class CSceneComponent* component);
    void SetMoveSpeed(float speed);
    void AddPathList(const std::list<Vector2>& pathList);
    bool Move(const Vector2& targetPos);
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CNavigationAgent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

