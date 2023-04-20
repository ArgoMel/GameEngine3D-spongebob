#pragma once
#include "Editor\EditorWindow.h"

class CUINumberWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorInputText* m_NumberInput;
	class CEditorInputText* m_SpaceInput;
	class CEditorInputText* m_PlayTime;
	class CEditorInputText* m_PlayScale;
	class CEditorInputText* m_PosX;
	class CEditorInputText* m_PosY;
	class CEditorInputText* m_SizeX;
	class CEditorInputText* m_SizeY;
	class CEditorSlider* m_Slider;
	CSharedPtr<class CUINumber>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	int m_Count;
	void NumberCreateCallback();
	void NumberChangeCallback();
	void TextureChangeCallback();
protected:
	CUINumberWindow();
	virtual ~CUINumberWindow();
public:
	void SetSelectWidget(class CUINumber* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

