#include "Transform.h"
#include "CameraComponent.h"
#include "../Engine.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Resource/Shader/TransformConstantBuffer.h"
#include "../Resource/Animation/SkeletonSocket.h"

float CTransform::m_MinY = FLT_MAX;
float CTransform::m_MaxY = -FLT_MAX;

CTransform::CTransform()
	: m_Is2D(false)
	, m_InheritScale(false)
	, m_InheritRotX(false)
	, m_InheritRotY(false)
	, m_InheritRotZ(false)
	, m_InheritParentRotationPosX(true)
	, m_InheritParentRotationPosY(true)
	, m_InheritParentRotationPosZ(true)
	, m_UpdateScale(true)
	, m_UpdateRot(true)
	, m_UpdateRotAxis(false)
	, m_UpdatePos(true)
	, m_RelativeScale(1.f, 1.f, 1.f)
	, m_WorldScale(1.f, 1.f, 1.f)
	, m_Parent(nullptr)
	, m_Scene(nullptr)
	, m_Object(nullptr)
	, m_Owner(nullptr)
	, m_Socket(nullptr)
	, m_MeshSize(1.f, 1.f, 1.f)
	, m_2DZ(0.f)
	, m_Radius(0.f)
{
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::axis[i];
		m_WorldAxis[i] = Vector3::axis[i];
	}
	m_CBuffer = new CTransformConstantBuffer;
	m_CBuffer->Init();
}

CTransform::CTransform(const CTransform& transform)
{
	*this = transform;
	m_CBuffer = transform.m_CBuffer->Clone();
	m_Socket = nullptr;
}

CTransform::~CTransform()
{
	SAFE_DELETE(m_CBuffer);
}

void CTransform::Clear()
{
	if (m_Is2D)
	{
		m_MinY = FLT_MAX;
		m_MaxY = -FLT_MAX;
	}
}

void CTransform::InheritRotation(bool current)
{
	// �θ� ���� ��� �θ��� ȸ���� ������ �޴´�.
	// ��, x, y, z �� ȸ���� ��� ���� ������ ������ ������ ���� �� �ְ� �صξ����Ƿ� �� �ึ�� ����Ѵ�.
	if (m_Parent)
	{
		// x, y, z �� ȸ���� �ϳ��� �θ� ������ �ް� ���� Ʈ�������� �ƴ� ȸ���� �Ͼ
		// Ʈ�������� �ڽ� Ʈ�������� ��� ��ġ�� ������ �Ǿ�� �Ѵ�.
		if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !current)
		{
			InheritParentRotationPos();
		}
	}
	// x, y, z �� ȸ�� ������ �̿��Ͽ� ���� ȸ������� ���ϱ� ���� ������� ������.
	// Degree�� Radian���� ��ȯ�Ѵ�.
	Vector3	convertRot = m_RelativeRot.ConvertAngle();
	// ������� ���Ѵ�.
	DirectX::XMVECTOR	qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x,convertRot.y, convertRot.z);
	// ����� ���Ѵ�.
	Matrix	matRot;
	matRot.RotationQuaternion(qut);
	// ������� ������� ȸ������� �̿��ؼ� ������� ���� ȸ����Ų��.
	// ��������� �����Ҷ� 41, 42, 43 �κп� ��ġ�� ����.
	// ���� ��ġ�� �ʿ� ���� ���⸸ �����ϹǷ� ��ġ�� ������ �ȹް� ȸ������ ������ �ް�
	// �����ָ� �ȴ�. ��, x, y, z �� ���� ��� 4x4 ��İ� ���ϱ� ���� w�� 0���� ����ٸ�
	// ����� 41, 42, 43�� ������ �ȹް� �������� �ǹǷ� ��ġ�� ������ �ȹ������� w�� 0����,
	// ������ �������� w�� 1�� �ؼ� ���ϸ� �ȴ�.
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}
//�������
	convertRot = m_WorldRot.ConvertAngle();
//�����
	qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x,convertRot.y, convertRot.z);
//���
	matRot.RotationQuaternion(qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}
	// ȸ���� ���� ����� ��� �Ϸ�Ǿ����� �ڽĳ�带 �ݺ��ϸ� �����Ѵ�.
	m_UpdateRot = true;
	// ��� �ڽĳ�带 �����Ѵ�.
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->InheritRotation(false);
	}
}

