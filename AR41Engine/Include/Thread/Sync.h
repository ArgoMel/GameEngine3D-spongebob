#pragma once
#include "../EngineInfo.h"

class CSync
{
private:
	CRITICAL_SECTION* m_Crt;
public:
	CSync(CRITICAL_SECTION* crt)
	{
		m_Crt = crt;
		EnterCriticalSection(m_Crt);
	}
	~CSync()
	{
		LeaveCriticalSection(m_Crt);
	}
};