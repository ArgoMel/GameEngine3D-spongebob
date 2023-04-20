#pragma once
#include "PrimitiveComponent.h"

class CParticleComponent : public CPrimitiveComponent
{
private:
    friend class CGameObject;
    friend class CScene;
    class CParticleConstantBuffer* m_CBuffer;
    CSharedPtr<class CParticle>   m_Particle;
    float               m_SpawnTime;
    float               m_SpawnTimeMax;
    int                 m_SpawnCountMax;
    std::vector<class CStructuredBuffer*>   m_vecStructuredBuffer;
protected:
    CParticleComponent();
    CParticleComponent(const CParticleComponent& component);
    virtual ~CParticleComponent();
public:
    CParticle* GetParticle()	const
    {
        return m_Particle;
    }
    float GetSpawnTime()	const
    {
        return m_SpawnTime;
    }
    float GetSpawnTimeMax()	const
    {
        return m_SpawnTimeMax;
    }
    int GetSpawnCountMax()	const
    {
        return m_SpawnCountMax;
    }
    void DeleteCurrentParticle();
    void ChangeParticle(const std::string& name);
    void SetSpawnTime(float time);
    void SetSpawnCountMax(int count);
    void SetCamDir(EParticle_Cam_Dir dir);
    void SetParticle(const std::string& name);
    void SetParticle(class CParticle* particle);
    virtual void SetMaterial(int slot, const std::string& name);
    virtual void SetMaterial(int slot, class CMaterial* material);
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CParticleComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

