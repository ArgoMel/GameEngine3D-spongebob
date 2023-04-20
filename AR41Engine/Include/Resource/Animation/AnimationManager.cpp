#include "AnimationManager.h"
#include "../Shader/Animation2DConstantBuffer.h"

CAnimationManager::CAnimationManager()
{
	m_Anim2DBuffer = new CAnimation2DConstantBuffer;
}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(m_Anim2DBuffer);
}

bool CAnimationManager::Init()
{
	m_Anim2DBuffer->Init();
	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
	const TCHAR* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->Init(textureName, fileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->Init(texture))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
	const TCHAR* fullPath)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->InitFullPath(textureName, fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->Init(textureName, vecFileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->InitFullPath(textureName, vecFullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::AddSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& end)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	sequence->AddFrame(start, end);
	return true;
}

bool CAnimationManager::AddSequence2DFrame(const std::string& name, float startX, float startY, float endX, float endY)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	sequence->AddFrame(startX, startY, endX, endY);
	return true;
}

bool CAnimationManager::AddSequence2DFrameAll(const std::string& name, int count, const Vector2& start, const Vector2& end)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	sequence->AddFrameAll(count, start, end);
	return true;
}

bool CAnimationManager::AddSequence2DFrameAll(const std::string& name, int count, float startX, float startY, float endX,
	float endY)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	sequence->AddFrameAll(count, startX, startY, endX, endY);
	return true;
}

bool CAnimationManager::SaveSequence2D(const std::string& name, const char* fullPath)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	return sequence->Save(fullPath);
}

bool CAnimationManager::LoadSequence2D(const std::string& name, const char* fullPath)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	bool	find = true;
	if (!sequence)
	{
		sequence = new CAnimationSequence2D;
		sequence->SetName(name);
		find = false;
	}
	if (!sequence->Load(fullPath))
	{
		return false;
	}
	if (!find)
	{
		m_mapSequence2D.insert(std::make_pair(name, sequence));
	}
	return true;
}

bool CAnimationManager::SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	return sequence->Save(fileName, pathName);
}

bool CAnimationManager::LoadSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	bool	find = true;
	if (!sequence)
	{
		sequence = new CAnimationSequence2D;
		sequence->SetName(name);
		find = false;
	}
	if (!sequence->Load(fileName, pathName))
	{
		return false;
	}
	if (!find)
	{
		m_mapSequence2D.insert(std::make_pair(name, sequence));
	}
	return true;
}

