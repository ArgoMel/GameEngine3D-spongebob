#pragma once
#include "Mesh.h"

class CStaticMesh : public CMesh
{
	friend class CMeshManager;
protected:
	CStaticMesh();
	virtual ~CStaticMesh();
public:
	virtual bool LoadMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName = MESH_PATH);
	virtual bool LoadMeshFullPath(const std::string& name, const TCHAR* fullPath);
	virtual bool LoadMeshMultibyte(const std::string& name, const char* fileName, const std::string& pathName = MESH_PATH);
	virtual bool LoadMeshMultibyteFullPath(const std::string& name, const char* fullPath);
};

