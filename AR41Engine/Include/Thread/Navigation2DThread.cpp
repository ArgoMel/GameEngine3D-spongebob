#include "Navigation2DThread.h"
#include "Navigation.h"
#include "DataStream.h"
#include "../Component/NavigationAgent.h"
#include "../Component/SceneComponent.h"

CNavigation2DThread::CNavigation2DThread()
	: m_TileMap(nullptr)
	, m_Navigation(nullptr)
{
}

CNavigation2DThread::~CNavigation2DThread()
{
	SAFE_DELETE(m_Navigation);
}

void CNavigation2DThread::SetNavigationComponent(CSceneComponent* navComponent)
{
	m_TileMap = (CTileMapComponent*)navComponent;
	SAFE_DELETE(m_Navigation);
	m_Navigation = new CNavigation;
	m_Navigation->CreateNavigation((CTileMapComponent*)m_TileMap);
}

void CNavigation2DThread::Run()
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
		CNavigationAgent* agent = (CNavigationAgent*)address;
		if (!agent)
		{
			if (m_InputQueue.empty())
			{
				Suspend();
			}
			return;
		}
		Vector2	start;
		Vector2	end;
		stream.GetData<Vector2>(&start, 8);
		stream.GetData<Vector2>(&end, 8);
		// 길을 찾아준다.
		std::list<Vector2>	pathList;
		m_Navigation->FindPath(start, end, pathList);
		// 길찾기가 끝나면 결과를 반환해준다.
		agent->AddPathList(pathList);
	}
}