#pragma once
#include "Component.h"

class CObjectComponent :public CComponent
{
protected:
    friend class CGameObject;
    friend class CScene;
    CObjectComponent();
    CObjectComponent(const CObjectComponent& obj);
    virtual ~CObjectComponent() = 0;
public:
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CObjectComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

