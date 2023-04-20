#pragma once
#include "EditorWidget.h"
class CEditorCheckBox : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	std::function<void(bool)>	m_CheckCallback;
	bool					m_Check;
protected:
	CEditorCheckBox();
	virtual ~CEditorCheckBox();
public:
	const bool GetCheck()	const
	{
		return m_Check;
	}
	void SetCheck(bool b)
	{
		m_Check = b;
	}
	virtual bool Init();
	virtual void Render();
	template <typename T>
	void SetClickCallback(T* obj, void(T::* func)(bool))
	{
		m_CheckCallback = std::bind(func, obj, std::placeholders::_1);
	}
};

