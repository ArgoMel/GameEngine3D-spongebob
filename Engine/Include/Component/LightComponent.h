#pragma once
#include "SceneComponent.h"
#include "../Resource/Shader/LightConstantBuffer.h"

class CLightComponent : public CSceneComponent
{
protected:
	friend class CGameObject;
	friend class CScene;
	CLightConstantBuffer*	m_CBuffer;
	LightCBuffer			m_LightInfo;
	CLightComponent();
	CLightComponent(const CLightComponent& component);
	virtual ~CLightComponent();
public:
	const LightCBuffer& GetLightInfo()	const
	{
		return m_LightInfo;
	}
	ELightType GetLightType()	const
	{
		return (ELightType)m_LightInfo.LightType;
	}
	float GetLightDistance()	const
	{
		return m_LightInfo.Distance;
	}
	float GetLightIntensity()	const
	{
		return m_LightInfo.Intensity;
	}
	void SetLightType(ELightType type)
	{
		m_LightInfo.LightType = (int)type;
		m_CBuffer->SetLightType(type);
	}
	void SetLightColor(const Vector4& color)
	{
		m_LightInfo.Color = color;
		m_CBuffer->SetLightColor(color);
	}
	void SetLightPos(const Vector3& pos)
	{
		m_LightInfo.Pos = pos;
		m_CBuffer->SetLightPos(pos);
	}
	void SetLightDir(const Vector3& dir)
	{
		m_LightInfo.Dir = dir;
		m_CBuffer->SetLightDir(dir);
	}
	void SetLightDistance(float distance)
	{
		m_LightInfo.Distance = distance;
		m_CBuffer->SetLightDistance(distance);
	}
	void SetLightAngleIn(float angleIn)
	{
		m_LightInfo.AngleIn = angleIn;
		m_CBuffer->SetLightAngleIn(angleIn);
	}
	void SetLightAngleOut(float angleOut)
	{
		m_LightInfo.AngleOut = angleOut;
		m_CBuffer->SetLightAngleOut(angleOut);
	}
	void SetLightAtt1(float att1)
	{
		m_LightInfo.Att1 = att1;
		m_CBuffer->SetLightAtt1(att1);
	}
	void SetLightAtt2(float att2)
	{
		m_LightInfo.Att2 = att2;
		m_CBuffer->SetLightAtt2(att2);
	}
	void SetLightAtt3(float att3)
	{
		m_LightInfo.Att3 = att3;
		m_CBuffer->SetLightAtt3(att3);
	}
	void SetLightIntensity(float intensity)
	{
		m_LightInfo.Intensity = intensity;
		m_CBuffer->SetLightIntensity(intensity);
	}
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CLightComponent* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void SetShader();
};

