#pragma once
#include "../../EngineInfo.h"

class CMeshManager
{
private:
	friend class CResourceManager;
	std::unordered_map<std::string, CSharedPtr<class CMesh>>	m_mapMesh;
	CMeshManager();
	~CMeshManager();
	bool CreateSphere(std::vector<Vertex3D>& vecVertex, std::vector<int>& vecIndex, float radius, unsigned int subDivision);
	void Subdivide(std::vector<Vertex3D>& vecVertices, std::vector<int>& vecIndices);
	float AngleFromXY(float x, float y);
public:
	bool Init();
	bool CreateMesh(class CScene* scene, MeshType type, const std::string& name,void* vtxData, int size, int count, 
		D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive,void* idxData = nullptr, int idxSize = 0, 
		int idxCount = 0, D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT,DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	bool LoadMesh(class CScene* scene, MeshType type, const std::string& name, const TCHAR* fileName, 
		const std::string& pathName = MESH_PATH);
	bool LoadMeshFullPath(class CScene* scene, MeshType type, const std::string& name, const TCHAR* fullPath);
	bool LoadMeshMultibyte(class CScene* scene, MeshType type, const std::string& name, const char* fileName, 
		const std::string& pathName = MESH_PATH);
	bool LoadMeshMultibyteFullPath(class CScene* scene, MeshType type, const std::string& name, const char* fullPath);
	class CMesh* FindMesh(const std::string& name);
	void ReleaseMesh(const std::string& name);
};

