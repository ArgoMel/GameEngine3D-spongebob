#pragma once
#include "EngineInfo.h"

struct PathInfo
{
	TCHAR	path[MAX_PATH];
	char	pathMultibyte[MAX_PATH];

	PathInfo() 
		: path{}
		, pathMultibyte{}
	{
	}
};

class CPathManager
{
	DECLARE_SINGLE(CPathManager)
private:
	std::unordered_map<std::string, PathInfo*>	m_mapPath;
public:
	bool Init();
	bool AddPath(const std::string& name, const TCHAR* path,const std::string& basePathName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& name);
};
