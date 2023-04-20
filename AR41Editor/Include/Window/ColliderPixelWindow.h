#pragma once
#include "Editor\EditorWindow.h"

class CColliderPixelWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_ChannelList;
	class CEditorListBox* m_ProfileRelationList;
	class CEditorCheckBox* m_ProfileRelationCheck;
	class CEditorInputText* m_ProfileName;
	class CEditorSlider* m_Slider;
	class CEditorComboBox* m_ColorTypeCB;
	class CEditorComboBox* m_AlphaTypeCB;
	CSharedPtr<class CColliderPixel>	m_SelectComponent;
	CSharedPtr<class CSpriteComponent>	m_SelectSprite;
	std::string			m_SelectChannelItem;
	std::string			m_SelectRelationItem;
	int m_ColorTypeIndex;
	int m_AlphaTypeIndex;
	void ChannelClickCallback(int index, const std::string& item);
	void RelationClickCallback(int index, const std::string& item);
	void ColorTypeClickCallback(int index, const std::string& item);
	void AlphaTypeClickCallback(int index, const std::string& item);
	void ColiderCreateCallback();
	void ProfileCreateCallback();
	void PixelCreateCallback();
	void ChannelChangeCallback();
	void RelationChangeCallback();
	void PixelInfoChangeCallback();
	void LoadChannelName();
protected:
	CColliderPixelWindow();
	virtual ~CColliderPixelWindow();
public:
	const std::string& GetSelectChannelItem()	const
	{
		return m_SelectChannelItem;
	}
	void SetSelectComponent(class CCollider* component);
	void SetSelectSprite(class CSpriteComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

