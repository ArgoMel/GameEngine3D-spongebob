#pragma once
#include "Editor\EditorWindow.h"

class CClassWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_ObjectList;
	class CEditorListBox* m_ComponentList;
	class CEditorInputText* m_ObjectSelectName;
	class CEditorInputText* m_ComponentSelectName;
	std::string			m_SelectObjectItem;
	std::string			m_SelectComponentItem;
	void ObjectClickCallback(int index, const std::string& item);
	void ComponentClickCallback(int index, const std::string& item);
	void ComponentCreateCallback();
	void ObjectSearch();
	void ComponentSearch();
	void LoadGameObjectName();
	void LoadComponentName();
protected:
	CClassWindow();
	virtual ~CClassWindow();
public:
	const std::string& GetSelectObjectItem()	const
	{
		return m_SelectObjectItem;
	}
	const std::string& GetSelectComponentItem()	const
	{
		return m_SelectComponentItem;
	}
	virtual bool Init();
	virtual void Update(float deltaTime);
	void ObjectCreateCallback();
};

