#include "Ref.h"

CRef::CRef()
	: m_RefCount(0)
	, m_Enable(true)
	, m_Active(true)
	, m_TypeID(0)
{
}

CRef::CRef(const CRef& ref)
	: m_RefCount(0)
	, m_Name(ref.m_Name)
	, m_TypeName(ref.m_TypeName)
	, m_TypeID(ref.m_TypeID)
	, m_Enable(ref.m_Enable)
	, m_Active(ref.m_Active)
{
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
	++m_RefCount;
}

int CRef::Release()
{
	--m_RefCount;
	if (m_RefCount <= 0)
	{
		delete this;
		return 0;
	}
	return m_RefCount;
}

void CRef::Save(FILE* file)
{
	int	length = (int)m_Name.length();
	fwrite(&length, sizeof(int), 1, file);
	fwrite(m_Name.c_str(), sizeof(char), length, file);
	length = (int)m_TypeName.length();
	fwrite(&length, sizeof(int), 1, file);
	fwrite(m_TypeName.c_str(), sizeof(char), length, file);
	fwrite(&m_TypeID, sizeof(size_t), 1, file);
}

void CRef::Load(FILE* file)
{
	int	length = 0;
	char	text[256] = {};
	fread(&length, sizeof(int), 1, file);
	assert(length>=0);
	fread(text, sizeof(char), length, file);
	m_Name = text;
	memset(text, 0, 256);
	fread(&length, sizeof(int), 1, file);
	assert(length >= 0);
	fread(text, sizeof(char), length, file);
	m_TypeName = text;
	fread(&m_TypeID, sizeof(size_t), 1, file);
}
