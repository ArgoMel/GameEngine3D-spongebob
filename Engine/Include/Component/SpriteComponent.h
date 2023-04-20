#pragma once
#include "PrimitiveComponent.h"
#include "../Animation/Animation2D.h"

class CSpriteComponent : public CPrimitiveComponent
{
protected:
    friend class CGameObject;
    friend class CScene;
    friend class CInput;
    CSharedPtr<CAnimation2D> m_Animation;
    CSpriteComponent();
    CSpriteComponent(const CSpriteComponent& component);
    virtual ~CSpriteComponent();
public:
    class CAnimation2D* GetAnimation2D()    const
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
    bool SetTexture(class CTexture* texture);
    bool SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
    bool SetTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
    void SetTextureFrameIndex(int index);
    class CTexture* GetTexture(int index = 0)    const;
    void ClearAnimation();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CSpriteComponent* Clone()    const;
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
        return anim;
    }
};

