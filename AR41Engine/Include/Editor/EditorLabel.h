#pragma once
#include "EditorWidget.h"
class CEditorLabel :public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	ImVec2	m_Align;
protected:
	CEditorLabel();
	virtual ~CEditorLabel();
public:
	void SetAlign(float x, float y)
	{
		m_Align.x = x;
		m_Align.y = y;
	}
	virtual bool Init();
	virtual void Render();
};

