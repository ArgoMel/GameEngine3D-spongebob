#pragma once
#include "../EngineInfo.h"

class CTransform
{
private:
	friend class CSceneComponent;
	static float	m_MinY;
	static float	m_MaxY;
	class CScene* m_Scene;
	class CGameObject* m_Object;
	class CSceneComponent* m_Owner;
	class CSkeletonSocket* m_Socket;
	class CTransformConstantBuffer* m_CBuffer;
	CTransform* m_Parent;
	Vector3		m_RelativeScale;
	Vector3		m_RelativeRot;
	Vector3		m_RelativePos;
	Vector3		m_RelativeAxis[(int)AXIS::AXIS_MAX];
	Vector3		m_WorldScale;
	Vector3		m_WorldRot;
	Vector3		m_WorldPos;
	Vector3		m_WorldAxis[(int)AXIS::AXIS_MAX];
	Vector3		m_OriginWorldScale;
	Vector3		m_OriginWorldRot;
	Vector3		m_OriginWorldPos;
	Vector3		m_Pivot;
	Vector3		m_MeshSize;
	Vector3		m_Min;
	Vector3		m_Max;
	Vector3		m_CalculateMin;
	Vector3		m_CalculateMax;
	Vector3		m_Center;
	Vector3		m_Offset;
	Matrix		m_matScale;
	Matrix		m_matRot;
	Matrix		m_matPos;
	Matrix		m_matWorld;
	bool		m_Is2D;
	bool	m_InheritScale;
	bool	m_InheritRotX;
	bool	m_InheritRotY;
	bool	m_InheritRotZ;
	bool	m_InheritParentRotationPosX;
	bool	m_InheritParentRotationPosY;
	bool	m_InheritParentRotationPosZ;
	bool	m_UpdateScale;
	bool	m_UpdateRot;
	bool	m_UpdateRotAxis;
	bool	m_UpdatePos;
	float	m_2DZ;
	float		m_Radius;
	std::vector<CTransform*>    m_vecChild;
	CTransform();
	CTransform(const CTransform& transform);
	~CTransform();
	void SetChildRelativeScale(const Vector3& scale);
	void SetChildRelativeRotation(const Vector3& rot);
	void SetChildRelativePosition(const Vector3& pos);

	void AddChildRelativeScale(const Vector3& scale);
	void AddChildRelativeRotation(const Vector3& rot);
	void AddChildRelativePosition(const Vector3& pos);

	void SetChildWorldScale(const Vector3& scale);
	void SetChildWorldRotation(const Vector3& rot);
	void SetChildWorldPosition(const Vector3& pos);

