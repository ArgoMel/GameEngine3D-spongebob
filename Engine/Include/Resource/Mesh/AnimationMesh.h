#pragma once
#include "Mesh.h"

class CAnimationMesh : public CMesh
{
private:
	friend class CMeshManager;
	class CStructuredBuffer* m_BoneBuffer;
	CSharedPtr<class CSkeleton>	m_Skeleton;
	int			m_InstancingCount;
protected:
	CAnimationMesh();
	virtual ~CAnimationMesh();
	virtual bool ConvertFBX(class CFBXLoader* loader, const char* fullPath);
	virtual bool SaveMesh(FILE* file);
	virtual bool LoadMesh(FILE* file);
public:
	class CStructuredBuffer* GetBoneBuffer()	const
	{
		return m_BoneBuffer;
	}
	class CSkeleton* GetSkeleton()	const
	{
		return m_Skeleton;
	}
	int GetBoneCount()	const;
	void SetBoneShader();
	void ResetBoneShader();
	void SetSkeleton(class CSkeleton* skeleton);
	void SetSkeleton(const std::string& name, const TCHAR* fileName, const std::string& pathName = MESH_PATH);
	void SetSkeletonFullPath(const std::string& name, const TCHAR* fullPath);
	void SetSkeletonMultibyte(const std::string& name, const char* fileName, const std::string& pathName = MESH_PATH);
	void SetSkeletonMultibyteFullPath(const std::string& name, const char* fullPath);
	void ResizeBoneBuffer(int count);
	virtual bool LoadMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName = MESH_PATH);
	virtual bool LoadMeshFullPath(const std::string& name, const TCHAR* fullPath);
	virtual bool LoadMeshMultibyte(const std::string& name, const char* fileName, const std::string& pathName = MESH_PATH);
	virtual bool LoadMeshMultibyteFullPath(const std::string& name, const char* fullPath);
};

