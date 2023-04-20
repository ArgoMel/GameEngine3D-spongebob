#pragma once
#include "PrimitiveComponent.h"

enum class EDecalFadeState
{
    None,
    FadeIn,
    FadeOut,
    FadeInOut
};

enum class EFadeState
{
    FadeIn,
    Duration,
    FadeOut
};

class CDecalComponent : public CPrimitiveComponent
{
private:
    friend class CGameObject;
    friend class CScene;
#ifdef _DEBUG
    CSharedPtr<class CStaticMesh>   m_DebugMesh;
    CSharedPtr<class CMaterial>     m_DebugMaterial;
#endif // _DEBUG
    EDecalFadeState m_FadeState;
    EFadeState  m_Fade;
    float   m_FadeTime;
    float   m_FadeInTime;
    float   m_FadeOutTime;
    float   m_FadeOutDelayTime;
    bool    m_FinishDelete;
protected:
    CDecalComponent();
    CDecalComponent(const CDecalComponent& component);
    virtual ~CDecalComponent();
public:
    void SetFadeState(EDecalFadeState state)
    {
        m_FadeState = state;
    }
    void SetFadeInTime(float time)
    {
        m_FadeInTime = time;
    }
    void SetFadeOutTime(float time)
    {
        m_FadeOutTime = time;
    }
    void SetFadeOutDelayTime(float time)
    {
        m_FadeOutDelayTime = time;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    void RenderDebug();
    virtual CDecalComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

