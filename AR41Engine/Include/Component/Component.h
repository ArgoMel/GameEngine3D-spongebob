#pragma once
#include "../Ref.h"
class CComponent :public CRef
{
private:
    friend class CGameObject;
    friend class CSceneManager;
    friend class CScene;
    static std::unordered_map<std::string, CComponent*> m_mapComponent;
protected:
    class CScene* m_Scene;
    class CGameObject* m_Owner;
    ComponentType   m_ComponentType;
    std::string     m_ComponentTypeName;
    int             m_SerialNumber; // 게임오브젝트에 포함될때 사용할 식별번호.
    CComponent();
    CComponent(const CComponent& obj);
    virtual ~CComponent() = 0;
public:
    class CScene* GetScene()    const
    {
        return m_Scene;
    }
    class CGameObject* GetOwner()   const
    {
        return m_Owner;
    }
    ComponentType GetComponentType()    const
    {
        return m_ComponentType;
    }
    const std::string& GetComponentTypeName()   const
    {
        return m_ComponentTypeName;
    }
    int GetSerialNumber()   const
    {
        return m_SerialNumber;
    }
    virtual void SetScene(class CScene* scene)
    {
        m_Scene = scene;
    }
    virtual void SetOwner(class CGameObject* owner)
    {
        m_Owner = owner;
    }
    void SetSerialNumber(int number)
    {
        m_SerialNumber = number;
    }
    static void AddComponentCDO(const std::string& name, CComponent* CDO)
    {
        m_mapComponent.insert(std::make_pair(name, CDO));
    }
    static CComponent* FindCDO(const std::string& name)
    {
        auto	iter = m_mapComponent.find(name);
        if (iter == m_mapComponent.end())
        {
            return nullptr;
        }
        return iter->second;
    }
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

