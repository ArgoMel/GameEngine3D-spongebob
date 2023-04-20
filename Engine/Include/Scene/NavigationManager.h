#pragma once
#include "../EngineInfo.h"

class CNavigationManager
{
protected:
	friend class CScene;
	friend class CSceneManager;
	class CScene* m_Owner;
	std::vector<class CNavigationThread*>	m_vecNavThread;
	CNavigationManager();
	virtual ~CNavigationManager();
public:
	class CScene* GetScene()	const
	{
		return m_Owner;
	}
	virtual void AddNavigationThread(class CNavigationThread* thread)
	{
		m_vecNavThread.push_back(thread);
	}
	virtual void Start() = 0;
	virtual bool Init() = 0;
	virtual bool CreateNavigationMesh(class CSceneComponent* component);
	virtual void Move(class CNavigationAgent* agent, const Vector2& end);
	virtual void Move(class CNavigationAgent3D* agent, const Vector3& end);
	virtual float GetHeight(const Vector3& pos);
	virtual bool GetPickingPos(Vector3& result)	const;
};

