#pragma once
#include "EditorInfo.h"

class CEditorWindow
{
protected:
	friend class CEditorGUIManager;
	std::string		m_Name;
	bool			m_Open;
	int				m_WindowFlag;
	std::vector<class CEditorWidget*>	m_vecWidget;
	CEditorWindow();
	virtual ~CEditorWindow();
public:
	bool GetOpen()
	{
		return m_Open;
	}
	void SetName(const std::string& name)
	{
		m_Name = name;
	}
	void AddWindowFlag(ImGuiWindowFlags_ flag)
	{
		m_WindowFlag |= flag;
	}
	void AddWidget(CEditorWidget* widget)
	{
		m_vecWidget.push_back(widget);
	}
	void DeleteAllWidget();
	void ClearWidget();
	void Open();
	void Close();
	class CEditorWidget* FindWidget(const std::string& name);
	virtual bool Init();
	virtual void Update(float deltaTime);
	template <typename T>
	T* CreateWidget(const std::string& name, float width = 100.f, float height = 100.f)
	{
		T* widget = new T;
		widget->SetName(name);
		widget->m_Owner = this;
		widget->SetSize(width, height);
		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}
		m_vecWidget.push_back(widget);
		return widget;
	}
	template <typename T>
	T* CreateWidgetEmpty(const std::string& name, float width = 100.f, float height = 100.f)
	{
		T* widget = new T;
		widget->SetName(name);
		widget->m_Owner = this;
		widget->SetSize(width, height);
		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}
		return widget;
	}
};

