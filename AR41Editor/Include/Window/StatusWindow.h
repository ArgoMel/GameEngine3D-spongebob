#pragma once
#include "Editor\EditorWindow.h"

class CStatusWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_Check;
	class CEditorCheckBox* m_FXAACheck;
	class CEditorInputText* m_Frame;
	class CEditorInputText* m_PosX;
	class CEditorInputText* m_PosY;
	class CEditorInputText* m_PosZ;
	class CEditorInputText* m_SizeX;
	class CEditorInputText* m_SizeY;
	class CEditorInputText* m_SizeZ;
	//¸¶¿ì½º
	class CEditorInputText* m_RayPos[3];
	class CEditorInputText* m_RayDir[3];
	class CEditorInputText* m_MousePos[2];
	class CEditorInputText* m_MouseWorldPos[3];
	class CEditorInputText* m_MouseUIPos[2];
	class CEditorInputText* m_MouseMove[2];
	class CEditorInputText* m_MouseMove2D[2];
	CSharedPtr<class CSceneComponent>	m_SelectComponent;
	CSharedPtr<class CUIWidget>	m_SelectWidget;
	void FXAACheckCallback(bool check);
protected:
	CStatusWindow();
	virtual ~CStatusWindow();
public:
	void SetSelectComponent(class CSceneComponent* component);
	void SetSelectWidget(class CUIWidget* widget);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

