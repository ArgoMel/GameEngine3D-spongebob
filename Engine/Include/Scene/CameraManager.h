#pragma once
#include "../EngineInfo.h"

class CCameraManager
{
private:
	friend class CScene;
	class CScene* m_Owner;
	CSharedPtr<class CCameraComponent>	m_CurrentCamera;
	CSharedPtr<class CCameraComponent>	m_UICamera;
	CCameraManager();
	~CCameraManager();
public:
	class CCameraComponent* GetCurrentCamera()	const
	{
		return m_CurrentCamera;
	}
	class CCameraComponent* GetUICamera()	const
	{
		return m_UICamera;
	}
	void SetCurrentCamera(class CCameraComponent* camera);
	void SetUICamera(class CCameraComponent* camera);
	void Start();
	bool Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void Save(FILE* file);
	void Load(FILE* file);
};

