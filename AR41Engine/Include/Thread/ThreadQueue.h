#pragma once
#include "../EngineInfo.h"
#include "Sync.h"

class CThreadQueue
{
private:
	ThreadSyncData	m_Data[201];
	CRITICAL_SECTION	m_Crt;
	int		m_Head;
	int		m_Tail;
	int		m_Size;
public:
	CThreadQueue();
	~CThreadQueue();
	void push(int header, int size, unsigned char* data)
	{
		if (full())
		{
			return;
		}
		CSync	sync(&m_Crt);
		m_Tail = (m_Tail + 1) % 201;
		m_Data[m_Tail].header = header;
		m_Data[m_Tail].size = size;
		memcpy(m_Data[m_Tail].data, data, size);
		++m_Size;
	}
	void pop(int* header, int* size, unsigned char* data)
	{
		if (empty())
		{
			return;
		}
		CSync	sync(&m_Crt);
		m_Head = (m_Head + 1) % 201;
		*header = m_Data[m_Head].header;
		*size = m_Data[m_Head].size;
		memcpy(data, m_Data[m_Head].data, m_Data[m_Head].size);
		--m_Size;
	}
	int size()
	{
		CSync	sync(&m_Crt);
		return m_Size;
	}
	bool full()
	{
		CSync	sync(&m_Crt);
		return m_Size == 200;
	}
	bool empty()
	{
		CSync	sync(&m_Crt);
		return m_Size == 0;
	}
};