void CTransform::InheritParentRotationPos()
{
	if (m_Parent)
	{
		Matrix	matRot;
		Vector3	parentRot;
		if (m_InheritRotX)
		{
			parentRot.x = m_Parent->GetWorldRot().x;
		}
		if (m_InheritRotY)
		{
			parentRot.y = m_Parent->GetWorldRot().y;
		}
		if (m_InheritRotZ)
		{
			parentRot.z = m_Parent->GetWorldRot().z;
		}
		if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
		{
			Vector3	convertRot = parentRot.ConvertAngle();
			DirectX::XMVECTOR	qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
			Matrix	matRot;
			matRot.RotationQuaternion(qut);
			Vector3	parentPos = m_Parent->GetWorldPos();
			memcpy(&matRot._41, &parentPos, sizeof(Vector3));
			Vector3	relativePos = m_RelativePos + m_Offset;
			m_WorldPos = relativePos.TransformCoord(matRot);
		}
		else
		{
			m_WorldPos = m_RelativePos + m_Parent->GetWorldPos() + m_Offset;
		}
	}
	m_UpdatePos = true;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->InheritParentRotationPos();
	}
}

void CTransform::InheritWorldRotation(bool current)
{
	if (m_Parent)
	{
		if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !current)
		{
			InheritWorldParentRotationPos();
		}
	}
	else
	{
		m_WorldRot = m_RelativeRot;
	}
	Vector3	convertRot = m_RelativeRot.ConvertAngle();
	DirectX::XMVECTOR	qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x,convertRot.y, convertRot.z);
	Matrix	matRot;
	matRot.RotationQuaternion(qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}
	convertRot = m_WorldRot.ConvertAngle();
	qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x,convertRot.y, convertRot.z);
	matRot.RotationQuaternion(qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}
	m_UpdateRot = true;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->InheritWorldRotation(false);
	}
}

void CTransform::InheritWorldParentRotationPos()
{
	if (m_Parent)
	{
		Matrix	matRot;
		Vector3	parentRot;
		if (m_InheritRotX)
		{
			parentRot.x = m_Parent->GetWorldRot().x;
		}
		if (m_InheritRotY)
		{
			parentRot.y = m_Parent->GetWorldRot().y;
		}
		if (m_InheritRotZ)
		{
			parentRot.z = m_Parent->GetWorldRot().z;
		}
		if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
		{
			Vector3	convertRot = parentRot.ConvertAngle();
			DirectX::XMVECTOR	qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x,convertRot.y, convertRot.z);
			Matrix	matRot;
			matRot.RotationQuaternion(qut);
			Vector3	parentPos = m_Parent->GetWorldPos();
			memcpy(&matRot._41, &parentPos, sizeof(Vector3));
			Vector3	relativePos = m_RelativePos + m_Offset;
			m_WorldPos = relativePos.TransformCoord(matRot);
		}
		else
		{
			m_RelativePos = m_WorldPos - m_Parent->GetWorldPos() - m_Offset;
		}
	}
	m_UpdatePos = true;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->InheritParentRotationPos();
	}
}

void CTransform::SetRelativeScale(const Vector3& scale)
{
	m_RelativeScale = scale;
	// �θ� ���� ����� ������������� ũ��� ������� ũ��� ������ ũ��� ������ �Ѵ�.
	if (!m_Parent)
	{
		m_WorldScale = scale;
	}
	else
	{
		if (m_InheritScale)
		{
			m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
		}
		else
		{
			m_WorldScale = m_RelativeScale;
		}
	}
	Vector3	meshSizeScale = m_MeshSize * m_WorldScale;
	m_Radius = meshSizeScale.Length() * 0.5f;
	m_Center = (GetMin() + GetMax()) * 0.5f;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildRelativeScale(scale);
	}
	m_UpdateScale = true;
}

void CTransform::SetRelativeScale(const Vector2& scale)
{
	SetRelativeScale(Vector3(scale.x, scale.y, m_RelativeScale.z));
}

void CTransform::SetRelativeScale(float x, float y)
{
	SetRelativeScale(Vector3(x, y, m_RelativeScale.z));
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
	SetRelativeScale(Vector3(x, y, z));
}

void CTransform::SetRelativeScaleX(float x)
{
	SetRelativeScale(Vector3(x, m_RelativeScale.y, m_RelativeScale.z));
}

void CTransform::SetRelativeScaleY(float y)
{
	SetRelativeScale(Vector3(m_RelativeScale.x, y, m_RelativeScale.z));
}

void CTransform::SetRelativeScaleZ(float z)
{
	SetRelativeScale(Vector3(m_RelativeScale.x, m_RelativeScale.y, z));
}

