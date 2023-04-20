#pragma once
#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"

class CGameObject :public CRef
{
private:
    friend class CScene;
    friend class CSceneManager;
    static std::unordered_map<std::string, CGameObject*>	m_mapObjectCDO;
protected:
    class CScene* m_Scene;
    CGameObject* m_Parent;
    CSharedPtr<CSceneComponent> m_RootComponent;
    std::string		m_ObjectTypeName;
    Vector3			m_PrevPos;
    Vector3			m_Move;
    Vector3		m_Min;
    Vector3		m_Max;
    Vector3		m_Center;
    float		m_TimeScale;
    float       m_LifeTime;
    float		m_Radius;
    bool		m_FrustumCull;
    bool		m_Start;
    int		m_ComponentSerialNumber;
    std::list<CSceneComponent*> m_SceneComponentList;
    std::vector<CSharedPtr<CObjectComponent>>   m_vecObjectComponent;
    CGameObject();
    CGameObject(const CGameObject& obj);
    virtual ~CGameObject();
public:
    static bool SortComponent(class CSceneComponent* src, class CSceneComponent* dest);
    static void AddObjectCDO(const std::string& name, CGameObject* CDO)
    {
        m_mapObjectCDO.insert(std::make_pair(name, CDO));
    }
    static CGameObject* FindCDO(const std::string& name)
    {
        auto	iter = m_mapObjectCDO.find(name);
        if (iter == m_mapObjectCDO.end())
        {
            return nullptr;
        }
        return iter->second;
    }
    class CScene* GetScene()    const
    {
        return m_Scene;
    }
    CSceneComponent* GetRootComponent() const
    {
        return m_RootComponent;
    }
    const std::list<CSceneComponent*>& GetSceneComponents() const
    {
        return m_SceneComponentList;
    }
    const std::string& GetObjectTypeName()	const
    {
        return m_ObjectTypeName;
    }
    const Vector3& GetPrevPos()	const
    {
        return m_PrevPos;
    }
    const Vector3& GetMove()	const
    {
        return m_Move;
    }
    const Vector3& GetCenter()	const
    {
        return m_Center;
    }
    float GetObjectRadius()	const
    {
        return m_Radius;
    }
    bool GetFrustumCull()	const
    {
        return m_FrustumCull;
    }
    void SetScene(class CScene* scene);
    void SetRootComponent(CSceneComponent* component)
    {
        m_RootComponent = component;
    }
    float GetTimeScale()	const
    {
        return m_TimeScale;
    }
    void SetTimeScale(float scale)
    {
        m_TimeScale = scale;
    }
    void SetLifeTime(float lifeTime)
    {
        m_LifeTime = lifeTime;
    }
    void AddSceneComponent(CSceneComponent* component)
    {
        m_SceneComponentList.push_back(component);
    }
    void DeleteSceneComponent(CSceneComponent* component)
    {
        auto	iter = m_SceneComponentList.begin();
        auto	iterEnd = m_SceneComponentList.end();
        for (; iter != iterEnd; ++iter)
        {
            if (*iter == component)
            {
                m_SceneComponentList.erase(iter);
                return;
            }
        }
    }
    void AddChildToSocket(const std::string& socketName, CGameObject* obj);
    virtual void Destroy(); //삭제시 하위 컴포넌트도 전부 삭제하는 함수
    void GetAllComponentHierarchyName(std::vector<HierarchyName>& vecName);
    CComponent* FindComponent(const std::string& name);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual CGameObject* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual int InflictDamage(float damage=1.0f);
    virtual void Reset();
    void FrustumCull(class CCameraComponent* camera);
    bool Picking(PickingResult& result);
    template <typename T>
    T* FindComponentFromType()  const
    {
        auto    iter = m_SceneComponentList.begin();
        auto    iterEnd = m_SceneComponentList.end();
        for (; iter != iterEnd; ++iter)
        {
            if ((*iter)->CheckTypeID<T>())
            {
                return (T*)*iter;
            }
        }
        auto    iter1 = m_vecObjectComponent.begin();
        auto    iter1End = m_vecObjectComponent.end();
        for (; iter1 != iter1End; ++iter1)
        {
            if ((*iter1)->CheckTypeID<T>())
            {
                return (T*)(*iter1).Get();
            }
        }
        return nullptr;
    }
    template <typename T>
    T* CreateComponent(const std::string& name)
    {
        T* component = new T;
        component->SetName(name);
        component->SetScene(m_Scene);
        component->SetOwner(this);
        if (!component->Init())
        {
            SAFE_RELEASE(component);
            return nullptr;
        }
        if (component->GetComponentType() == ComponentType::Object)
        {
            m_vecObjectComponent.push_back((CObjectComponent*)component);
        }
        else
        {
            if (!m_RootComponent)
            {
                m_RootComponent = (CSceneComponent*)component;
                //m_SceneComponentList.push_back(m_RootComponent);
                m_SceneComponentList.emplace_back(m_RootComponent);
            }
            else
            {
                m_SceneComponentList.push_back((CSceneComponent*)component);
            }
        }
        component->SetSerialNumber(m_ComponentSerialNumber);
        ++m_ComponentSerialNumber;
        if (m_Start)
        {
            component->Start();
        }
        return component;
    }

    void SetInheritScale(bool inherit);
    void SetInheritRotX(bool inherit);
    void SetInheritRotY(bool inherit);
    void SetInheritRotZ(bool inherit);
    void SetInheritParentRotationPosX(bool inherit);
    void SetInheritParentRotationPosY(bool inherit);
    void SetInheritParentRotationPosZ(bool inherit);
    void InheritRotation(bool current);
    void InheritParentRotationPos();
    void InheritWorldRotation(bool current);
    void InheritWorldParentRotationPos();

