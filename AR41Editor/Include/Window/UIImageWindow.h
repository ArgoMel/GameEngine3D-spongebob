#pragma once
#include "Editor\EditorWindow.h"

class CUIImageWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	//애니메이션 옵션
	class CEditorInputText* m_PlayTime;
	class CEditorInputText* m_PlayScale;
	//만들거면 addframeinfo도 만들어줘야한다
	class CEditorInputText* m_PosX;
	class CEditorInputText* m_PosY;
	class CEditorInputText* m_SizeX;
	class CEditorInputText* m_SizeY;
	class CEditorSlider* m_Slider;
	CSharedPtr<class CUIImage>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	int m_Count;
	void ImgCreateCallback();
	void ImgChangeCallback();
	void TextureChangeCallback();
protected:
	CUIImageWindow();
	virtual ~CUIImageWindow();
public:
	void SetSelectWidget(class CUIImage* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

