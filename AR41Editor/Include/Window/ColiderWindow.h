#pragma once
#include "Editor\EditorWindow.h"

class CColiderWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_ColiderCB;
	class CEditorListBox* m_ChannelList;
	class CEditorListBox* m_ProfileRelationList;
	class CEditorCheckBox* m_ProfileRelationCheck;
	class CEditorInputText* m_ProfileName;
	class CEditorInputText* m_ColiderSizeText[2];
	CSharedPtr<class CColliderBox2D>	m_SelectBox2D;
	CSharedPtr<class CColliderSphere2D>	m_SelectSphere2D;
	CSharedPtr<class CColliderOBB2D>	m_SelectOBB2D;
	CSharedPtr<class CSpriteComponent>	m_SelectSprite;
	std::string			m_SelectChannelItem;
	std::string			m_SelectRelationItem;
	int m_ColliderIndex;
	void ColiderClickCallback(int index, const std::string& item);
	void ChannelClickCallback(int index, const std::string& item);
	void RelationClickCallback(int index, const std::string& item);
	void ColiderCreateCallback();
	void ProfileCreateCallback();
	void ColiderChangeCallback();
	void ChannelChangeCallback();
	void RelationChangeCallback();
	void LoadChannelName();
protected:
	CColiderWindow();
	virtual ~CColiderWindow();
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

