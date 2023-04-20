#pragma once
#include "PrimitiveComponent.h"

class CStaticMeshComponent : public CPrimitiveComponent
{
protected:
    friend class CGameObject;
    friend class CScene;
    CStaticMeshComponent();
    CStaticMeshComponent(const CStaticMeshComponent& component);
    virtual ~CStaticMeshComponent();
public:
    virtual bool SetMesh(const std::string& name);
    virtual bool SetMesh(class CMesh* mesh);
    virtual bool SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& name, const TCHAR* fullPath);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual void RenderShadowMap();
    virtual CStaticMeshComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

