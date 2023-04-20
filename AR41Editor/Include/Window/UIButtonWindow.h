#pragma once
#include "Editor\EditorWindow.h"

class CUIButtonWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_ButtonStateCB;
	class CEditorComboBox* m_ButtonEventStateCB;
	class CEditorCheckBox* m_ButtonEnableCheck;
	//�ִϸ��̼� �ɼ�
	class CEditorInputText* m_PlayTime;
	class CEditorInputText* m_PlayScale;
	//����Ÿ� addframeinfo�� ���������Ѵ�
	class CEditorInputText* m_PosX;
	class CEditorInputText* m_PosY;
	class CEditorInputText* m_SizeX;
	class CEditorInputText* m_SizeY;
	class CEditorSlider* m_Slider;
	CSharedPtr<class CUIButton>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	int m_ButtonStateIndex;
	int m_ButtonEventStateIndex;
	int m_Count;
	void StateCBCallback(int index, const std::string& item);
	void EventStateCBCallback(int index, const std::string& item);
	void ButtonCreateCallback();
	void ButtonChangeCallback();
	void TextureChangeCallback();
	void SoundChangeCallback();
protected:
	CUIButtonWindow();
	virtual ~CUIButtonWindow();
public:
	void SetSelectWidget(class CUIButton* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

