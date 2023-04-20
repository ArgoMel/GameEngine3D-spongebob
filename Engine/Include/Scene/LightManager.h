#pragma once
#include "../EngineInfo.h"

class CLightManager
{
private:
	friend class CScene;
	class CScene* m_Owner;
	class CStructuredBuffer* m_LightInfoBuffer;
	CSharedPtr<class CGameObject>	m_GlobalLightObj;
	CSharedPtr<class CLightComponent>	m_GlobalLightComponent;
	CSharedPtr<class CShader>	m_LightAccShader;
	size_t				m_LightInfoBufferCount;
	std::list<CSharedPtr<class CLightComponent>>	m_LightList;
	CLightManager();
	~CLightManager();
public:
	class CGameObject* GetGlobalLightObj()	const
	{
		return m_GlobalLightObj;
	}
	class CLightComponent* GetGlobalLightComponent()	const
	{
		return m_GlobalLightComponent;
	}
	void SetGlobalLightObject(class CGameObject* light);
	void AddLight(class CLightComponent* light);
	void DeleteLight(class CLightComponent* light);
	void DeleteLight(const std::string& name);
	void SetLightAccShader(const std::string& name);
	void Start();
	bool Init();
	void Update(float deltaTime);
	void Render();
	void Save(FILE* file);
	void Load(FILE* file);
};

