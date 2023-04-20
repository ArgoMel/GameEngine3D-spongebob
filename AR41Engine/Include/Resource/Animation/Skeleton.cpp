#include "Skeleton.h"
#include "SkeletonSocket.h"
#include "../../Device.h"
#include "../../PathManager.h"
#include "../Shader/StructuredBuffer.h"

CSkeleton::CSkeleton()
	: m_pOffsetMatrixBuffer(nullptr)
	, m_Scene(nullptr)
{
	SetTypeID<CSkeleton>();
}

CSkeleton::CSkeleton(const CSkeleton& skeleton)
	: CRef(skeleton)
	, m_Scene(nullptr)
{
	for (size_t i = 0; i < skeleton.m_vecBones.size(); ++i)
	{
		Bone* bone = new Bone;
		*bone = *skeleton.m_vecBones[i];
		m_vecBones.push_back(bone);
	}
	m_pOffsetMatrixBuffer = nullptr;
	m_BoneSocket.clear();
	size_t	size = skeleton.m_BoneSocket.size();
	for (size_t i = 0; i < size; ++i)
	{
		CSkeletonSocket* socket = skeleton.m_BoneSocket[i]->Clone();
		m_BoneSocket.push_back(socket);
	}
}

CSkeleton::~CSkeleton()
{
	SAFE_DELETE(m_pOffsetMatrixBuffer);
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		SAFE_DELETE(m_vecBones[i]);
	}
	m_vecBones.clear();
	size_t	size = m_BoneSocket.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_BoneSocket[i]);
	}
	m_BoneSocket.clear();
}

size_t CSkeleton::GetBoneCount() const
{
	return m_vecBones.size();
}

Bone* CSkeleton::GetBone(int index) const
{
	return m_vecBones[index];
}

Bone* CSkeleton::GetBone(const std::string& name) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->name == name)
		{
			return m_vecBones[i];
		}
	}
	return nullptr;
}

int CSkeleton::GetBoneIndex(const std::string& name) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->name == name)
		{
			return (int)i;
		}
	}
	return -1;
}

bool CSkeleton::CheckBone(const std::string& name) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->name == name)
		{
			return true;
		}
	}
	return false;
}

const Matrix& CSkeleton::GetBoneMatrix(const std::string& name) const
{
	int	index = GetBoneIndex(name);
	return m_vecBones[index]->matBone;
}

const Matrix& CSkeleton::GetBoneMatrix(int index) const
{
	return m_vecBones[index]->matBone;
}

CSkeletonSocket* CSkeleton::GetSocket(const std::string& name)
{
	size_t	size = m_BoneSocket.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_BoneSocket[i]->m_Name == name)
		{
			return m_BoneSocket[i];
		}
	}
	return nullptr;
}

void CSkeleton::AddBone(Bone* bone)
{
	m_vecBones.push_back(bone);
}

bool CSkeleton::SaveSkeleton(const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	char	fullPath[MAX_PATH] = {};
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return SaveSkeletonFullPath(fullPath);
}

bool CSkeleton::SaveSkeletonFullPath(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return false;
	}
	size_t	boneCount = m_vecBones.size();
	fwrite(&boneCount, sizeof(size_t), 1, file);
	for (size_t i = 0; i < boneCount; ++i)
	{
		size_t	nameCount = m_vecBones[i]->name.size();
		fwrite(&nameCount, sizeof(size_t), 1, file);
		fwrite(m_vecBones[i]->name.c_str(), 1, nameCount, file);
		fwrite(&m_vecBones[i]->depth, sizeof(int), 1, file);
		fwrite(&m_vecBones[i]->parentIndex, sizeof(int), 1, file);
		fwrite(&m_vecBones[i]->matOffset, sizeof(Matrix), 1, file);
		fwrite(&m_vecBones[i]->matBone, sizeof(Matrix), 1, file);
	}
	fclose(file);
	return true;
}

bool CSkeleton::LoadSkeleton(CScene* scene, const std::string& name, const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	char	fullPath[MAX_PATH] = {};
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return LoadSkeletonFullPath(scene, name, fullPath);
}

bool CSkeleton::LoadSkeletonFullPath(CScene* scene, const std::string& name, const char* fullPath)
{
	m_Scene = scene;
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		--m_vecBones[i]->refCount;
		if (m_vecBones[i]->refCount == 0)
		{
			SAFE_DELETE(m_vecBones[i]);
		}
	}
	m_vecBones.clear();
	size_t	boneCount = 0;
	fread(&boneCount, sizeof(size_t), 1, file);
	for (size_t i = 0; i < boneCount; ++i)
	{
		Bone* bone = new Bone;
		m_vecBones.push_back(bone);
		size_t	nameCount = 0;
		char	boneName[256] = {};
		fread(&nameCount, sizeof(size_t), 1, file);
		assert(nameCount >= 0);
		fread(boneName, 1, nameCount, file);
		bone->name = boneName;
		fread(&bone->depth, sizeof(int), 1, file);
		fread(&bone->parentIndex, sizeof(int), 1, file);
		fread(&bone->matOffset, sizeof(Matrix), 1, file);
		fread(&bone->matBone, sizeof(Matrix), 1, file);
	}
	fclose(file);
	return true;
}

void CSkeleton::SetShader()
{
	if (!m_pOffsetMatrixBuffer)
	{
		m_pOffsetMatrixBuffer = new CStructuredBuffer;
		std::vector<Matrix>	vecOffset;
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i]->matOffset);
		}
		m_pOffsetMatrixBuffer->Init("OffsetMatrixBuffer", (unsigned int)sizeof(Matrix),
			(unsigned int)vecOffset.size(), 16, true, (int)EShaderBufferType::Compute);

		m_pOffsetMatrixBuffer->UpdateBuffer(&vecOffset[0], (int)vecOffset.size());
	}
	m_pOffsetMatrixBuffer->SetShader();
}

void CSkeleton::ResetShader()
{
	m_pOffsetMatrixBuffer->ResetShader();
}

void CSkeleton::AddSocket(const std::string& boneName, const std::string& socketName, const Vector3& offset, 
	const Vector3& offsetRot)
{
	if (!CheckBone(boneName))
	{
		return;
	}
	CSkeletonSocket* socket = new CSkeletonSocket;
	socket->m_BoneName = boneName;
	socket->m_Name = socketName;
	socket->m_Offset = offset;
	socket->m_OffsetRotation = offsetRot;
	socket->m_BoneIndex = GetBoneIndex(boneName);
	m_BoneSocket.push_back(socket);
}

void CSkeleton::Update(float deltaTime, const std::vector<OutputBoneInfo>& vecBoneInfo, class CAnimationMeshComponent* owner)
{	
	size_t	size = m_BoneSocket.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_BoneSocket[i]->Update(vecBoneInfo[m_BoneSocket[i]->m_BoneIndex], owner);
	}
}

CSkeleton* CSkeleton::Clone()
{
	return new CSkeleton(*this);
}
