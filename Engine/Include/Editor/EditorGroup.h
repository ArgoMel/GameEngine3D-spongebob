#pragma once
#include "EditorWidget.h"

class CEditorGroup : public CEditorWidget
{
private:
	friend class CEditorWindow;
	template <typename T>
	friend class CEditorTreeItem;
	std::vector<CEditorWidget*>	m_vecWidget;
protected:
	CEditorGroup();
	virtual ~CEditorGroup();
public:
	virtual bool Init();
	virtual void Render();
	template <typename T>
	T* CreateWidget(const std::string& name, float width = 100.f, float height = 100.f)
	{
		T* widget = new T;
		widget->SetName(name);
		widget->m_Owner = m_Owner;
		widget->SetSize(width, height);
		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}
		m_vecWidget.push_back(widget);
		return widget;
	}
};

