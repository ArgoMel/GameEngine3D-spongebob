#include "AnimationMesh.h"
#include "FBXLoader.h"
#include "../Animation/Skeleton.h"
#include "../Animation/AnimationSequence.h"
#include "../Shader/StructuredBuffer.h"
#include "../../PathManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CAnimationMesh::CAnimationMesh()
    : m_BoneBuffer(nullptr)
    , m_InstancingCount(100)
{
    m_MeshType = MeshType::Animation;
    SetTypeID<CAnimationMesh>();
}

CAnimationMesh::~CAnimationMesh()
{
	SAFE_DELETE(m_BoneBuffer)
}

int CAnimationMesh::GetBoneCount() const
{
	return (int)m_Skeleton->GetBoneCount();
}

void CAnimationMesh::SetBoneShader()
{
	m_BoneBuffer->SetShader(12, (int)EShaderBufferType::Vertex);
}

void CAnimationMesh::ResetBoneShader()
{
	m_BoneBuffer->ResetShader(12, (int)EShaderBufferType::Vertex);
}

void CAnimationMesh::SetSkeleton(CSkeleton* skeleton)
{
	m_Skeleton = skeleton;
	if (!m_BoneBuffer)
	{
		m_BoneBuffer = new CStructuredBuffer;
	}
	m_BoneBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount() * m_InstancingCount, 2);
}

void CAnimationMesh::SetSkeleton(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	TCHAR   fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	if (info)
	{
		lstrcpy(fullPath, info->path);
	}
	lstrcat(fullPath, fileName);
	return SetSkeletonFullPath(name, fullPath);
}

void CAnimationMesh::SetSkeletonFullPath(const std::string& name, const TCHAR* fullPath)
{
	char   _FullPath[MAX_PATH] = {};
	int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, _FullPath,	length, nullptr, nullptr);
	return SetSkeletonMultibyteFullPath(name, _FullPath);
}

void CAnimationMesh::SetSkeletonMultibyte(const std::string& name, const char* fileName, const std::string& pathName)
{
	char   fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	if (info)
	{
		strcpy_s(fullPath, info->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return SetSkeletonMultibyteFullPath(name, fullPath);
}

void CAnimationMesh::SetSkeletonMultibyteFullPath(const std::string& name, const char* fullPath)
{
	m_Skeleton = new CSkeleton;
	m_Skeleton->LoadSkeletonFullPath(m_Scene, name, fullPath);
	if (!m_BoneBuffer)
	{
		m_BoneBuffer = new CStructuredBuffer;
	}
	m_BoneBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount() * m_InstancingCount, 2);
}

void CAnimationMesh::ResizeBoneBuffer(int count)
{
	m_InstancingCount = count;
	m_BoneBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount() * m_InstancingCount, 2);
}

bool CAnimationMesh::LoadMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName)
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

bool CAnimationMesh::LoadMeshFullPath(const std::string& name, const TCHAR* fullPath)
{
    char   _FullPath[MAX_PATH] = {};
    int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, fullPath, -1, _FullPath, length, nullptr, nullptr);
    return LoadMeshMultibyteFullPath(name, _FullPath);
}

bool CAnimationMesh::LoadMeshMultibyte(const std::string& name, const char* fileName, const std::string& pathName)
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

bool CAnimationMesh::LoadMeshMultibyteFullPath(const std::string& name, const char* fullPath)
{
    char    ext[_MAX_EXT] = {};    // FBX 파일인지를 판단한다.
    _splitpath_s(fullPath, 0, 0, 0, 0, 0, 0, ext, _MAX_EXT);
    _strupr_s(ext);
    if (strcmp(ext, ".FBX") == 0)
    {
        CFBXLoader  loader;
        if (!loader.LoadFBX(fullPath, false))
        {
            return false;
        }
        return ConvertFBX(&loader, fullPath);
    }
    return LoadMeshFile(fullPath);
}

bool CAnimationMesh::ConvertFBX(CFBXLoader* loader, const char* fullPath)
{
	if (!CMesh::ConvertFBX(loader, fullPath))
	{
		return false;
	}
	const std::vector<FbxBone*>* vecBone = loader->GetBones();	// 애니메이션 처리
	if (vecBone->empty())
	{
		return true;
	}
	std::string	skeletonName = m_Name + "_Skeleton";
	m_Skeleton = new CSkeleton;
	// 본 수만큼 반복한다.
	std::vector<FbxBone*>::const_iterator	iterB;
	std::vector<FbxBone*>::const_iterator	iterBEnd = vecBone->end();
	for (iterB = vecBone->begin(); iterB != iterBEnd; ++iterB)
	{
		Bone* bone = new Bone;
		bone->name = (*iterB)->Name;
		bone->depth = (*iterB)->Depth;
		bone->parentIndex = (*iterB)->ParentIndex;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				bone->matOffset[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
			}
		}
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				bone->matBone[i][j] = (float)(*iterB)->matBone.mData[i].mData[j];
			}
		}
		m_Skeleton->AddBone(bone);
	}
	// 애니메이션 클립을 추가한다.
	const std::vector<FbxAnimationClip*>* vecClip = loader->GetClips();
	// 파일 이름을 제외한 경로를 만들어둔다.
	char	animPath[MAX_PATH] = {};
	strcpy_s(animPath, fullPath);
	int	length = (int)strlen(animPath);
	for (int i = length - 1; i >= 0; --i)
	{
		if (animPath[i] == '/' || animPath[i] == '\\')
		{
			size_t forOverflow= sizeof(TCHAR) * (length - (i + 1));
			memset(&animPath[i + 1], 0, forOverflow);
			break;
		}
	}
	// 클립을 읽어온다.
	std::vector<FbxAnimationClip*>::const_iterator	iterC;
	std::vector<FbxAnimationClip*>::const_iterator	iterCEnd = vecClip->end();
	for (iterC = vecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		if (m_Scene)
		{
			m_Scene->GetResource()->LoadAnimationSequence((*iterC)->Name, false, *iterC);
		}
		else
		{
			CResourceManager::GetInst()->LoadAnimationSequence((*iterC)->Name, false, *iterC);
		}
		CAnimationSequence* sequence = nullptr;
		if (m_Scene)
		{
			sequence = m_Scene->GetResource()->FindAnimationSequence((*iterC)->Name);
		}
		else
		{
			sequence = CResourceManager::GetInst()->FindAnimationSequence((*iterC)->Name);
		}
		if (!sequence)
		{
			continue;
		}
		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, fullPath);
		memcpy(&strAnimFullPath[length - 3], "sqc", 3);
		sequence->SaveFullPathMultibyte(strAnimFullPath);
	}
	if (m_Skeleton)
	{
		char	skeletonPath[MAX_PATH] = {};
		strcpy_s(skeletonPath, fullPath);
		memcpy(&skeletonPath[length - 3], "bne", 3);
		m_Skeleton->SaveSkeletonFullPath(skeletonPath);
		if (!m_BoneBuffer)
		{
			m_BoneBuffer = new CStructuredBuffer;
		}
		m_BoneBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount() * m_InstancingCount, 2);
	}
	return true;
}

bool CAnimationMesh::SaveMesh(FILE* file)
{
	if (!CMesh::SaveMesh(file))
	{
		return false;
	}
	return true;
}

bool CAnimationMesh::LoadMesh(FILE* file)
{
	if (!CMesh::LoadMesh(file))
	{
		return false;
	}
	return true;
}