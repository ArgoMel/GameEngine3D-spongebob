#pragma once
#include "ComponentWidgetList.h"

class CTargetArmWidgetList : public CComponentWidgetList
{
protected:
	friend class CEditorWindow;
	CTargetArmWidgetList();
	virtual ~CTargetArmWidgetList();
public:
	virtual bool Init();
};

