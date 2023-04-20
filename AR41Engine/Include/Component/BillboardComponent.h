#pragma once
#include "PrimitiveComponent.h"

class CBillboardComponent : public CPrimitiveComponent
{
protected:
    friend class CGameObject;
    friend class CScene;
    CBillboardComponent();
    CBillboardComponent(const CBillboardComponent& component);
    virtual ~CBillboardComponent();
public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CBillboardComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

