#pragma once
#include "../EngineInfo.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Sound/Sound.h"
#include "../Resource/Font/Font.h"
#include "../Resource/Font/FontCollection.h"
#include "../Resource/Particle/Particle.h"

class CSceneResource
{
private:
	friend class CScene;
	class CScene* m_Owner;
	std::unordered_map<std::string, CSharedPtr<CMesh>>		m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CShader>>	m_mapShader;
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CMaterial>>	m_mapMaterial;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapAnimationSequence2D;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>>	m_mapAnimationSequence;
	std::unordered_map<std::string, CSharedPtr<CSkeleton>>	m_mapSkeleton;
	std::unordered_map<std::string, CSharedPtr<CSound>>	m_mapSound;
	std::unordered_map<std::string, CSharedPtr<CFont>>	m_mapFont;
	std::unordered_map<std::string, CSharedPtr<CFontCollection>>	m_mapFontCollection;
	std::unordered_map<std::string, CSharedPtr<CParticle>>	m_mapParticle;
	CSceneResource();
	~CSceneResource();
public:
	bool Init();
// ===================== Mesh =========================
	bool CreateMesh(MeshType type, const std::string& name, void* vtxData, int size, int count,
		D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData = nullptr, int idxSize = 0, 
		int idxCount = 0, D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT,	DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	bool LoadMesh(MeshType type, const std::string& name, const TCHAR* fileName, const std::string& pathName = MESH_PATH);
	bool LoadMeshFullPath(MeshType type, const std::string& name, const TCHAR* fullPath);
	bool LoadMeshMultibyte(MeshType type, const std::string& name, const char* fileName, 
		const std::string& pathName = MESH_PATH);
	bool LoadMeshMultibyteFullPath(MeshType type, const std::string& name, const char* fullPath);
	class CMesh* FindMesh(const std::string& name);
// ===================== Shader =========================
	class CShader* FindShader(const std::string& name);
// ===================== Texture =========================
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
	bool CreateTarget(const std::string& name, unsigned int width, unsigned int height, 
		DXGI_FORMAT pixelFormat, DXGI_FORMAT depthFormat = DXGI_FORMAT_UNKNOWN);
	void RenderTexture();
	class CTexture* FindTexture(const std::string& name);
// ===================== Material =========================
	CMaterial* FindMaterial(const std::string& name);
	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		if (FindMaterial(name))
		{
			return true;
		}
		if (!CResourceManager::GetInst()->CreateMaterial<T>(name))
		{
			return false;
		}
		m_mapMaterial.insert(std::make_pair(name, CResourceManager::GetInst()->FindMaterial(name)));
		return true;
	}
// ===================== Animation =========================
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, const TCHAR* fullPath);
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
		const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
		const std::vector<const TCHAR*>& vecFullPath);
	bool AddAnimationSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& end);
	bool AddAnimationSequence2DFrame(const std::string& name, float startX, float startY, float endX, float endY);
	bool AddAnimationSequence2DFrameAll(const std::string& name, int count, const Vector2& start, const Vector2& end);
	bool AddAnimationSequence2DFrameAll(const std::string& name, int count, float startX, float startY,
		float endX, float endY);
	bool SaveSequence2D(const std::string& name, const char* fullPath);
	bool LoadSequence2D(const std::string& name, const char* fullPath);
	bool SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName);
	bool LoadSequence2D(const std::string& name, const char* fileName, const std::string& pathName);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& name);
// Animation3D
	bool LoadAnimationSequence(const std::string& name, bool loop, const FbxAnimationClip* clip);
	bool LoadAnimationSequence(const std::string& name, bool loop, int startFrame, int endFrame, float playTime,
		const std::vector<BoneKeyFrame*>& vecFrame);
	bool LoadAnimationSequence(const std::string& name, const TCHAR* fileName, 
		const std::string& pathName = ANIMATION3D_PATH);
	bool LoadAnimationSequenceFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadAnimationSequenceMultibyte(const std::string& name,const char* fileName, 
		const std::string& pathName = ANIMATION3D_PATH);
	bool LoadAnimationSequenceMultibyteFullPath(const std::string& name, const char* fullPath);
	CAnimationSequence* FindAnimationSequence(const std::string& name);
	bool LoadSkeleton(const std::string& name, const TCHAR* fileName, 
		const std::string& pathName = ANIMATION3D_PATH);
	bool LoadSkeletonFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadSkeletonMultibyte(const std::string& name, const char* fileName, 
		const std::string& pathName = ANIMATION3D_PATH);
	bool LoadSkeletonMultibyteFullPath(const std::string& name, const char* fullPath);
	void AddSocket(const std::string& skeletonName, const std::string& boneName, const std::string& socketName,
		const Vector3& offset = Vector3(0, 0, 0), const Vector3& offsetRot = Vector3(0, 0, 0));
	bool SetMeshSkeleton(const std::string& meshName, const std::string& skeletonName);
	CSkeleton* FindSkeleton(const std::string& name);
// ============================ Sound ================================
	bool CreateSoundChannel(const std::string& name);
	bool LoadSound(const std::string& groupName, const std::string& name,
		bool loop, const char* fileName, const std::string& pathName = SOUND_PATH);
	bool SetVolume(float volume);
	bool SetVolume(const std::string& groupName, float volume);
	bool SoundPlay(const std::string& name);
	bool SoundStop(const std::string& name);
	bool SoundPause(const std::string& name);
	bool SoundResume(const std::string& name);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& name);
	class CSound* FindSound(const std::string& name);
// ============================ Font ================================
	bool CreateFontCollection(const std::string& name, const TCHAR* fileName, const std::string& pathName = FONT_PATH);
	bool LoadFont(const std::string& name, const TCHAR* fontName,
		int weight, float fontSize, const TCHAR* localName, int stretch = 5);
	const TCHAR* GetFontFaceName(const std::string& collectionName);
	const char* GetFontFaceNameMultibyte(const std::string& collectionName);
	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(const Vector4& color);
	bool CreateFontColor(unsigned int color);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int color);
	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(const Vector4& color);
	CFont* FindFont(const std::string& name);
	CFontCollection* FindFontCollection(const std::string& name);
// ============================ Particle ================================
	bool CreateParticle(const std::string& name);
	CParticle* FindParticle(const std::string& name);
};
