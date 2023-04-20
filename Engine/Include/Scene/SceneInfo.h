#pragma once
#include "../EngineInfo.h"

class CSceneInfo
{
protected:
	friend class CScene;
	friend class CSceneManager;
	class CScene* m_Owner;
	CSharedPtr<class CGameObject> m_PlayerObject;
	CSharedPtr<class CGameObject> m_BossObject;
	std::string	m_ClassTypeName;
	std::string	m_PlayerObjectName;
	std::string	m_BossObjectName;
	std::string	m_FileName;
	std::string	m_PrevFileName;
	TCHAR		m_CutSceneName[MAX_PATH];
	CSceneInfo();
	CSceneInfo(const CSceneInfo& info);
	virtual ~CSceneInfo();
	void SetClassTypeName(const std::string& name)
	{
		m_ClassTypeName = name;
	}
public:
	CGameObject* GetPlayerObject()	const
	{
		return m_PlayerObject;
	}
	CGameObject* GetBossObject()	const
	{
		return m_BossObject;
	}
	const std::string& GetFileName()	const
	{
		return m_FileName;
	}
	const std::string& GetPrevFileName()	const
	{
		return m_PrevFileName;
	}
	const TCHAR* GetCutSceneName()	const
	{
		return m_CutSceneName;
	}
	void SetPlayerObject(class CGameObject* player);
	void SetBossObject(class CGameObject* boss);
	void SetFileName(const std::string& name)
	{
		m_FileName = name;
	}
	void SetPrevFileName(const std::string& name)
	{
		m_PrevFileName = name;
	}
	void SetCutSceneName(const TCHAR* fileName)
	{
		_tcscpy_s(m_CutSceneName, MAX_PATH, fileName);
	}
	void LoadComplete();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CSceneInfo* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual void SceneChangeComplete();
};

