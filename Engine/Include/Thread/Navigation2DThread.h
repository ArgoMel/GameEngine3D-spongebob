#pragma once
#include "NavigationThread.h"
#include "ThreadQueue.h"

class CNavigation2DThread : public CNavigationThread
{
private:
	friend class CThreadManager;
	class CTileMapComponent* m_TileMap;
	class CNavigation* m_Navigation;
protected:
	CNavigation2DThread();
	virtual ~CNavigation2DThread();
public:
	virtual void SetNavigationComponent(class CSceneComponent* navComponent);
	virtual void Run();
};

