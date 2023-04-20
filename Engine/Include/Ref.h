#pragma once
#include "EngineInfo.h"

class CRef
{
protected:
	std::string	m_Name;
	std::string	m_TypeName;
	size_t	m_TypeID;
	int		m_RefCount;
	bool	m_Enable;	// Ȱ��, ��Ȱ��
	bool	m_Active;	// ��� �ִ��� �׾�����
public:
	CRef();
	CRef(const CRef& ref);
	const std::string& GetName()	const
	{
		return m_Name;
	}
	const std::string& GetTypeName()	const
	{
		return m_TypeName;
	}
	size_t GetTypeID()	const
	{
		return m_TypeID;
	}
	int GetRefCount()	const
	{
		return m_RefCount;
	}
	bool GetEnable()	const
	{
		return m_Enable;
	}
	bool GetActive()	const
	{
		return m_Active;
	}
	void SetName(const std::string& name)
	{
		m_Name = name;
	}
	void SetEnable(bool enable)
	{
		m_Enable = enable;
	}
	virtual void Destroy()
	{
		m_Active = false;
	}
	virtual ~CRef();
	void AddRef();
	int Release();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	template <typename T>
	bool CheckTypeID()	const
	{
		return m_TypeID == typeid(T).hash_code();
	}
	template <typename T>
	void SetTypeID()
	{
		m_TypeName = typeid(T).name();			// Ÿ�� �̸��� ���ڿ��� ���´�.
		m_TypeID = typeid(T).hash_code();		// Ÿ���� ������ ��ȣ�� ���´�.
	}
};

