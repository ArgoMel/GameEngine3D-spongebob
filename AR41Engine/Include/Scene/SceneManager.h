#pragma once
#include "Scene.h"
#include "../GameObject/GameObject.h"

class CSceneManager
{
	DECLARE_SINGLE(CSceneManager)
private:
	CScene* m_Scene;
	CScene* m_NextScene;
	bool ChangeScene();
public:
	CScene* GetScene()	const
	{
		return m_Scene;
	}
	CScene* GetNextScene()	const
	{
		return m_NextScene;
	}
	bool Init();
	bool Update(float deltaTime);
	bool PostUpdate(float deltaTime);
	bool Collision(float deltaTime);
	void CreateNextScene(bool autoChange = true);
	void ChangeNextScene();
	template <typename T>
	bool CreateSceneInfo(bool current = true)
	{
		if (current)
		{
			return m_Scene->CreateSceneInfo<T>();
		}
		return m_NextScene->CreateSceneInfo<T>();
	}
	template <typename T>
	bool CreateSceneInfo(bool current, const std::string& name, const std::string& prevName="", const TCHAR* fileName = TEXT(""))
	{
		if (current)
		{
			return m_Scene->CreateSceneInfo<T>(name, prevName, fileName);
		}
		CGameObject* gizmo = m_Scene->FindObject("Gizmo");
		if(gizmo)
		{
			gizmo->Destroy();
		}
		return m_NextScene->CreateSceneInfo<T>(name, prevName, fileName);
	}
};