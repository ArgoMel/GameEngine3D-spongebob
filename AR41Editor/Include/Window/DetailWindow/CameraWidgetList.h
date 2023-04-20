#pragma once
#include "ComponentWidgetList.h"

class CCameraWidgetList : public CComponentWidgetList
{
protected:
	friend class CEditorWindow;
	CCameraWidgetList();
	virtual ~CCameraWidgetList();
public:
	virtual bool Init();
};

