#pragma once
#include "Scene\SceneInfo.h"

class CLoadingSceneInfo : public CSceneInfo
{
private:
    class CLoadingThread* m_LoadingThread;
    class CThreadQueue* m_LoadingQueue;
    class CLoadingUI* m_LoadingUI;
    CSharedPtr<CGameObject>	m_Particle;
    float m_StartTime;
public:
    CLoadingSceneInfo();
    ~CLoadingSceneInfo();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void SceneChangeComplete();
};

