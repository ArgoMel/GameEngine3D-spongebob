#pragma once
#include "NavigationThread.h"
#include "ThreadQueue.h"

class CNavigation3DThread : public CNavigationThread
{
private:
	friend class CThreadManager;
	class CNavigationMesh* m_NavMesh;
	class CNavigation3D* m_Navigation;
protected:
	CNavigation3DThread();
	virtual ~CNavigation3DThread();
public:
	class CNavigationMesh* GetNavigationMesh()	const
	{
		return m_NavMesh;
	}
	class CNavigation3D* GetNavigation()	const
	{
		return m_Navigation;
	}
	void SetNavigationMesh(class CNavigationMesh* navMesh);
	virtual void SetNavigationComponent(class CSceneComponent* navComponent);
	virtual void Run();
};

