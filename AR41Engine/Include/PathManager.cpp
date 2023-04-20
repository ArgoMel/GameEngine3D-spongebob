#include "PathManager.h"
DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	auto	iter = m_mapPath.begin();
	auto	iterEnd = m_mapPath.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	TCHAR	root[MAX_PATH] = {};
	GetModuleFileName(0, root, MAX_PATH);
	int	length = lstrlen(root)-1;
	for (size_t i = length; i > 0; --i)
	{
		if (root[i] == '/' || root[i] == '\\')
		{
			memset(&root[i + 1], 0, sizeof(TCHAR) * (length - i));
			break;
		}
	}
	PathInfo* info = new PathInfo;
	lstrcpy(info->path, root);
#ifdef UNICODE
	int	pathLength = WideCharToMultiByte(CP_ACP, 0, info->path, -1,0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, info->path, -1,info->pathMultibyte, pathLength, 0, 0);
#else
	strcpy_s(info->pathMultibyte, info->path);
#endif // UNICODE
	m_mapPath.insert(std::make_pair(ROOT_PATH, info));
	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(SHADER_PATH, TEXT("Shader\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(FONT_PATH, TEXT("Font\\"));
	AddPath(ANIMATION_PATH, TEXT("Animation\\"));
	AddPath(ANIMATION3D_PATH, TEXT("Animation3D\\"));
	AddPath(SCENE_PATH, TEXT("Scene\\"));
	AddPath(SAVE_PATH, TEXT("Save\\"));
	AddPath(MESH_PATH, TEXT("Mesh\\"));
	AddPath(DIALOG_PATH, TEXT("Dialog\\"));
	AddPath(CHATGPT_PATH, TEXT("ChatGPT\\"));
	AddPath(CUTSCENE_PATH, TEXT("Cutscene\\"));
	return true;
}

bool CPathManager::AddPath(const std::string& name, const TCHAR* path, const std::string& basePathName)
{
	if (FindPath(name))
	{
		return false;
	}
	const PathInfo* basePath = FindPath(basePathName);
	PathInfo* info = new PathInfo;
	if (basePath)
	{
		lstrcpy(info->path, basePath->path);
	}
	lstrcat(info->path, path);
#ifdef UNICODE
	int	pathLength = WideCharToMultiByte(CP_ACP, 0, info->path, -1,	0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, info->path, -1,info->pathMultibyte, pathLength, 0, 0);
#else
	strcpy_s(info->pathMultibyte, info->path);
#endif // UNICODE
	m_mapPath.insert(std::make_pair(name, info));
	return true;
}

const PathInfo* CPathManager::FindPath(const std::string& name)
{
	auto	iter = m_mapPath.find(name);
	if (iter == m_mapPath.end())
	{
		return nullptr;
	}
	return iter->second;
}
