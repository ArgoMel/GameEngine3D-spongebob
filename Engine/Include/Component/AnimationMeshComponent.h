#pragma once
#include "PrimitiveComponent.h"
#include "../Animation/Animation.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/Mesh/AnimationMesh.h"

class CAnimationMeshComponent : public CPrimitiveComponent
{
protected:
    friend class CGameObject;
    friend class CScene;
    CSharedPtr<CSkeleton>   m_Skeleton;
    CSharedPtr<CAnimation>  m_Animation;
    CAnimationMeshComponent();
    CAnimationMeshComponent(const CAnimationMeshComponent& component);
    virtual ~CAnimationMeshComponent();
public:
    CSkeleton* GetSkeleton()    const
    {
        return m_Skeleton;
    }
    CAnimation* GetAnimation()    const
    {
        return m_Animation;
    }
    std::string GetAnimationName()  const
    {
        if (!m_Animation)
        {
            return "None";
        }
        return m_Animation->GetAnimationClassName();
    }
    void GetAnimationNames(std::vector<std::string>& vecNames);
    void SetAnimation(class CAnimation* anim);
    virtual bool SetMesh(const std::string& name);
    virtual bool SetMesh(class CMesh* mesh);
    virtual bool SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& name, const TCHAR* fullPath);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual void RenderShadowMap();
    virtual CAnimationMeshComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    template <typename T>
    T* SetAnimation(const std::string& name)
    {
        T* anim = new T;
        anim->SetName(name);
        anim->m_Owner = this;
        if (!anim->Init())
        {
            SAFE_DELETE(anim);
            return nullptr;
        }
        m_Animation = anim;
        if (m_Skeleton)
        {
            m_Animation->SetSkeleton(m_Skeleton);
        }
        if (m_Mesh)
        {
            m_Animation->SetInstancingBoneBuffer(((CAnimationMesh*)m_Mesh.Get())->GetBoneBuffer());
        }
        return anim;
    }
};

