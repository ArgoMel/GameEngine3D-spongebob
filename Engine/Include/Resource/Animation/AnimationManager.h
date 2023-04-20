#pragma once
#include "AnimationSequence2D.h"
#include "AnimationSequence.h"
#include "Skeleton.h"
#include "../Mesh/FBXLoader.h"

class CAnimationManager
{
private:
	friend class CResourceManager;
	class CAnimation2DConstantBuffer* m_Anim2DBuffer;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapSequence2D;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>>	m_mapSequence;
	std::unordered_map<std::string, CSharedPtr<CSkeleton>>	m_mapSkeleton;
	CAnimationManager();
	~CAnimationManager();
public:
	class CAnimation2DConstantBuffer* GetAnim2DConstantBuffer()	const
	{
		return m_Anim2DBuffer;
	}
	bool Init();
//2D
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, const TCHAR* fullPath);
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
		const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
		const std::vector<const TCHAR*>& vecFullPath);
	bool AddSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& end);
	bool AddSequence2DFrame(const std::string& name, float startX, float startY, float endX, float endY);
	bool AddSequence2DFrameAll(const std::string& name, int count, const Vector2& start, const Vector2& end);
	bool AddSequence2DFrameAll(const std::string& name, int count, float startX, float startY, float endX, float endY);
	bool SaveSequence2D(const std::string& name, const char* fullPath);
	bool LoadSequence2D(const std::string& name, const char* fullPath);
	bool SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName);
	bool LoadSequence2D(const std::string& name, const char* fileName, const std::string& pathName);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& name);
	void ReleaseAnimationSequence2D(const std::string& name);
// Animation 3D
	bool LoadAnimationSequence(const std::string& name, bool loop, const FbxAnimationClip* clip);
	bool LoadAnimationSequence(const std::string& name, bool loop, int startFrame, int endFrame, float playTime,
		const std::vector<BoneKeyFrame*>& vecFrame);
	bool LoadAnimationSequence(const std::string& name, const TCHAR* fileName, 
		const std::string& pathName = ANIMATION3D_PATH);
	bool LoadAnimationSequenceFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadAnimationSequenceMultibyte(const std::string& name, const char* fileName, 
		const std::string& pathName = ANIMATION3D_PATH);
	bool LoadAnimationSequenceMultibyteFullPath(const std::string& name, const char* fullPath);
	CAnimationSequence* FindAnimationSequence(const std::string& name);
	void ReleaseAnimationSequence(const std::string& name);
	bool LoadSkeleton(class CScene* scene, const std::string& name,const TCHAR* fileName,
		const std::string& pathName = ANIMATION3D_PATH);
	bool LoadSkeletonFullPath(class CScene* scene, const std::string& name, const TCHAR* fullPath);
	bool LoadSkeletonMultibyte(class CScene* scene, const std::string& name, const char* fileName,
		const std::string& pathName = ANIMATION3D_PATH);
	bool LoadSkeletonMultibyteFullPath(class CScene* scene, const std::string& name, const char* fullPath);
	void AddSocket(const std::string& skeletonName,	const std::string& boneName, const std::string& socketName,
		const Vector3& offset = Vector3(0, 0, 0), const Vector3& offsetRot = Vector3(0, 0, 0));
	CSkeleton* FindSkeleton(const std::string& name);
	void ReleaseSkeleton(const std::string& name);
};

