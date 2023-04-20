#pragma once
#include "Thread.h"

class CThreadManager
{
	DECLARE_SINGLE(CThreadManager)
private:
	std::unordered_map<std::string, CThread*>	m_mapThread;
	std::unordered_map<std::string, CRITICAL_SECTION*>	m_mapCriticalSection;
public:
	bool Init();
	bool Suspend(const std::string& name);
	bool Resume(const std::string& name);
	bool ReStart(const std::string& name);
	bool Delete(const std::string& name);
	bool Start(const std::string& name);
	void CreateNavigationThread(class CSceneComponent* navComponent, bool Is2D = true);
	void DeleteNavigationThread(class CSceneComponent* navComponent);
	void CreateNavigationThread(class CNavigationMesh* navMesh);
	void DeleteNavigationThread(class CNavigationMesh* navMesh);
	CThread* FindThread(const std::string& name);
	bool CreateCriticalSection(const std::string& name);
	bool DeleteCriticalSection(const std::string& name);
	CRITICAL_SECTION* FindCriticalSection(const std::string& name);
	template <typename T>
	T* Create(const std::string& name)
	{
		T* thread = (T*)FindThread(name);
		if (thread)
		{
			return thread;
		}
		thread = new T;
		if (!thread->Init())
		{
			SAFE_DELETE(thread);
			return nullptr;
		}
		m_mapThread.insert(std::make_pair(name, thread));
		return thread;
	}
};

