#pragma once
#include "SceneComponent.h"

class CPrimitiveComponent :public CSceneComponent
{
protected:
    friend class CGameObject;
    friend class CScene;
    CSharedPtr<class CMesh> m_Mesh;
    int     m_InstanceID;
    bool    m_Render;
    bool	m_ReceiveDecal;
    std::vector<CSharedPtr<class CMaterial>>    m_vecMaterial;
    CPrimitiveComponent();
    CPrimitiveComponent(const CPrimitiveComponent& component);
    virtual ~CPrimitiveComponent();
public:
    class CMesh* GetMesh()  const
    {
        return m_Mesh;
    }
    int GetInstanceID() const
    {
        return m_InstanceID;
    }
    bool IsRender() const
    {
        return m_Render;
    }
    bool GetReceiveDecal()	const
    {
        return m_ReceiveDecal;
    }
    class CMaterial* GetMaterial(int slot)
    {
        return m_vecMaterial[slot];
    }
    const std::vector<CSharedPtr<class CMaterial>>* GetMaterials()  const
    {
        return &m_vecMaterial;
    }
    int GetSlotCount()  const
    {
        return (int)m_vecMaterial.size();
    }
    void SetInstanceID(int ID)
    {
        m_InstanceID = ID;
    }
    void SetReceiveDecal(bool decal);
    virtual bool SetMesh(const std::string& name);
    virtual bool SetMesh(class CMesh* mesh);
    virtual bool SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& name, const TCHAR* fullPath);
    virtual void SetMaterial(int slot, const std::string& name);
    virtual void SetMaterial(int slot, class CMaterial* material);
    virtual void AddMaterial(const std::string& name);
    virtual void AddMaterial(class CMaterial* material);
    void ClearMaterial();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual void RenderShadowMap();
    virtual CPrimitiveComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

