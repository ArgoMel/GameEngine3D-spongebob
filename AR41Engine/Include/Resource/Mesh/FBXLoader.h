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
	FbxAMatrix	matTransform;//������� ����ؼ� �����ӻ����� �������� ����
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
//2d�� ġ�� ������ ����
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
	Vector4	BaseColor;	//���ݻ�(diffuse)
	Vector4	AmbientColor;	//ȯ�汤(�Ϻ�) ��ο����� ����
	Vector4	SpecularColor;	//���ݻ�
	Vector4	EmissiveColor;	//hdr�ʼ�(������͸�) ���� �߻��Ҷ� ���
	float	SpecularPower;	//���ݻ�Ǵ� ���� ����
	float	TransparencyFactor;
	float	Shininess;
	std::string	DiffuseTexture;
	std::string	BumpTexture;	//	�����ʹ�=�븻���� ������ �̿��� �ؽ����� �����̰� �ִ°�ó�� ���̴� ���
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
	std::vector<Vector3>	vecNormal;	//	�븻����(��������) ���� �ٶ󺸴� �������� ���� ������ ���⺤��
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
//3dmax�� ���μ��� xy�� �������� z�� dx�� ���μ��� xz�� �������� y�̴� 
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

