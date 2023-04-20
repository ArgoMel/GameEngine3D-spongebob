#pragma once
#include "NavigationManager.h"

class CNavigationManager2D : public CNavigationManager
{
protected:
	friend class CScene;
	friend class CSceneManager;
	CNavigationManager2D();
	virtual ~CNavigationManager2D();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Move(class CNavigationAgent* agent, const Vector2& end);
};

