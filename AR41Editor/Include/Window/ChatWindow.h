#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CChatWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_ChatRange;
	class CEditorInputText* m_Chat;
	CEditorTree<class CEditorText*>* m_TextTree;
	std::string			m_SelectTextItem;
	int m_ClickCount;
	void SendBtnCallback();
	void TreeCallback(CEditorTreeItem<class CEditorText*>* node, const std::string& item);
protected:
	CChatWindow();
	virtual ~CChatWindow();
public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	void Clear();
	void ClearSelect();
	void Delete();
	bool AddItem(class CEditorText* text, const std::string& name, const std::string& parentName = "√§∆√√¢");
};

