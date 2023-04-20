#pragma once
#include "Editor\EditorWindow.h"

class CDetailWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	void ChangeWidget(class CSceneComponent* component);
	void LoadButtonClick();
	void CreateEditorWidgetList(ESceneComponentType type);
protected:
	CSharedPtr<class CSceneComponent> m_SelectComponent;
	std::vector<class CComponentWidgetList*>	m_vecComponentWidgetList;
	CDetailWindow();
	virtual ~CDetailWindow();
public:
	class CSceneComponent* GetSelectComponent()	const
	{
		return m_SelectComponent;
	}
	class CComponentWidgetList* GetComponentWidgetList(const std::string& name);
	void SetSelectComponent(class CSceneComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

