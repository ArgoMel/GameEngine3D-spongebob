#pragma once
#include "PrimitiveComponent.h"

class CFogComponent : public CPrimitiveComponent
{
private:
    friend class CGameObject;
    friend class CScene;
    class CTerrainConstantBuffer* m_CBuffer;
    float m_FogStart;
    float m_FogEnd;
protected:
    CFogComponent();
    CFogComponent(const CFogComponent& component);
    virtual ~CFogComponent();
public:
    void SetFogData(const Vector4& color,float start,float end);
    virtual bool SetMesh(const std::string& name);
    virtual bool SetMesh(class CMesh* mesh);
    virtual bool SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& name, const TCHAR* fullPath);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void RenderShadowMap();
    virtual void Render();
    virtual CFogComponent* Clone() const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

