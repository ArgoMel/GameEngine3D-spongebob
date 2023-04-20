#pragma once
#include "Thread.h"
#include "ThreadQueue.h"

enum class ELoadingHeader
{
	LoadingRate
};

class CLoadingThread : public CThread
{
private:
	friend class CThreadManager;
	void LoadingCallback(float rate);
protected:
	std::string		m_FileName;
	std::string		m_PathName;
	std::string		m_FullPath;
	CThreadQueue	m_Queue;
	bool			m_LoadComplete;
	CLoadingThread();
	virtual ~CLoadingThread();
public:
	CThreadQueue* GetQueue()
	{
		return &m_Queue;
	}
	bool IsLoadComplete()	const
	{
		return m_LoadComplete;
	}
	void SetLoadingSceneFileName(const std::string& fileName)
	{
		m_FileName = fileName;
	}
	void SetLoadingScenePathName(const std::string& pathName)
	{
		m_PathName = pathName;
	}
	void SetLoadingSceneFullPath(const std::string& fullPath)
	{
		m_FullPath = fullPath;
	}
	virtual void Run();
};

