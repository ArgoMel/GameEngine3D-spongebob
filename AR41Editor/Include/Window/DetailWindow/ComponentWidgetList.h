#pragma once
#include "Editor\EditorGroup.h"

class CComponentWidgetList : public CEditorGroup
{
protected:
	friend class CEditorWindow;
	friend class CDetailWindow;
	class CDetailWindow* m_DetailWindow;
	CComponentWidgetList();
	virtual ~CComponentWidgetList();
public:
	virtual bool Init();
	virtual void Render();
};

