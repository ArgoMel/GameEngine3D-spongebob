#pragma once
#include "EngineInfo.h"

class CRef
{
protected:
	std::string	m_Name;
	std::string	m_TypeName;
	size_t	m_TypeID;
	int		m_RefCount;
	bool	m_Enable;	// 활성, 비활성
	bool	m_Active;	// 살아 있는지 죽었는지
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
		m_TypeName = typeid(T).name();			// 타입 이름을 문자열로 얻어온다.
		m_TypeID = typeid(T).hash_code();		// 타입의 고유한 번호를 얻어온다.
	}
};

