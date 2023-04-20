#pragma once
#include "../../EngineInfo.h"
#include "fbxsdk.h"

#ifdef _DEBUG
#pragma comment(lib, "../Bin/libfbxsdk-md_Debug.lib")
#else
#pragma comment(lib, "../Bin/libfbxsdk-md.lib")
#endif // _DEBUG

struct FbxBone
{
	std::string     Name;
	int             Depth;
	int             ParentIndex;
	FbxAMatrix       matOffset;
	FbxAMatrix       matBone;
	FbxBone()
		: Depth(0)
		, ParentIndex(0)
	{
	}
};

struct FbxKeyFrame
{
	FbxAMatrix	matTransform;//본행렬을 사용해서 프레임사이의 움직임을 구현
	double		Time;
};

struct FbxBoneKeyFrame
{
	int			BoneIndex;
	std::vector<FbxKeyFrame>	vecKeyFrame;
	FbxBoneKeyFrame()
		: BoneIndex(0)
	{
	}
};
//2d로 치면 프레임 정보
struct FbxAnimationClip
{
	std::string		Name;
	FbxTime			StartTime;
	FbxTime			EndTime;
	FbxLongLong		TimeLength;
	FbxTime::EMode	TimeMode;
	std::vector<FbxBoneKeyFrame>	vecBoneKeyFrame;
	FbxAnimationClip()
		: TimeLength(0)
		, TimeMode(FbxTime::EMode::eDefaultMode)
	{
	}
};

struct FbxWeight
{
	int		Index;
	double	Weight;
};

struct FbxMaterial
{
	Vector4	BaseColor;	//난반사(diffuse)
	Vector4	AmbientColor;	//환경광(암부) 어두워지는 정도
	Vector4	SpecularColor;	//정반사
	Vector4	EmissiveColor;	//hdr필수(블루필터링) 빛을 발산할때 사용
	float	SpecularPower;	//정반사되는 빛의 강도
	float	TransparencyFactor;
	float	Shininess;
	std::string	DiffuseTexture;
	std::string	BumpTexture;	//	덤프맵밍=노말맵핑 음영을 이용해 텍스쳐의 높낮이가 있는것처럼 보이는 기법
	std::string	SpecularTexture;
	std::string	Name;
	FbxMaterial()
		: SpecularPower(0.f)
		, TransparencyFactor(0.f)
		, Shininess(0.f)
	{
	}
};

struct FbxMeshContainer
{
	std::vector<Vector3>	vecPos;
	std::vector<Vector3>	vecNormal;	//	노말벡터(법선벡터) 면이 바라보는 방향으로 나온 법선의 방향벡터
	std::vector<Vector2>	vecUV;
	std::vector<Vector3>	vecTangent;
	std::vector<Vector3>	vecBinormal;
	std::vector<Vector4>	vecBlendWeight;
	std::vector<Vector4>	vecBlendIndex;
	//std::vector<Vector4>	vecColor;
	std::vector<std::vector<unsigned int>>	vecIndex;
	std::unordered_map<int, std::vector<FbxWeight>>	mapWeights;
	bool	Bump;
	bool	Animation;
	FbxMeshContainer() 
		: Bump(false)
		, Animation(false)
	{
	}
};


class CFBXLoader
{
private:
	friend class CMesh;
	friend class CStaticMesh;
	friend class CAnimationMesh;
	friend class CAnimationSequence;
	CFBXLoader();
	~CFBXLoader();
	FbxManager* m_Manager;
	FbxScene* m_Scene;
	FbxArray<FbxString*>			m_NameArray;
	bool							m_Mixamo;
	std::vector<FbxMeshContainer*>	m_vecMeshContainer;
	std::vector<std::vector<FbxMaterial*>>	m_vecMaterial;
	std::vector<FbxBone*>			m_vecBone;
	std::vector<FbxAnimationClip*>	m_vecClip;
	void Triangulate(FbxNode* node);
	void LoadMaterial(FbxSurfaceMaterial* mtrl);
	Vector4 GetMaterialColor(FbxSurfaceMaterial* mtrl, const char* propertyName, const char* propertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial* mtrl, const char* propertyName);
	std::string GetMaterialTexture(FbxSurfaceMaterial* mtrl, const char* propertyName);
	void LoadMesh(FbxNode* node, bool isStatic);
//3dmax는 가로세로 xy에 수직선이 z고 dx는 가로세로 xz에 수직선이 y이다 
	void LoadMesh(FbxMesh* mesh, bool isStatic);
	void LoadNormal(FbxMesh* mesh, FbxMeshContainer* container, int vtxID, int controlIndex);
	void LoadUV(FbxMesh* mesh, FbxMeshContainer* container, int UVID, int controlIndex);
	void LoadTangent(FbxMesh* mesh, FbxMeshContainer* container, int vtxID, int controlIndex);
	void LoadBinormal(FbxMesh* mesh, FbxMeshContainer* container, int vtxID, int controlIndex);
	void LoadColor(FbxMesh* mesh, FbxMeshContainer* container, int vtxID, int controlIndex);
	void LoadAnimationClip();
	void LoadBone(FbxNode* node);
	void LoadBoneRecursive(FbxNode* node, int depth, int index, int parent);
	void LoadAnimation(FbxMesh* mesh, FbxMeshContainer* container);
	FbxAMatrix GetTransform(FbxNode* node);
	int FindBoneFromName(const std::string& name);
	void LoadWeightAndIndex(FbxCluster* cluster, int boneIndex, FbxMeshContainer* container);
	void LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matTransform, int boneIndex, FbxMeshContainer* container);
	void LoadTimeTransform(FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matTransform, int boneIndex);
	void ChangeWeightAndIndices(FbxMeshContainer* container);
public:
	const std::vector<FbxMeshContainer*>* GetContainer()	const
	{
		return &m_vecMeshContainer;
	}
	const std::vector<std::vector<FbxMaterial*>>* GetMaterials()	const
	{
		return &m_vecMaterial;
	}
	const std::vector<FbxBone*>* GetBones()	const
	{
		return &m_vecBone;
	}
	const std::vector<FbxAnimationClip*>* GetClips()	const
	{
		return &m_vecClip;
	}
	bool LoadFBX(const char* fullPath, bool isStatic = true);
};

