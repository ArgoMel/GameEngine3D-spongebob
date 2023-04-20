#pragma once
#include "Editor\EditorWindow.h"
class CUITextWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_AlignHCB;
	class CEditorComboBox* m_AlignVCB;
	class CEditorListBox* m_FontList;
	class CEditorCheckBox* m_TransparencyCheck;
	class CEditorInputText* m_PosX;
	class CEditorInputText* m_PosY;
	class CEditorInputText* m_SizeX;
	class CEditorInputText* m_SizeY;
	class CEditorInputText* m_TextInput;
	class CEditorInputText* m_FontSizeInput;
	class CEditorSlider* m_Slider;
	//shadow option
	class CEditorSlider* m_ShadowSlider;
	class CEditorInputText* m_ShadowOffsetX;
	class CEditorInputText* m_ShadowOffsetY;
	class CEditorCheckBox* m_ShadowEnableCheck;
	class CEditorCheckBox* m_ShadowTransparencyCheck;
	CSharedPtr<class CUIText>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	std::string			m_FontItem;
	int m_AlignHIndex;
	int m_AlignVIndex;
	int m_Count;
	void AlignHCBCallback(int index, const std::string& item);
	void AlignVCBCallback(int index, const std::string& item);
	void FontListClickCallback(int index, const std::string& item);
	void TextCreateCallback();
	void TextChangeCallback();
	void LoadFontName();
protected:
	CUITextWindow();
	virtual ~CUITextWindow();
public:
	void SetSelectWidget(class CUIText* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

