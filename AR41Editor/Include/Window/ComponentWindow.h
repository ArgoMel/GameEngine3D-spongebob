#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CComponentWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	CEditorTree<class CComponent*>* m_Tree;
	CEditorTree<class CUIWidget*>* m_WidgetTree;
	CSharedPtr<class CComponent>	m_SelectComponent;
	CSharedPtr<class CUIWidget>	m_SelectWidget;
	void TreeCallback(CEditorTreeItem<class CComponent*>* node, const std::string& item);
	void TreeDCCallback(CEditorTreeItem<class CComponent*>* node, const std::string& item);
	void WidgetCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item);
	void WidgetDCCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item);
protected:
	CComponentWindow();
	virtual ~CComponentWindow();
public:
	class CComponent* GetSelectComponent()	const
	{
		return m_SelectComponent;
	}
	class CUIWidget* GetSelectWidget()	const
	{
		return m_SelectWidget;
	}
	bool AddItem(class CComponent* component, const std::string& name, const std::string& parentName = "Root");
	bool AddWidget(class CUIWidget* widget, const std::string& name, const std::string& parentName = "Canvas");
	void AddInput(class CScene* scene);
	void Clear();
	void ClearSelect();
	virtual bool Init();
	virtual void Update(float deltaTime);
	void Delete();
	void ChangePos();
};

