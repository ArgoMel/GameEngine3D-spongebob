#include "NavigationAgent3D.h"
#include "SceneComponent.h"
#include "../Thread/DataStream.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "../Scene/NavigationManager3D.h"

CNavigationAgent3D::CNavigationAgent3D()
{
	m_ComponentType = ComponentType::Object;
	SetTypeID<CNavigationAgent3D>();
	m_ComponentTypeName = "NavigationAgent3D";
	m_MoveSpeed = 400.f;
}

CNavigationAgent3D::CNavigationAgent3D(const CNavigationAgent3D& obj)
	: CObjectComponent(obj)
{
	m_MoveSpeed = obj.m_MoveSpeed;
}

CNavigationAgent3D::~CNavigationAgent3D()
{
}

void CNavigationAgent3D::SetUpdateComponent(CSceneComponent* component)
{
	m_UpdateComponent = component;
}

void CNavigationAgent3D::SetMoveSpeed(float speed)
{
	m_MoveSpeed = speed;
}

void CNavigationAgent3D::AddPathList(const std::list<Vector3>& pathList)
{
	CDataStream		stream;
	unsigned char	buffer[1024] = {};
	stream.SetBuffer(buffer);
	int	count = (int)pathList.size();
	stream.AddData<int>(&count, sizeof(int));
	auto	iter = pathList.begin();
	auto	iterEnd = pathList.end();
	for (; iter != iterEnd; ++iter)
	{
		stream.AddData<Vector3>(&(*iter), sizeof(Vector3));
	}
	m_FindQueue.push(0, stream.GetSize(), buffer);
}

bool CNavigationAgent3D::Move(const Vector3& targetPos)
{
	if (!m_UpdateComponent)
	{
		return false;
	}
	m_Scene->GetNavigationManager()->Move(this, targetPos);
	return true;
}

bool CNavigationAgent3D::MovePicking()
{
	if (!m_UpdateComponent)
	{
		return false;
	}
	Vector3	pickPos;
	if (!m_Scene->GetNavigationManager()->GetPickingPos(pickPos))
	{
		return false;
	}
	m_Scene->GetNavigationManager()->Move(this, pickPos);
	return true;
}

void CNavigationAgent3D::Destroy()
{
	CObjectComponent::Destroy();
}

void CNavigationAgent3D::Start()
{
	CObjectComponent::Start();
	if (!m_UpdateComponent)
	{
		m_UpdateComponent = m_Owner->GetRootComponent();
	}
}

bool CNavigationAgent3D::Init()
{
	if (!CObjectComponent::Init())
	{
		return false;
	}
	return true;
}

void CNavigationAgent3D::Update(float deltaTime)
{
	CObjectComponent::Update(deltaTime);
	if (!m_FindQueue.empty())
	{
		m_PathList.clear();
		int	header = 0;
		int size = 0;
		unsigned char	buffer[1024] = {};
		m_FindQueue.pop(&header, &size, buffer);
		CDataStream		stream;
		stream.SetBuffer(buffer);
		int		count = 0;
		stream.GetData<int>(&count, sizeof(int));
		for (int i = 0; i < count; ++i)
		{
			Vector3	pos;
			stream.GetData<Vector3>(&pos, sizeof(Vector3));
			m_PathList.push_back(pos);
		}
	}
	if (!m_PathList.empty() && m_UpdateComponent)
	{
		Vector3	targetPos = m_PathList.front();
		Vector3	srcPos = m_UpdateComponent->GetWorldPos();
		float	dist = srcPos.Distance(targetPos);
		Vector3	dir = targetPos - srcPos;
		dir.Normalize();
		float	speed = m_MoveSpeed * deltaTime;
		srcPos += dir * speed;
		srcPos.y = m_Scene->GetNavigationManager()->GetHeight(srcPos);
		if (speed >= dist)
		{
			srcPos = targetPos;
			m_PathList.pop_front();
		}
		m_UpdateComponent->SetWorldPosition(srcPos);
	}
}

void CNavigationAgent3D::PostUpdate(float deltaTime)
{
	CObjectComponent::PostUpdate(deltaTime);
}

void CNavigationAgent3D::Render()
{
	CObjectComponent::Render();
}

CNavigationAgent3D* CNavigationAgent3D::Clone() const
{
	return new CNavigationAgent3D(*this);
}

void CNavigationAgent3D::Save(FILE* file)
{
	CObjectComponent::Save(file);
}

void CNavigationAgent3D::Load(FILE* file)
{
	CObjectComponent::Load(file);
}
