#include "Navigation3DThread.h"
#include "DataStream.h"
#include "Navigation3D.h"
#include "../Component/NavigationAgent3D.h"
#include "../Component/SceneComponent.h"
#include "../Scene/NavigationMesh.h"

CNavigation3DThread::CNavigation3DThread()
	: m_NavMesh(nullptr)
	, m_Navigation(nullptr)
{
}

CNavigation3DThread::~CNavigation3DThread()
{
	SAFE_DELETE(m_Navigation);
}

void CNavigation3DThread::SetNavigationMesh(CNavigationMesh* navMesh)
{
	m_NavMesh = navMesh;
	SAFE_DELETE(m_Navigation);
	m_Navigation = new CNavigation3D;
	m_Navigation->CreateNavigation(m_NavMesh);
}

void CNavigation3DThread::SetNavigationComponent(CSceneComponent* navComponent)
{
}

void CNavigation3DThread::Run()
{
	if (!m_InputQueue.empty())
	{
		int	header = 0;
		int size = 0;
		unsigned char	buffer[1024] = {};
		m_InputQueue.pop(&header, &size, buffer);
		CDataStream	stream;
		stream.SetBuffer(buffer);
		unsigned __int64	address = 0;
		stream.GetData<unsigned __int64>(&address, 8);
		CNavigationAgent3D* agent = (CNavigationAgent3D*)address;
		if (!agent)
		{
			if (m_InputQueue.empty())
			{
				Suspend();
			}
			return;
		}
		Vector3	start;
		Vector3	end;
		stream.GetData<Vector3>(&start, 12);
		stream.GetData<Vector3>(&end, 12);
		std::list<Vector3>	pathList;
		m_Navigation->FindPath(start, end, pathList);
		agent->AddPathList(pathList);
		if (m_InputQueue.empty() && m_Loop)
		{
			Suspend();
		}
	}
}