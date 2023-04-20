#include "StaticMesh.h"
#include "FBXLoader.h"
#include "../../PathManager.h"

CStaticMesh::CStaticMesh()
{
    m_MeshType = MeshType::Static;
    SetTypeID<CStaticMesh>();
}

CStaticMesh::~CStaticMesh()
{
}

bool CStaticMesh::LoadMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    TCHAR   fullPath[MAX_PATH] = {};
    const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
    if (info)
    {
        lstrcpy(fullPath, info->path);
    }
    lstrcat(fullPath, fileName);
    return LoadMeshFullPath(name, fullPath);
}

bool CStaticMesh::LoadMeshFullPath(const std::string& name, const TCHAR* fullPath)
{
    char   _FullPath[MAX_PATH] = {};
    int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, fullPath, -1, _FullPath, length, nullptr, nullptr);
    return LoadMeshMultibyteFullPath(name, _FullPath);
}

bool CStaticMesh::LoadMeshMultibyte(const std::string& name, const char* fileName, const std::string& pathName)
{
    char   fullPath[MAX_PATH] = {};
    const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
    if (info)
    {
        strcpy_s(fullPath, info->pathMultibyte);
    }
    strcat_s(fullPath, fileName);
    return LoadMeshMultibyteFullPath(name, fullPath);
}

bool CStaticMesh::LoadMeshMultibyteFullPath(const std::string& name, const char* fullPath)
{
    char    ext[_MAX_EXT] = {};    // FBX 파일인지를 판단한다.
    _splitpath_s(fullPath, 0, 0, 0, 0, 0, 0, ext, _MAX_EXT);
    _strupr_s(ext);
    if (strcmp(ext, ".FBX") == 0)
    {
        CFBXLoader  loader;
        if (!loader.LoadFBX(fullPath))
        {
            return false;
        }
        return ConvertFBX(&loader, fullPath);
    }
    return LoadMeshFile(fullPath);
}
