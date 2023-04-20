#pragma once
#include "NavigationManager.h"

class CNavigationManager3D : public CNavigationManager
{
protected:
	friend class CScene;
	friend class CSceneManager;
	class CNavigationMesh* m_NavigationMesh;
	class CTerrainComponent* m_Terrain;
	CNavigationManager3D();
	virtual ~CNavigationManager3D();
public:
	virtual void AddNavigationThread(class CNavigationThread* thread);
	virtual void Start();
	virtual bool Init();
	virtual bool CreateNavigationMesh(class CSceneComponent* component);
	virtual void Move(class CNavigationAgent3D* agent, const Vector3& end);
	virtual float GetHeight(const Vector3& pos);
	virtual bool GetPickingPos(Vector3& result)	const;
};

