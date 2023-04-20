#pragma once
#include "EditorWidget.h"

class CEditorButton : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	std::function<void()>	m_ClickCallback;
	bool					m_Click;
protected:
	CEditorButton();
	virtual ~CEditorButton();
public:
	virtual bool Init();
	virtual void Render();
	template <typename T>
	void SetClickCallback(T* obj, void(T::* func)())
	{
		m_ClickCallback = std::bind(func, obj);
	}
};

