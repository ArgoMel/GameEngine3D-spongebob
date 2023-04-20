#pragma once
#include "Editor\EditorWindow.h"

class CImageWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_SpriteList;
	class CEditorInputText* m_SpriteSelectName;
	class CEditorImage* m_Image;
	class CEditorSlider* m_Slider;
	CSharedPtr<class CSpriteComponent>	m_SelectComponent;
	std::string			m_SelectSpriteItem;
	std::string			m_SpritePath;
	void SpriteInputCallback();
	void SpriteClickCallback(int index, const std::string& item);
	void SpriteCreateCallback();
	void SpriteColorCallback();
	void LoadSpriteName();
protected:
	CImageWindow();
	virtual ~CImageWindow();
public:
	const std::string& GetSelectSpriteItem()	const
	{
		return m_SelectSpriteItem;
	}
	void SetSelectComponent(class CSpriteComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

