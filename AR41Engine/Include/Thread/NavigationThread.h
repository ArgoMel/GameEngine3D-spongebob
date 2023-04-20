#pragma once
#include "Thread.h"
#include "ThreadQueue.h"

class CNavigationThread : public CThread
{
private:
	friend class CThreadManager;
protected:
	CThreadQueue	m_InputQueue;
	CNavigationThread();
	virtual ~CNavigationThread();
public:
	int GetWorkCount()
	{
		return m_InputQueue.size();
	}
	virtual void SetNavigationComponent(class CSceneComponent* navComponent) = 0;
	void AddInputData(class CNavigationAgent* agent, const Vector2& end);
	void AddInputData(class CNavigationAgent3D* agent, const Vector3& end);
	virtual void Run() = 0;
};

