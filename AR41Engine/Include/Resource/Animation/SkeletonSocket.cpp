#include "SkeletonSocket.h"
#include "../../Component/AnimationMeshComponent.h"

CSkeletonSocket::CSkeletonSocket()
	: m_BoneIndex(0)
{
}

CSkeletonSocket::CSkeletonSocket(const CSkeletonSocket& socket)
{
	*this = socket;	//�����Ҵ��ϴ°� ������ ���� �����ص� �ȴ�	
}

CSkeletonSocket::~CSkeletonSocket()
{
}

void CSkeletonSocket::ComputeBone()
{	
	// ���� ȸ�������� �̿��ؼ� Offset�� ��ġ�� �����ش�.
	// ���� Offset�� 0, 0, 0 �̶�� ������ �ʿ����.
	if (m_Offset.x != 0.f || m_Offset.y != 0.f || m_Offset.z != 0.f)
	{
		Matrix	matRot;	// ����� ���Ѵ�.
		Vector4	boneRot = m_BoneRot;
		boneRot.x = DegreeToRadian(boneRot.x);
		boneRot.y = DegreeToRadian(boneRot.y);
		boneRot.z = DegreeToRadian(boneRot.z);
		matRot.RotationQuaternion(boneRot);
		memcpy(&matRot._41, &m_BonePos, sizeof(Vector3));
		m_WorldPos = m_Offset.TransformCoord(matRot);
	}
	else
	{
		m_WorldPos = m_BonePos;
	}
	m_WorldRot.x = m_OffsetRotation.x + m_BoneRot.x;
	m_WorldRot.y = m_OffsetRotation.y + m_BoneRot.y;
	m_WorldRot.z = m_OffsetRotation.z + m_BoneRot.z;
	m_WorldScale = m_BoneScale;
}

void CSkeletonSocket::ComputeWorld(CAnimationMeshComponent* owner)
{
	Vector3	parentRot = owner->GetWorldRot();
	parentRot=parentRot.ConvertAngle();
	DirectX::XMVECTOR	qut = DirectX::XMQuaternionRotationRollPitchYaw(parentRot.x,parentRot.y, parentRot.z);
	Matrix	matRot;
	matRot.RotationQuaternion(qut);
	Vector3	parentPos = owner->GetWorldPos();
	memcpy(&matRot._41, &parentPos, sizeof(Vector3));
	m_WorldPos = m_WorldPos.TransformCoord(matRot);
	m_WorldRot += owner->GetWorldRot();
	m_WorldScale *= owner->GetWorldScale();
}

void CSkeletonSocket::Update(const OutputBoneInfo& info, CAnimationMeshComponent* owner)
{
	m_PrevWorldPos = m_WorldPos;
	m_PrevWorldScale = m_WorldScale;
	m_PrevWorldRot = m_WorldRot;
	// ������ �θ�� ���̴�.
	// ���� �θ�� ������ �����̴�.
	m_BoneScale = info.scale;
	m_BonePos = info.pos;
	m_BoneRot = info.rot;
	m_BoneRot.x = RadianToDegree(m_BoneRot.x);
	m_BoneRot.y = RadianToDegree(m_BoneRot.y);
	m_BoneRot.z = RadianToDegree(m_BoneRot.z);
	ComputeBone();
	ComputeWorld(owner);
	Matrix	matScale, matRot, matPos;
	matScale.Scaling(m_WorldScale);
	matRot.Rotation(m_WorldRot);
	matPos.Translation(m_WorldPos);
	m_matSocket = matScale * matRot * matPos;
	m_MoveScale = m_WorldScale - m_PrevWorldScale;
	m_MovePos = m_WorldPos - m_PrevWorldPos;
	m_MoveRot = m_WorldRot - m_PrevWorldRot;
	if (m_MoveScale.x <= 0.00001f)
	{
		m_MoveScale.x = 1.f;
	}
	if (m_MoveScale.y <= 0.00001f)
	{
		m_MoveScale.y = 1.f;
	}
	if (m_MoveScale.z <= 0.00001f)
	{
		m_MoveScale.z = 1.f;
	}
}

CSkeletonSocket* CSkeletonSocket::Clone()
{
	return new CSkeletonSocket(*this);
}
