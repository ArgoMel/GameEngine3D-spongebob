#include "NavigationAgent.h"
#include "SceneComponent.h"
#include "../Thread/DataStream.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"

CNavigationAgent::CNavigationAgent()
{
	m_ComponentType = ComponentType::Object;
	SetTypeID<CNavigationAgent>();
	m_ComponentTypeName = "NavigationAgent";
	m_MoveSpeed = 400.f;
}

CNavigationAgent::CNavigationAgent(const CNavigationAgent& obj)
	: CObjectComponent(obj)
{
	m_MoveSpeed = obj.m_MoveSpeed;
}

CNavigationAgent::~CNavigationAgent()
{
}

void CNavigationAgent::SetUpdateComponent(CSceneComponent* component)
{
	m_UpdateComponent = component;
}

void CNavigationAgent::SetMoveSpeed(float speed)
{
	m_MoveSpeed = speed;
}

void CNavigationAgent::AddPathList(const std::list<Vector2>& pathList)
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
		stream.AddData<Vector2>(&(*iter), sizeof(Vector2));
	}
	m_FindQueue.push(0, stream.GetSize(),buffer);
}

bool CNavigationAgent::Move(const Vector2& targetPos)
{
	if (!m_UpdateComponent)
	{
		return false;
	}
	m_Scene->GetNavigationManager()->Move(this, targetPos);
	return true;
}

void CNavigationAgent::Destroy()
{
	CObjectComponent::Destroy();
}

void CNavigationAgent::Start()
{
	CObjectComponent::Start();
	if (!m_UpdateComponent)
	{
		m_UpdateComponent = m_Owner->GetRootComponent();
	}
}

bool CNavigationAgent::Init()
{
	if (!CObjectComponent::Init())
	{
		return false;
	}
	return true;
}

void CNavigationAgent::Update(float deltaTime)
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
			Vector2	pos;
			stream.GetData<Vector2>(&pos, sizeof(Vector2));
			m_PathList.push_back(pos);
		}
	}
	if (!m_PathList.empty() && m_UpdateComponent)
	{
		Vector2	targetPos = m_PathList.front();
		Vector2	srcPos = m_UpdateComponent->GetWorldPos();
		float	dist = srcPos.Distance(targetPos);
		Vector2	dir = targetPos - srcPos;
		dir.Normalize();
		float	speed = m_MoveSpeed * deltaTime;
		srcPos += dir * speed;
		// 도착했을 경우
		if (speed >= dist)
		{
			srcPos = targetPos;
			m_PathList.pop_front();
		}
		m_UpdateComponent->SetWorldPosition(srcPos);
	}
}

void CNavigationAgent::PostUpdate(float deltaTime)
{
	CObjectComponent::PostUpdate(deltaTime);
}

void CNavigationAgent::Render()
{
	CObjectComponent::Render();
}

CNavigationAgent* CNavigationAgent::Clone() const
{
	return new CNavigationAgent(*this);
}

void CNavigationAgent::Save(FILE* file)
{
	CObjectComponent::Save(file);
}

void CNavigationAgent::Load(FILE* file)
{
	CObjectComponent::Load(file);
}