CAnimationSequence2D* CAnimationManager::FindAnimationSequence2D(const std::string& name)
{
	auto	iter = m_mapSequence2D.find(name);
	if (iter == m_mapSequence2D.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CAnimationManager::ReleaseAnimationSequence2D(const std::string& name)
{
	auto	iter = m_mapSequence2D.find(name);
	if (iter != m_mapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapSequence2D.erase(iter);
		}
	}
}

bool CAnimationManager::LoadAnimationSequence(const std::string& name, bool loop, const FbxAnimationClip* clip)
{
	CAnimationSequence* sequence = FindAnimationSequence(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence;
	sequence->SetName(name);
	if (!sequence->CreateSequence(loop, clip))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::LoadAnimationSequence(const std::string& name, bool loop, int startFrame, int endFrame, 
	float playTime, const std::vector<BoneKeyFrame*>& vecFrame)
{
	CAnimationSequence* sequence = FindAnimationSequence(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence;
	sequence->SetName(name);
	if (!sequence->CreateSequence(name, loop, startFrame, endFrame, playTime, vecFrame))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::LoadAnimationSequence(const std::string& name, const TCHAR* fileName, 
	const std::string& pathName)
{
	CAnimationSequence* sequence = FindAnimationSequence(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence;
	sequence->SetName(name);
	if (!sequence->CreateSequence(name, fileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::LoadAnimationSequenceFullPath(const std::string& name, const TCHAR* fullPath)
{
	CAnimationSequence* sequence = FindAnimationSequence(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence;
	sequence->SetName(name);
	if (!sequence->CreateSequenceFullPath(name, fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::LoadAnimationSequenceMultibyte(const std::string& name, const char* fileName, 
	const std::string& pathName)
{
	CAnimationSequence* sequence = FindAnimationSequence(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence;
	sequence->SetName(name);
	if (!sequence->CreateSequenceMultibyte(name, fileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::LoadAnimationSequenceMultibyteFullPath(const std::string& name, const char* fullPath)
{
	CAnimationSequence* sequence = FindAnimationSequence(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence;
	sequence->SetName(name);
	if (!sequence->CreateSequenceFullPathMultibyte(name, fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence.insert(std::make_pair(name, sequence));
	return true;
}

CAnimationSequence* CAnimationManager::FindAnimationSequence(const std::string& name)
{
	auto	iter = m_mapSequence.find(name);
	if (iter == m_mapSequence.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CAnimationManager::ReleaseAnimationSequence(const std::string& name)
{
	auto	iter = m_mapSequence.find(name);
	if (iter != m_mapSequence.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapSequence.erase(iter);
		}
	}
}

bool CAnimationManager::LoadSkeleton(CScene* scene, const std::string& name, const TCHAR* fileName, 
	const std::string& pathName)
{
	CSkeleton* skeleton = FindSkeleton(name);
	if (skeleton)
	{
		return true;
	}
	skeleton = new CSkeleton;
	skeleton->SetName(name);
	char    fileNameMultibyte[MAX_PATH] = {};
	int length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameMultibyte, length, 0, 0);
	if (!skeleton->LoadSkeleton(scene, name, fileNameMultibyte, pathName))
	{
		SAFE_DELETE(skeleton);
		return false;
	}
	m_mapSkeleton.insert(std::make_pair(name, skeleton));
	return true;
}

bool CAnimationManager::LoadSkeletonFullPath(CScene* scene, const std::string& name, const TCHAR* fullPath)
{
	CSkeleton* skeleton = FindSkeleton(name);
	if (skeleton)
	{
		return true;
	}
	skeleton = new CSkeleton;
	skeleton->SetName(name);
	char    fullPathMultibyte[MAX_PATH] = {};
	int length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultibyte, length, 0, 0);
	if (!skeleton->LoadSkeletonFullPath(scene, name, fullPathMultibyte))
	{
		SAFE_DELETE(skeleton);
		return false;
	}
	m_mapSkeleton.insert(std::make_pair(name, skeleton));
	return true;
}

bool CAnimationManager::LoadSkeletonMultibyte(CScene* scene, const std::string& name, const char* fileName,
	const std::string& pathName)
{
	CSkeleton* skeleton = FindSkeleton(name);
	if (skeleton)
	{
		return true;
	}
	skeleton = new CSkeleton;
	skeleton->SetName(name);
	if (!skeleton->LoadSkeleton(scene, name, fileName, pathName))
	{
		SAFE_DELETE(skeleton);
		return false;
	}
	m_mapSkeleton.insert(std::make_pair(name, skeleton));
	return true;
}

bool CAnimationManager::LoadSkeletonMultibyteFullPath(CScene* scene, const std::string& name, const char* fullPath)
{
	CSkeleton* skeleton = FindSkeleton(name);
	if (skeleton)
	{
		return true;
	}
	skeleton = new CSkeleton;
	skeleton->SetName(name);
	if (!skeleton->LoadSkeletonFullPath(scene, name, fullPath))
	{
		SAFE_DELETE(skeleton);
		return false;
	}
	m_mapSkeleton.insert(std::make_pair(name, skeleton));
	return true;
}

void CAnimationManager::AddSocket(const std::string& skeletonName, const std::string& boneName, 
	const std::string& socketName, const Vector3& offset, const Vector3& offsetRot)
{
	CSkeleton* skeleton = FindSkeleton(skeletonName);
	if (!skeleton)
	{
		return;
	}
	skeleton->AddSocket(boneName, socketName, offset, offsetRot);
}

CSkeleton* CAnimationManager::FindSkeleton(const std::string& name)
{
	auto	iter = m_mapSkeleton.find(name);
	if (iter == m_mapSkeleton.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CAnimationManager::ReleaseSkeleton(const std::string& name)
{
	auto	iter = m_mapSkeleton.find(name);
	if (iter != m_mapSkeleton.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapSkeleton.erase(iter);
		}
	}
}