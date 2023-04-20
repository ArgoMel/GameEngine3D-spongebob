#pragma once
#include "ComponentWidgetList.h"

class CSceneComponentWidgetList : public CComponentWidgetList
{
protected:
	friend class CEditorWindow;
	CSceneComponentWidgetList();
	virtual ~CSceneComponentWidgetList();
public:
	virtual bool Init();
};

