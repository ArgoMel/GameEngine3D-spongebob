#pragma once
#include "../EngineInfo.h"

class CDataStream
{
private:
	unsigned char* m_Buffer;
	int			m_Size;
public:
	CDataStream();
	~CDataStream();
	int GetSize()	const
	{
		return m_Size;
	}
	void SetBuffer(unsigned char* buffer)
	{
		m_Buffer = buffer;
	}
	template <typename T>
	void AddData(const T* data, int size)
	{
		memcpy(m_Buffer + m_Size, data, size);
		m_Size += size;
	}
	template <typename T>
	void GetData(T* data, int size)
	{
		memcpy(data, m_Buffer + m_Size, size);
		m_Size += size;
	}
};