	void AddChildWorldScale(const Vector3& scale);
	void AddChildWorldRotation(const Vector3& rot);
	void AddChildWorldPosition(const Vector3& pos);
public:
	Vector3 GetRelativeScale()	const
	{
		return m_RelativeScale;
	}
	Vector3 GetRelativeRot()	const
	{
		return m_RelativeRot;
	}
	Vector3 GetRelativePos()	const
	{
		return m_RelativePos;
	}
	Vector3 GetRelativeAxis(AXIS axis)	const
	{
		return m_RelativeAxis[(int)axis];
	}
	const Vector3& GetWorldScale()	const
	{
		return m_WorldScale;
	}
	const Vector3& GetWorldRot()	const
	{
		return m_WorldRot;
	}
	const Vector3& GetWorldPos()	const
	{
		return m_WorldPos;
	}
	const Vector3& GetWorldAxis(AXIS axis)	const
	{
		return m_WorldAxis[(int)axis];
	}
	const Vector3& GetPivot()	const
	{
		return m_Pivot;
	}
	const Vector3& GetMeshSize()	const
	{
		return m_MeshSize;
	}
	const Vector3& GetMin()
	{
		m_CalculateMin= m_Min * m_WorldScale + m_WorldPos;

		return m_CalculateMin;
	}
	const Vector3& GetMax()	
	{
		m_CalculateMax= m_Max * m_WorldScale + m_WorldPos;
		return m_CalculateMax;
	}
	const Vector3& GetCenter()	const
	{
		return m_Center;
	}
	const Vector3& GetOffset()	const
	{
		return m_Offset;
	}
	const Matrix& GetWorldMatrix()	const
	{
		return m_matWorld;
	}
	float GetRadius()	const;
	void SetSocket(class CSkeletonSocket* socket)
	{
		m_Socket = socket;
	}
	void SetPivot(const Vector3& pivot)
	{
		m_Pivot = pivot;
	}
	void SetPivot(const Vector2& pivot)
	{
		m_Pivot.x = pivot.x;
		m_Pivot.y = pivot.y;
	}
	void SetPivot(float x, float y, float z)
	{
		m_Pivot = Vector3(x, y, z);
	}
	void SetPivot(float x, float y)
	{
		m_Pivot.x = x;
		m_Pivot.y = y;
	}
	void SetMeshSize(const Vector3& meshSize)
	{
		m_MeshSize = meshSize;
		Vector3	meshSizeScale = m_MeshSize * m_WorldScale;
		m_Radius = meshSizeScale.Length() * 0.5f;
	}
	void SetMeshSize(const Vector2& meshSize)
	{
		m_MeshSize.x = meshSize.x;
		m_MeshSize.y = meshSize.y;
		Vector2	meshSizeScale;
		meshSizeScale.x = meshSize.x * m_WorldScale.x;
		meshSizeScale.y = meshSize.y * m_WorldScale.y;
		m_Radius = meshSizeScale.Length() * 0.5f;
	}
	void SetMeshSize(float x, float y, float z)
	{
		m_MeshSize = Vector3(x, y, z);
		Vector3	meshSizeScale = m_MeshSize * m_WorldScale;
		m_Radius = meshSizeScale.Length() * 0.5f;
	}
	void SetMeshSize(float x, float y)
	{
		m_MeshSize.x = x;
		m_MeshSize.y = y;
		Vector2	meshSizeScale;
		meshSizeScale.x = m_MeshSize.x * m_WorldScale.x;
		meshSizeScale.y = m_MeshSize.y * m_WorldScale.y;
		m_Radius = meshSizeScale.Length() * 0.5f;
	}
	void SetMin(const Vector3& min)
	{
		m_Min = min;
		Vector3	scaleMin = GetMin();
		Vector3	scaleMax = GetMax();
		m_Center = (scaleMin + scaleMax) * 0.5f;
		m_Radius = (scaleMax - scaleMin).Length() * 0.5f;
	}
	void SetMax(const Vector3& max)
	{
		m_Max = max;
		Vector3	scaleMin = GetMin();
		Vector3	scaleMax = GetMax();
		m_Center = (scaleMin + scaleMax) * 0.5f;
		m_Radius = (scaleMax - scaleMin).Length() * 0.5f;
	}
	void SetInheritScale(bool inherit)
	{
		m_InheritScale = inherit;
	}
	void SetInheritRotX(bool inherit)
	{
		m_InheritRotX = inherit;
	}
	void SetInheritRotY(bool inherit)
	{
		m_InheritRotY = inherit;
	}
	void SetInheritRotZ(bool inherit)
	{
		m_InheritRotZ = inherit;
	}
	void SetInheritParentRotationPosX(bool inherit)
	{
		m_InheritParentRotationPosX = inherit;
	}
	void SetInheritParentRotationPosY(bool inherit)
	{
		m_InheritParentRotationPosY = inherit;
	}
	void SetInheritParentRotationPosZ(bool inherit)
	{
		m_InheritParentRotationPosZ = inherit;
	}
	void Set2D(bool is2D)
	{
		m_Is2D = is2D;
	}
	void Clear();
	//부모의 정보를 자식transform에게 적용할때 사용
	void InheritRotation(bool current);
	void InheritParentRotationPos();
	void InheritWorldRotation(bool current);
	void InheritWorldParentRotationPos();

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
	void SetWorldRotationAxis(const Vector3& originDir, const Vector3& view);
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

	void Start();
	bool Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	CTransform* Clone()    const;
	void SetTransform();	// 트랜스폼 정보를 Shader로 보내준다.
	void SetShadowMapTransform();
	void ComputeWorld();	// World정보를 계산한다.
	void Save(FILE* file);
	void Load(FILE* file);
};