    Vector3 GetRelativeScale()	const;
    Vector3 GetRelativeRot()	const;
    Vector3 GetRelativePos()	const;
    Vector3 GetRelativeAxis(AXIS axis)	const;
    void SetRelativeScale(const Vector3& scale);
    void SetRelativeScale(const Vector2& scale);
    void SetRelativeScale(float x, float y);
    void SetRelativeScale(float x, float y, float z);
    void SetRelativeScaleX(float x);
    void SetRelativeScaleY(float y);
    void SetRelativeScaleZ(float z);
    void SetRelativeRotation(const Vector3& rot);
    void SetRelativeRotation(const Vector2& rot);
    void SetRelativeRotation(float x, float y);
    void SetRelativeRotation(float x, float y, float z);
    void SetRelativeRotationX(float x);
    void SetRelativeRotationY(float y);
    void SetRelativeRotationZ(float z);
    void SetRelativePosition(const Vector3& pos);
    void SetRelativePosition(const Vector2& pos);
    void SetRelativePosition(float x, float y);
    void SetRelativePosition(float x, float y, float z);
    void SetRelativePositionX(float x);
    void SetRelativePositionY(float y);
    void SetRelativePositionZ(float z);

    void AddRelativeScale(const Vector3& scale);
    void AddRelativeScale(const Vector2& scale);
    void AddRelativeScale(float x, float y);
    void AddRelativeScale(float x, float y, float z);
    void AddRelativeScaleX(float x);
    void AddRelativeScaleY(float y);
    void AddRelativeScaleZ(float z);
    void AddRelativeRotation(const Vector3& rot);
    void AddRelativeRotation(const Vector2& rot);
    void AddRelativeRotation(float x, float y);
    void AddRelativeRotation(float x, float y, float z);
    void AddRelativeRotationX(float x);
    void AddRelativeRotationY(float y);
    void AddRelativeRotationZ(float z);
    void AddRelativePosition(const Vector3& pos);
    void AddRelativePosition(const Vector2& pos);
    void AddRelativePosition(float x, float y);
    void AddRelativePosition(float x, float y, float z);
    void AddRelativePositionX(float x);
    void AddRelativePositionY(float y);
    void AddRelativePositionZ(float z);

    float GetRadius()	const;
    const Vector3& GetWorldScale()	const;
    const Vector3& GetWorldRot()	const;
    const Vector3& GetWorldPos()	const;
    const Vector3& GetWorldAxis(AXIS axis)	const;
    const Vector3& GetPivot()	const;
    const Vector3& GetMeshSize()	const;
    const Vector3& GetOffset()	const;
    const Matrix& GetWorldMatrix()	const;
    void SetPivot(const Vector3& pivot);
    void SetPivot(const Vector2& pivot);
    void SetPivot(float x, float y, float z);
    void SetPivot(float x, float y);
    void SetMeshSize(const Vector3& meshSize);
    void SetMeshSize(const Vector2& meshSize);
    void SetMeshSize(float x, float y, float z);
    void SetMeshSize(float x, float y);

    void SetWorldScale(const Vector3& scale);
    void SetWorldScale(const Vector2& scale);
    void SetWorldScale(float x, float y);
    void SetWorldScale(float x, float y, float z);
    void SetWorldScaleX(float x);
    void SetWorldScaleY(float y);
    void SetWorldScaleZ(float z);
    void SetWorldRotation(const Vector3& rot);
    void SetWorldRotation(const Vector2& rot);
    void SetWorldRotation(float x, float y);
    void SetWorldRotation(float x, float y, float z);
    void SetWorldRotationX(float x);
    void SetWorldRotationY(float y);
    void SetWorldRotationZ(float z);
    void SetWorldPosition(const Vector3& pos);
    void SetWorldPosition(const Vector2& pos);
    void SetWorldPosition(float x, float y);
    void SetWorldPosition(float x, float y, float z);
    void SetWorldPositionX(float x);
    void SetWorldPositionY(float y);
    void SetWorldPositionZ(float z);
    void SetOffset(const Vector3& offset);
    void SetOffset(const Vector2& offset);
    void SetOffsetX(float x);
    void SetOffsetY(float y);
    void SetOffsetZ(float z);

    void AddWorldScale(const Vector3& scale);
    void AddWorldScale(const Vector2& scale);
    void AddWorldScale(float x, float y);
    void AddWorldScale(float x, float y, float z);
    void AddWorldScaleX(float x);
    void AddWorldScaleY(float y);
    void AddWorldScaleZ(float z);
    void AddWorldRotation(const Vector3& rot);
    void AddWorldRotation(const Vector2& rot);
    void AddWorldRotation(float x, float y);
    void AddWorldRotation(float x, float y, float z);
    void AddWorldRotationX(float x);
    void AddWorldRotationY(float y);
    void AddWorldRotationZ(float z);
    void AddWorldPosition(const Vector3& pos);
    void AddWorldPosition(const Vector2& pos);
    void AddWorldPosition(float x, float y);
    void AddWorldPosition(float x, float y, float z);
    void AddWorldPositionX(float x);
    void AddWorldPositionY(float y);
    void AddWorldPositionZ(float z);
    void AddOffset(const Vector3& offset);
    void AddOffset(const Vector2& offset);
    void AddOffsetX(float x);
    void AddOffsetY(float y);
    void AddOffsetZ(float z);
};