void CTransform::SetRelativeRotation(const Vector3& rot)
{
	m_RelativeRot = rot;
	if (!m_Parent)
	{
		m_WorldRot = rot;
	}
	else
	{
		if (m_InheritRotX)
		{
			m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
		}
		else
		{
			m_WorldRot.x = m_RelativeRot.x;
		}
		if (m_InheritRotY)
		{
			m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
		}
		else
		{
			m_WorldRot.y = m_RelativeRot.y;
		}
		if (m_InheritRotZ)
		{
			m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
		}
		else
		{
			m_WorldRot.z = m_RelativeRot.z;
		}
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildRelativeRotation(rot);
	}
	InheritRotation(true);
}

void CTransform::SetRelativeRotation(const Vector2& rot)
{
	SetRelativeRotation(Vector3(rot.x, rot.y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotation(float x, float y)
{
	SetRelativeRotation(Vector3(x, y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
	SetRelativeRotation(Vector3(x, y, z));
}

void CTransform::SetRelativeRotationX(float x)
{
	SetRelativeRotation(Vector3(x, m_RelativeRot.y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotationY(float y)
{
	SetRelativeRotation(Vector3(m_RelativeRot.x, y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotationZ(float z)
{
	SetRelativeRotation(Vector3(m_RelativeRot.x, m_RelativeRot.y, z));
}

void CTransform::SetRelativePosition(const Vector3& pos)
{
	m_RelativePos = pos;
	if (!m_Parent)
	{
		m_WorldPos = pos+m_Offset;
	}
	else
	{
		m_WorldPos = pos + m_Parent->GetWorldPos()+ m_Offset;
	}
	InheritParentRotationPos();
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildRelativePosition(pos);
	}
}

void CTransform::SetRelativePosition(const Vector2& pos)
{
	SetRelativePosition(Vector3(pos.x, pos.y, m_RelativePos.z));
}

void CTransform::SetRelativePosition(float x, float y)
{
	SetRelativePosition(Vector3(x, y, m_RelativePos.z));
}

void CTransform::SetRelativePosition(float x, float y, float z)
{
	SetRelativePosition(Vector3(x, y, z));
}

void CTransform::SetRelativePositionX(float x)
{
	SetRelativePosition(Vector3(x, m_RelativePos.y, m_RelativePos.z));
}

void CTransform::SetRelativePositionY(float y)
{
	SetRelativePosition(Vector3(m_RelativePos.x, y, m_RelativePos.z));
}

void CTransform::SetRelativePositionZ(float z)
{
	SetRelativePosition(Vector3(m_RelativePos.x, m_RelativePos.y, z));
}

void CTransform::AddRelativeScale(const Vector3& scale)
{
	m_RelativeScale += scale;
	if (!m_Parent)
	{
		m_WorldScale = m_RelativeScale;
	}
	else
	{
		if (m_InheritScale)
		{
			m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
		}
		else
		{
			m_WorldScale = m_RelativeScale;
		}
	}
	Vector3	meshSizeScale = m_MeshSize * m_WorldScale;
	m_Radius = meshSizeScale.Length() * 0.5f;
	m_Center = (GetMin() + GetMax()) * 0.5f;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildRelativeScale(scale);
	}
	m_UpdateScale = true;
}

void CTransform::AddRelativeScale(const Vector2& scale)
{
	AddRelativeScale(Vector3(scale.x, scale.y,0.f));
}

void CTransform::AddRelativeScale(float x, float y)
{
	AddRelativeScale(Vector3(x, y, 0.f));
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
	AddRelativeScale(Vector3(x, y, z));
}

void CTransform::AddRelativeScaleX(float x)
{
	AddRelativeScale(Vector3(x, 0.f, 0.f));
}

void CTransform::AddRelativeScaleY(float y)
{
	AddRelativeScale(Vector3(0.f, y, 0.f));
}

void CTransform::AddRelativeScaleZ(float z)
{
	AddRelativeScale(Vector3(0.f, 0.f, z));
}

void CTransform::AddRelativeRotation(const Vector3& rot)
{
	m_RelativeRot += rot;
	if (!m_Parent)
	{
		m_WorldRot = m_RelativeRot;
	}
	else
	{
		if (m_InheritRotX)
		{
			m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
		}
		else
		{
			m_WorldRot.x = m_RelativeRot.x;
		}
		if (m_InheritRotY)
		{
			m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
		}
		else
		{
			m_WorldRot.y = m_RelativeRot.y;
		}
		if (m_InheritRotZ)
		{
			m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
		}
		else
		{
			m_WorldRot.z = m_RelativeRot.z;
		}
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildRelativeRotation(rot);
	}
	InheritRotation(true);
}

void CTransform::AddRelativeRotation(const Vector2& rot)
{
	AddRelativeRotation(Vector3(rot.x, rot.y, 0.f));
}

void CTransform::AddRelativeRotation(float x, float y)
{
	AddRelativeRotation(Vector3(x, y, 0.f));
}

void CTransform::AddRelativeRotation(float x, float y, float z)
{
	AddRelativeRotation(Vector3(x, y, z));
}

void CTransform::AddRelativeRotationX(float x)
{
	AddRelativeRotation(Vector3(x, 0.f, 0.f));
}

void CTransform::AddRelativeRotationY(float y)
{
	AddRelativeRotation(Vector3(0.f, y, 0.f));
}

void CTransform::AddRelativeRotationZ(float z)
{
	AddRelativeRotation(Vector3(0.f, 0.f, z));
}

void CTransform::AddRelativePosition(const Vector3& pos)
{
	m_RelativePos += pos;
	if (!m_Parent)
	{
		m_WorldPos = m_RelativePos+m_Offset;
	}
	else
	{
		m_WorldPos = m_RelativePos + m_Parent->GetWorldPos()+ m_Offset;
	}
	InheritParentRotationPos();
}

void CTransform::AddRelativePosition(const Vector2& pos)
{
	AddRelativePosition(Vector3(pos.x, pos.y, 0.f));
}

void CTransform::AddRelativePosition(float x, float y)
{
	AddRelativePosition(Vector3(x, y, 0.f));
}

void CTransform::AddRelativePosition(float x, float y, float z)
{
	AddRelativePosition(Vector3(x, y, z));
}

void CTransform::AddRelativePositionX(float x)
{
	AddRelativePosition(Vector3(x, 0.f, 0.f));
}

void CTransform::AddRelativePositionY(float y)
{
	AddRelativePosition(Vector3(0.f, y, 0.f));
}

void CTransform::AddRelativePositionZ(float z)
{
	AddRelativePosition(Vector3(0.f, 0.f, z));
}

void CTransform::SetWorldScale(const Vector3& scale)
{
	m_WorldScale = scale;
	if (!m_Parent)
	{
		m_RelativeScale = m_WorldScale;
	}
	else
	{
		if (m_InheritScale)
		{
			m_RelativeScale = m_WorldScale / m_Parent->GetWorldScale();
		}
		else
		{
			m_RelativeScale = m_WorldScale;
		}
	}
	Vector3	meshSizeScale = m_MeshSize * m_WorldScale;
	m_Radius = meshSizeScale.Length() * 0.5f;
	m_Center = (GetMin() + GetMax()) * 0.5f;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildWorldScale(scale);
	}
	m_UpdateScale = true;
}

void CTransform::SetWorldScale(const Vector2& scale)
{
	SetWorldScale(Vector3(scale.x, scale.y, m_WorldScale.z));
}

void CTransform::SetWorldScale(float x, float y)
{
	SetWorldScale(Vector3(x, y, m_WorldScale.z));
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	SetWorldScale(Vector3(x, y, z));
}

void CTransform::SetWorldScaleX(float x)
{
	SetWorldScale(Vector3(x, m_WorldScale.y, m_WorldScale.z));
}

void CTransform::SetWorldScaleY(float y)
{
	SetWorldScale(Vector3(m_WorldScale.x, y, m_WorldScale.z));
}

void CTransform::SetWorldScaleZ(float z)
{
	SetWorldScale(Vector3(m_WorldScale.x, m_WorldScale.y, z));
}

void CTransform::SetWorldRotation(const Vector3& rot)
{
	m_WorldRot = rot;
	if (!m_Parent)
	{
		m_RelativeRot = m_WorldRot;
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildWorldRotation(rot);
	}
	InheritWorldRotation(true);
}

void CTransform::SetWorldRotation(const Vector2& rot)
{
	SetWorldRotation(Vector3(rot.x, rot.y, m_WorldRot.z));
}

void CTransform::SetWorldRotation(float x, float y)
{
	SetWorldRotation(Vector3(x, y, m_WorldRot.z));
}

void CTransform::SetWorldRotation(float x, float y, float z)
{
	SetWorldRotation(Vector3(x, y, z));
}

void CTransform::SetWorldRotationX(float x)
{
	SetWorldRotation(Vector3(x, m_WorldRot.y, m_WorldRot.z));
}

void CTransform::SetWorldRotationY(float y)
{
	SetWorldRotation(Vector3(m_WorldRot.x, y, m_WorldRot.z));
}

void CTransform::SetWorldRotationZ(float z)
{
	SetWorldRotation(Vector3(m_WorldRot.x, m_WorldRot.y, z));
}

void CTransform::SetWorldRotationAxis(const Vector3& originDir, const Vector3& view)
{
	m_UpdateRotAxis = true;
	Vector3	rotAxis = originDir.Cross(view);
	rotAxis.Normalize();
	float angle = originDir.Angle(view);
	m_matRot = DirectX::XMMatrixRotationAxis(rotAxis.Convert(), DegreeToRadian(angle));
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::axis[i].TransformNormal(m_matRot);
		m_WorldAxis[i].Normalize();
	}
}

void CTransform::SetWorldPosition(const Vector3& pos)
{
	m_WorldPos = pos;
	if (!m_Parent)
	{
		m_RelativePos = m_WorldPos- m_Offset;
	}
	InheritWorldParentRotationPos();
}

void CTransform::SetWorldPosition(const Vector2& pos)
{
	SetWorldPosition(Vector3(pos.x, pos.y, m_WorldPos.z));
}

void CTransform::SetWorldPosition(float x, float y)
{
	SetWorldPosition(Vector3(x, y, m_WorldPos.z));
}

void CTransform::SetWorldPosition(float x, float y, float z)
{
	SetWorldPosition(Vector3(x, y, z));
}

void CTransform::SetWorldPositionX(float x)
{
	SetWorldPosition(Vector3(x, m_WorldPos.y, m_WorldPos.z));
}

void CTransform::SetWorldPositionY(float y)
{
	SetWorldPosition(Vector3(m_WorldPos.x, y, m_WorldPos.z));
}

void CTransform::SetWorldPositionZ(float z)
{
	SetWorldPosition(Vector3(m_WorldPos.x, m_WorldPos.y, z));
}

void CTransform::SetOffset(const Vector3& offset)
{
	m_Offset = offset;
	m_WorldPos = m_RelativePos + offset;
	InheritWorldParentRotationPos();
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetOffset(offset);
	}
}

void CTransform::SetOffset(const Vector2& offset)
{
	SetOffset(Vector3(offset.x, offset.y, m_Offset.z));
}

void CTransform::SetOffsetX(float x)
{
	SetOffset(Vector3(x, m_Offset.y, m_Offset.z));
}

void CTransform::SetOffsetY(float y)
{
	SetOffset(Vector3(m_Offset.x, y, m_Offset.z));
}

void CTransform::SetOffsetZ(float z)
{
	SetOffset(Vector3(m_Offset.x, m_Offset.y, z));
}

void CTransform::AddWorldScale(const Vector3& scale)
{
	m_WorldScale += scale;
	if (!m_Parent)
	{
		m_RelativeScale = m_WorldScale;
	}
	Vector3	meshSizeScale = m_MeshSize * m_WorldScale;
	m_Radius = meshSizeScale.Length() * 0.5f;
	m_Center = (GetMin() + GetMax()) * 0.5f;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildWorldScale(scale);
	}
	m_UpdateScale = true;
}

void CTransform::AddWorldScale(const Vector2& scale)
{
	AddWorldScale(Vector3(scale.x, scale.y, 0.f));
}

void CTransform::AddWorldScale(float x, float y)
{
	AddWorldScale(Vector3(x, y, 0.f));
}

void CTransform::AddWorldScale(float x, float y, float z)
{
	AddWorldScale(Vector3(x, y, z));
}

void CTransform::AddWorldScaleX(float x)
{
	AddWorldScale(Vector3(x, 0.f, 0.f));
}

void CTransform::AddWorldScaleY(float y)
{
	AddWorldScale(Vector3(0.f, y, 0.f));
}

void CTransform::AddWorldScaleZ(float z)
{
	AddWorldScale(Vector3(0.f, 0.f, z));
}

void CTransform::AddWorldRotation(const Vector3& rot)
{
	m_WorldRot += rot;
	if (!m_Parent)
	{
		m_RelativeRot = m_WorldRot;
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildWorldRotation(rot);
	}
	InheritWorldRotation(true);
}

void CTransform::AddWorldRotation(const Vector2& rot)
{
	AddWorldRotation(Vector3(rot.x, rot.y, 0.f));
}

void CTransform::AddWorldRotation(float x, float y)
{
	AddWorldRotation(Vector3(x, y, 0.f));
}

void CTransform::AddWorldRotation(float x, float y, float z)
{
	AddWorldRotation(Vector3(x, y, z));
}

void CTransform::AddWorldRotationX(float x)
{
	AddWorldRotation(Vector3(x, 0.f, 0.f));
}

void CTransform::AddWorldRotationY(float y)
{
	AddWorldRotation(Vector3(0.f, y, 0.f));
}

void CTransform::AddWorldRotationZ(float z)
{
	AddWorldRotation(Vector3(0.f, 0.f, z));
}

void CTransform::AddWorldPosition(const Vector3& pos)
{
	m_WorldPos += pos;
	if (!m_Parent)
	{
		m_RelativePos = m_WorldPos- m_Offset;
	}
	InheritWorldParentRotationPos();
}

void CTransform::AddWorldPosition(const Vector2& pos)
{
	AddWorldPosition(Vector3(pos.x, pos.y, 0.f));
}

void CTransform::AddWorldPosition(float x, float y)
{
	AddWorldPosition(Vector3(x, y, 0.f));
}

void CTransform::AddWorldPosition(float x, float y, float z)
{
	AddWorldPosition(Vector3(x, y, z));
}

void CTransform::AddWorldPositionX(float x)
{
	AddWorldPosition(Vector3(x, 0.f, 0.f));
}

void CTransform::AddWorldPositionY(float y)
{
	AddWorldPosition(Vector3(0.f, y, 0.f));
}

void CTransform::AddWorldPositionZ(float z)
{
	AddWorldPosition(Vector3(0.f, 0.f, z));
}

void CTransform::AddOffset(const Vector3& offset)
{
	m_Offset += offset;
	m_WorldPos = m_RelativePos + m_Offset;
	InheritWorldParentRotationPos();
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddOffset(offset);
	}
}

void CTransform::AddOffset(const Vector2& offset)
{
	AddOffset(Vector3(offset.x, offset.y, 0.f));
}

void CTransform::AddOffsetX(float x)
{
	AddOffset(Vector3(x, 0.f, 0.f));
}

void CTransform::AddOffsetY(float y)
{
	AddOffset(Vector3(0.f, y, 0.f));
}

void CTransform::AddOffsetZ(float z)
{
	AddOffset(Vector3(0.f, 0.f, z));
}

void CTransform::Start()
{
	InheritRotation(true);
	InheritParentRotationPos();
}

bool CTransform::Init()
{
	return true;
}

void CTransform::Update(float deltaTime)
{
	if (m_Is2D)
	{
		float	y = m_WorldPos.y - m_Pivot.y * m_WorldScale.y;
		if (m_MinY > y)
		{
			m_MinY = y;
		}
		if (m_MaxY < y)
		{
			m_MaxY = y;
		}
	}
}

void CTransform::PostUpdate(float deltaTime)
{
	if (m_Socket)
	{
		m_WorldScale *= m_Socket->GetMoveScale();
		m_WorldRot += m_Socket->GetMoveRot();
		m_WorldPos += m_Socket->GetMovePos();
		m_UpdateScale = true;
		m_UpdatePos = true;
		m_UpdateRot = true;
	}
	m_Center = (GetMin() + GetMax()) * 0.5f;
	m_Radius = (GetMax() - GetMin()).Length() * 0.5f;
	Vector3	worldPos = m_WorldPos;
	if (m_Is2D)
	{
		float	sizeY = (m_MaxY + 1.f) - (m_MinY - 1.f);
		float	y = worldPos.y - m_Pivot.y * m_WorldScale.y;
		float	z = (y - (m_MinY - 1.f)) / (sizeY / 1000.f);
		if (z != m_2DZ)
		{
			worldPos.z = z;
			m_2DZ = z;
			m_UpdatePos = true;
		}
	}
	if (m_UpdateScale)
	{
		m_matScale.Scaling(m_WorldScale);
	}
	// x, y, z ������ ȸ���� �ϰų� �ƴϸ� ������ ���� �̿��ؼ� ȸ���� �ϰų� ���� �ϳ��� ��츸 ����� �Ѵ�.
	if (m_UpdateRot && !m_UpdateRotAxis)
	{
		m_matRot.Rotation(m_WorldRot);
	}
	if (m_UpdatePos)
	{
		m_matPos.Translation(worldPos);
	}
	// ���� ����� ��ġ, ũ��, ȸ���� �ϳ��� ��ȭ�� �־�� �����Ѵ�.
	// ��ȭ�� ���� ��� ����� ���Ѵ�.
	// ������� = ũ�� * ���� * �̵� * ���� * �θ�
	if (m_UpdateScale || m_UpdateRot || m_UpdateRotAxis || m_UpdatePos)
	{
		m_matWorld = m_matScale * m_matRot * m_matPos;
	}
	m_UpdateScale = false;
	m_UpdateRot = false;
	m_UpdateRotAxis = false;
	m_UpdatePos = false;
}

CTransform* CTransform::Clone() const
{
	return new CTransform(*this);
}

void CTransform::SetTransform()
{
	m_CBuffer->SetWorldMatrix(m_matWorld);
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();
	m_CBuffer->SetViewMatrix(matView);
	m_CBuffer->SetProjMatrix(matProj);
	m_CBuffer->SetPivot(m_Pivot);
	m_CBuffer->SetMeshSize(m_MeshSize);
	m_CBuffer->UpdateBuffer();
	Clear();
}

void CTransform::SetShadowMapTransform()
{
	m_CBuffer->SetWorldMatrix(m_matWorld);
	// ������ ��ġ���� �ٶ󺸴� �׸��� �ʿ� ������� �̿��Ѵ�.
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetShadowViewMatrix();
	// �׸��� �ʿ� �ػ󵵰� ����Ǵ� ������ĸ� �̿��Ѵ�
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetShadowProjMatrix();
	m_CBuffer->SetViewMatrix(matView);
	m_CBuffer->SetProjMatrix(matProj);
	m_CBuffer->SetPivot(m_Pivot);
	m_CBuffer->SetMeshSize(m_MeshSize);
	m_CBuffer->UpdateBuffer();
	Clear();
}

void CTransform::ComputeWorld()
{
	m_matWorld = m_matScale * m_matRot * m_matPos;
}

void CTransform::Save(FILE* file)
{
	fwrite(&m_Is2D, 1, 1, file);
	fwrite(&m_2DZ, sizeof(float), 1, file);
	fwrite(&m_InheritScale, 1, 1, file);
	fwrite(&m_InheritRotX, 1, 1, file);
	fwrite(&m_InheritRotY, 1, 1, file);
	fwrite(&m_InheritRotZ, 1, 1, file);
	fwrite(&m_InheritParentRotationPosX, 1, 1, file);
	fwrite(&m_InheritParentRotationPosY, 1, 1, file);
	fwrite(&m_InheritParentRotationPosZ, 1, 1, file);
	fwrite(&m_RelativeScale, sizeof(Vector3), 1, file);
	fwrite(&m_RelativeRot, sizeof(Vector3), 1, file);
	fwrite(&m_RelativePos, sizeof(Vector3), 1, file);
	fwrite(m_RelativeAxis, sizeof(Vector3), (size_t)AXIS::AXIS_MAX, file);
	fwrite(&m_WorldScale, sizeof(Vector3), 1, file);
	fwrite(&m_WorldRot, sizeof(Vector3), 1, file);
	fwrite(&m_WorldPos, sizeof(Vector3), 1, file);
	fwrite(m_WorldAxis, sizeof(Vector3), (size_t)AXIS::AXIS_MAX, file);
	fwrite(&m_OriginWorldScale, sizeof(Vector3), 1, file);
	fwrite(&m_OriginWorldRot, sizeof(Vector3), 1, file);
	fwrite(&m_OriginWorldPos, sizeof(Vector3), 1, file);
	fwrite(&m_Pivot, sizeof(Vector3), 1, file);
	fwrite(&m_MeshSize, sizeof(Vector3), 1, file);
}

void CTransform::Load(FILE* file)
{
	fread(&m_Is2D, 1, 1, file);
	fread(&m_2DZ, sizeof(float), 1, file);
	fread(&m_InheritScale, 1, 1, file);
	fread(&m_InheritRotX, 1, 1, file);
	fread(&m_InheritRotY, 1, 1, file);
	fread(&m_InheritRotZ, 1, 1, file);
	fread(&m_InheritParentRotationPosX, 1, 1, file);
	fread(&m_InheritParentRotationPosY, 1, 1, file);
	fread(&m_InheritParentRotationPosZ, 1, 1, file);
	fread(&m_RelativeScale, sizeof(Vector3), 1, file);
	fread(&m_RelativeRot, sizeof(Vector3), 1, file);
	fread(&m_RelativePos, sizeof(Vector3), 1, file);
	fread(m_RelativeAxis, sizeof(Vector3), (size_t)AXIS::AXIS_MAX, file);
	fread(&m_WorldScale, sizeof(Vector3), 1, file);
	fread(&m_WorldRot, sizeof(Vector3), 1, file);
	fread(&m_WorldPos, sizeof(Vector3), 1, file);
	fread(m_WorldAxis, sizeof(Vector3), (size_t)AXIS::AXIS_MAX, file);
	fread(&m_OriginWorldScale, sizeof(Vector3), 1, file);
	fread(&m_OriginWorldRot, sizeof(Vector3), 1, file);
	fread(&m_OriginWorldPos, sizeof(Vector3), 1, file);
	fread(&m_Pivot, sizeof(Vector3), 1, file);
	fread(&m_MeshSize, sizeof(Vector3), 1, file);
	m_UpdateScale = true;
	m_UpdateRot = true;
	m_UpdatePos = true;
}

void CTransform::SetChildRelativeScale(const Vector3& scale)
{
	if (m_InheritScale)
	{
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
	}
	else
	{
		m_WorldScale = m_RelativeScale;
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildRelativeScale(scale);
	}
}

void CTransform::SetChildRelativeRotation(const Vector3& rot)
{
	if (m_InheritRotX)
	{
		m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
	}
	if (m_InheritRotY)
	{
		m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
	}
	if (m_InheritRotZ)
	{
		m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
	}
	Matrix	matRot;
	Vector3	convertRot = m_WorldRot.ConvertAngle();
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	matRot.RotationQuaternion(Qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}
	m_UpdateRot = true;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildRelativeRotation(rot);
	}
}

void CTransform::SetChildRelativePosition(const Vector3& pos)
{
	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos()+m_Offset;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildRelativePosition(pos);
	}
}

void CTransform::AddChildRelativeScale(const Vector3& scale)
{
	if (m_InheritScale)
	{
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
	}
	else
	{
		m_WorldScale = m_RelativeScale;
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildRelativeScale(scale);
	}
}

void CTransform::AddChildRelativeRotation(const Vector3& rot)
{
	if (m_InheritRotX)
	{
		m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
	}
	if (m_InheritRotY)
	{
		m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
	}
	if (m_InheritRotZ)
	{
		m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
	}
	Matrix	matRot;
	Vector3	convertRot = m_WorldRot.ConvertAngle();
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	matRot.RotationQuaternion(Qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}
	m_UpdateRot = true;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildRelativeRotation(rot);
	}
}

void CTransform::AddChildRelativePosition(const Vector3& pos)
{
	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos()+ m_Offset;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildRelativePosition(pos);
	}
}

