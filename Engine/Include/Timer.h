#pragma once
#include "EngineInfo.h"

class CTimer
{
private:
	LARGE_INTEGER	m_Second;
	LARGE_INTEGER	m_Time;
	float			m_DeltaTime;
	float			m_FPS;
	float			m_FPSTime;
	int				m_Tick;
public:
	CTimer();
	~CTimer();
	float GetDeltaTime()	const
	{
		return m_DeltaTime;
	}
	float GetFPS()	const
	{
		return m_FPS;
	}
	void Init();
	void Update();
};

