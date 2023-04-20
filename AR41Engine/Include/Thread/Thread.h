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
	//�Ͻ�����
	void Suspend();	
	//�����
	void Resume();
	//�ʱ�ȭ
	void ReStart();	
	//����
	void Stop();	
	void Start();
	virtual bool Init();
	virtual void Run() = 0;
};

