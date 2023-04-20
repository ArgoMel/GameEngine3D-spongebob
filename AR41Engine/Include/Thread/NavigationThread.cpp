#include "NavigationThread.h"
#include "DataStream.h"
#include "Navigation.h"
#include "../Component/NavigationAgent.h"
#include "../Component/NavigationAgent3D.h"
#include "../Component/SceneComponent.h"

CNavigationThread::CNavigationThread()
{
}

CNavigationThread::~CNavigationThread()
{
}

void CNavigationThread::AddInputData(CNavigationAgent* agent, const Vector2& end)
{
	unsigned char	buffer[1024] = {};
	CDataStream	stream;
	stream.SetBuffer(buffer);
	unsigned __int64	address = (unsigned __int64)agent;
	stream.AddData<unsigned __int64>(&address, 8);
	Vector3	ownerPos = agent->GetUpdateComponent()->GetWorldPos();
	Vector2 start = Vector2(ownerPos.x, ownerPos.y);
	stream.AddData<Vector2>(&start, 8);
	stream.AddData<Vector2>(&end, 8);
	m_InputQueue.push(0, stream.GetSize(), buffer);
	if (IsSuspend())
	{
		ReStart();
	}
}

void CNavigationThread::AddInputData(CNavigationAgent3D* agent, const Vector3& end)
{
	unsigned char	buffer[1024] = {};
	CDataStream	stream;
	stream.SetBuffer(buffer);
	unsigned __int64	address = (unsigned __int64)agent;
	stream.AddData<unsigned __int64>(&address, 8);
	Vector3	ownerPos = agent->GetUpdateComponent()->GetWorldPos();
	stream.AddData<Vector3>(&ownerPos, 12);
	stream.AddData<Vector3>(&end, 12);
	m_InputQueue.push(0, stream.GetSize(), buffer);
	if (IsSuspend())
	{
		ReStart();
	}
}