#pragma once
#include "../../Ref.h"

//컴퓨트쉐이더 사용(행렬연산)
struct Bone
{
    std::string     name;
    int             depth;
    int             parentIndex;
    Matrix          matOffset;
    Matrix          matBone;
    int             refCount;
    Bone() 
        : refCount(1)
        , depth(0)
        , parentIndex(0)
    {
    }
};

class CSkeleton : public CRef
{
private:
    friend class CAnimationManager;
    friend class CAnimationMesh;
    friend class CAnimationSequence;
    friend class CAnimationSequenceInstance;
    class CStructuredBuffer* m_pOffsetMatrixBuffer;
    class CScene* m_Scene;
    std::vector<Bone*>				m_vecBones;
    std::vector<class CSkeletonSocket*>	m_BoneSocket;
    CSkeleton();
    CSkeleton(const CSkeleton& skeleton);
    ~CSkeleton();
public:
    size_t GetBoneCount()	const;
    Bone* GetBone(int index = 0)	const;
    Bone* GetBone(const std::string& name)	const;
    int GetBoneIndex(const std::string& name)	const;
    bool CheckBone(const std::string& name)	const;
    const Matrix& GetBoneMatrix(const std::string& name)	const;
    const Matrix& GetBoneMatrix(int index)	const;
    class CSkeletonSocket* GetSocket(const std::string& name);
    void AddBone(Bone* bone);
    bool SaveSkeleton(const char* fileName, const std::string& pathName = MESH_PATH);
    bool SaveSkeletonFullPath(const char* fullPath);
    bool LoadSkeleton(class CScene* scene, const std::string& name, const char* fileName, 
        const std::string& pathName = MESH_PATH);
    bool LoadSkeletonFullPath(class CScene* scene, const std::string& name, const char* fullPath);
    void SetShader();
    void ResetShader();
    void AddSocket(const std::string& boneName, const std::string& socketName, 
        const Vector3& offset = Vector3(0, 0, 0), const Vector3& offsetRot =Vector3(0,0,0));
    void Update(float deltaTime, const std::vector<OutputBoneInfo>& vecBoneInfo, class CAnimationMeshComponent* owner);
    CSkeleton* Clone();
};