void CTransform::SetChildWorldScale(const Vector3& scale)
{
	if (m_InheritScale)
	{
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
	}
	else
	{
		m_WorldScale = m_RelativeScale;
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildWorldScale(scale);
	}
	m_UpdateScale = true;
}

void CTransform::SetChildWorldRotation(const Vector3& rot)
{
	if (m_InheritRotX)
	{
		m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
	}
	if (m_InheritRotY)
	{
		m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
	}
	if (m_InheritRotZ)
	{
		m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
	}
	Vector3	convertRot = m_RelativeRot.ConvertAngle();
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}
	convertRot = m_WorldRot.ConvertAngle();
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x,convertRot.y, convertRot.z);
	matRot.RotationQuaternion(Qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}
	m_UpdateRot = true;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildWorldRotation(rot);
	}
}

void CTransform::SetChildWorldPosition(const Vector3& pos)
{
	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos();
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetChildWorldPosition(pos);
	}
}

void CTransform::AddChildWorldScale(const Vector3& scale)
{
	m_WorldScale += scale;
	if (m_InheritScale)
	{
		m_RelativeScale = m_WorldScale / m_Parent->GetWorldScale();
	}
	else
	{
		m_RelativeScale = m_WorldScale;
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildWorldScale(scale);
	}
	m_UpdateScale = true;
}

