#pragma once
#include "../../EngineInfo.h"

class CSkeletonSocket
{
private:
	friend class CSkeleton;
	friend class CAnimation;
	std::string		m_Name;
	std::string		m_BoneName;
	Vector3			m_Offset;
	Vector3			m_OffsetRotation;
	Matrix			m_matSocket;
	Vector3			m_BoneScale;
	Vector3			m_BonePos;
	Vector4			m_BoneRot;
	Vector3			m_WorldScale;
	Vector3			m_WorldPos;
	Vector3			m_WorldRot;
	Vector3			m_PrevWorldScale;
	Vector3			m_PrevWorldPos;
	Vector3			m_PrevWorldRot;
	Vector3			m_MoveScale;
	Vector3			m_MovePos;
	Vector3			m_MoveRot;
	int				m_BoneIndex;
	CSkeletonSocket();
	CSkeletonSocket(const CSkeletonSocket& socket);
	~CSkeletonSocket();
	void ComputeBone();
	void ComputeWorld(class CAnimationMeshComponent* owner);
public:
	const std::string& GetName()	const
	{
		return m_Name;
	}
	const std::string& GetBoneName()	const
	{
		return m_BoneName;
	}
	const Vector3& GetOffset()	const
	{
		return m_Offset;
	}
	const Vector3& GetOffsetRotation()	const
	{
		return m_OffsetRotation;
	}
	const Matrix& GetSocketMatrix()	const
	{
		return m_matSocket;
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
	const Vector3& GetMoveScale()	const
	{
		return m_MoveScale;
	}
	const Vector3& GetMovePos()	const
	{
		return m_MovePos;
	}
	const Vector3& GetMoveRot()	const
	{
		return m_MoveRot;
	}
	int GetBoneIndex()	const
	{
		return m_BoneIndex;
	}
	void SetName(const std::string& name)
	{
		m_Name = name;
	}
	void SetBone(const std::string& boneName)
	{
		m_BoneName = boneName;
	}
	void SetOffset(const Vector3& offset)
	{
		m_Offset = offset;
	}
	void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}
	void SetOffsetRotation(const Vector3& offsetRotation)
	{
		m_OffsetRotation = offsetRotation;
	}
	void SetOffsetRotation(float x, float y, float z)
	{
		m_OffsetRotation = Vector3(x, y, z);
	}
	void SetBoneIndex(int index)
	{
		m_BoneIndex = index;
	}
	void Update(const OutputBoneInfo& info, class CAnimationMeshComponent* owner);
	CSkeletonSocket* Clone();
};

