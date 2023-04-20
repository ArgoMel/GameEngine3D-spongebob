#pragma once
#include "../../Ref.h"
#include "../Material/Material.h"

class CParticle : public CRef
{
private:
    friend class CParticleManager;
    friend class CResourceManager;
    friend class CSceneResource;
    friend class CParticleComponent;
    class CScene* m_Scene;
    class CParticleConstantBuffer* m_CBuffer;
    CSharedPtr<class CParticleUpdateShader> m_UpdateShader;
    CSharedPtr<class CParticleRenderShader> m_RenderShader;
    CSharedPtr<CMaterial>       m_Material;
    float           m_SpawnTime;
    std::vector<class CStructuredBuffer*>   m_vecStructuredBuffer;
    CParticle();
    ~CParticle();
public:
    CParticleConstantBuffer* GetParticleCBuffer()	const
    {
        return m_CBuffer;
    }
    void SetUpdateShader(const std::string& name);
    void SetRenderShader(const std::string& name);
    void AddStructuredBuffer(const std::string& name, unsigned int size, unsigned int count, int reg, 
        bool dynamic = false, int structuredBufferShaderType = (int)EShaderBufferType::Compute);
    void ResizeStructuredBuffer(const std::string& name, unsigned int size, unsigned int count, int reg, 
        bool dynamic = false, int structuredBufferShaderType = (int)EShaderBufferType::Compute);
    void CloneStructuredBuffer(std::vector<class CStructuredBuffer*>& vecBuffer);
    class CParticleConstantBuffer* CloneCBuffer()   const;
    void SetParticleSpawnCountMax(int count);
    void SetParticleSpawnTime(float time);
    void SetMaterial(const std::string& name);
    void SetMaterial(class CMaterial* material);
    void SetParticleEnable(bool enable);
    void SetParticleStartMin(const Vector3& min);
    void SetParticleStartMax(const Vector3& max);
    void SetParticleScaleMin(const Vector3& min);
    void SetParticleScaleMax(const Vector3& max);
    void SetParticleLifeTimeMin(float min);
    void SetParticleLifeTimeMax(float max);
    void SetParticleColorMin(const Vector4& min);
    void SetParticleColorMax(const Vector4& max);
    void SetParticleSpeedMin(float min);
    void SetParticleSpeedMax(float max);
    void SetParticleMoveEnable(bool enable);
    void SetParticleGravityEnable(bool enable);
    void SetParticleMoveDir(const Vector3& moveDir);
    void SetParticleMoveDirEnable(bool enable);
    void SetParticleMoveAngle(const Vector3& moveAngle);
    void SetParticleCamDir(EParticle_Cam_Dir dir);
    bool Init();
};

