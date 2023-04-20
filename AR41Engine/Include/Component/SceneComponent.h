#pragma once
#include "Component.h"
#include "Transform.h"

class CSceneComponent :public CComponent
{
protected:
    friend class CGameObject;
	friend class CScene;
	class CSkeletonSocket* m_Socket;
    CTransform* m_Transform;
    CSceneComponent* m_Parent;
	CSharedPtr<class CGraphicShader>	m_ShadowMapShader;
	SceneComponentType	m_SceneComponentType;
	std::string		m_SceneName;
	std::string     m_LayerName;
	std::string		m_ParentName;
	std::vector<CSharedPtr<CSceneComponent>>    m_vecChild;
	std::vector<std::string>	m_vecChildName;
	bool			m_FrustumCull;
    CSceneComponent();
    CSceneComponent(const CSceneComponent& component);
    virtual ~CSceneComponent();
public:
    CTransform* GetTransform()    const
    {
        return m_Transform;
    }
	CSceneComponent* GetParent()	const
	{
		return m_Parent;
	}
	SceneComponentType GetSceneComponentType()	const
	{
		return m_SceneComponentType;
	}
	const std::string& GetSceneName()   const
	{
		return m_SceneName;
	}
	const std::string& GetRenderLayerName() const
	{
		return m_LayerName;
	}
	void SetRenderLayerName(const std::string& name)
	{
		m_LayerName = name;
	}
	bool GetFrustumCull()	const
	{
		return m_FrustumCull;
	}
	void SetFrustumCull(bool cull)
	{
		m_FrustumCull = cull;
	}
	void AddOwner();
	virtual void SetSocket(class CSkeletonSocket* socket);
    virtual void SetScene(class CScene* scene);
    virtual void SetOwner(class CGameObject* owner);
    // SocketName은 나중에 3D에서 사용 예정.
    virtual void AddChild(CSceneComponent* child, const std::string& socketName = "");
    virtual void AddChild(class CGameObject* child, const std::string& socketName = "");
    bool DeleteChild(CSceneComponent* child);
    bool DeleteChild(const std::string& name);
    CSceneComponent* FindComponent(const std::string& name);
	void GetAllComponentHierarchyName(std::vector<HierarchyName>& vecName);
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
	virtual void RenderShadowMap();
    virtual CSceneComponent* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);

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

	const Vector3& GetCenter()	const;
	const Vector3& GetMin()	const;
	const Vector3& GetMax()	const;
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
	void SetMin(const Vector3& min);
	void SetMax(const Vector3& max);
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