#pragma once
#include "../EngineInfo.h"

class CThread
{
private:
	friend class CThreadManager;
	static unsigned int __stdcall ThreadFunction(void* arg);
protected:
	HANDLE	m_Thread;
	HANDLE	m_StartEvent;
	CRITICAL_SECTION	m_CRT;
	bool	m_Loop;
	bool	m_Suspend;
	CThread();
	virtual ~CThread() = 0;
public:
	bool IsSuspend()	const
	{
		return m_Suspend;
	}
	void SetLoop(bool Loop)
	{
		m_Loop = Loop;
	}
	//일시중지
	void Suspend();	
	//재시작
	void Resume();
	//초기화
	void ReStart();	
	//중지
	void Stop();	
	void Start();
	virtual bool Init();
	virtual void Run() = 0;
};