void CTransform::AddChildWorldRotation(const Vector3& rot)
{
	if (m_InheritRotX)
	{
		m_WorldRot.x += rot.x;
		m_RelativeRot.x = m_WorldRot.x - m_Parent->GetWorldRot().x;
	}
	if (m_InheritRotY)
	{
		m_WorldRot.y += rot.y;
		m_RelativeRot.y = m_WorldRot.y - m_Parent->GetWorldRot().y;
	}
	if (m_InheritRotZ)
	{
		m_WorldRot.z += rot.z;
		m_RelativeRot.z = m_WorldRot.z - m_Parent->GetWorldRot().z;
	}
	Vector3	convertRot = m_RelativeRot.ConvertAngle();
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}
	convertRot = m_WorldRot.ConvertAngle();
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(convertRot.x, convertRot.y, convertRot.z);
	matRot.RotationQuaternion(Qut);
	for (int i = 0; i < (int)AXIS::AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}
	m_UpdateRot = true;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildWorldRotation(rot);
	}
}

void CTransform::AddChildWorldPosition(const Vector3& pos)
{
	m_WorldPos += pos;
	m_RelativePos = m_WorldPos - m_Parent->GetWorldPos();
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddChildWorldPosition(pos);
	}
}

float CTransform::GetRadius()	const
{
	if (CEngine::GetInst()->GetEditorMode())
	{
		if (m_Radius < 10.f)
		{
			return 10.f;
		}
	}
	return m_Radius;
}