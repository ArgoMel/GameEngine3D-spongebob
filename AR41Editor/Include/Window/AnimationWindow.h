#pragma once
#include "Editor\EditorWindow.h"

class CAnimationWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_IMGList;
	class CEditorListBox* m_FrameList;
	class CEditorInputText* m_IMGSelectName;
	class CEditorInputText* m_AnimationName;
	class CEditorInputText* m_AnimationInfo[2];
	class CEditorImage* m_Image;
	class CEditorSlider* m_Slider;
	class CEditorCheckBox* m_LoopCheck;
	class CEditorCheckBox* m_ReverseCheck;
	class CAnimation2D* m_Anim;
	CSharedPtr<class CSpriteComponent>	m_SelectComponent;
	std::string			m_FolderName;
	std::string			m_SelectIMGItem;
	std::string			m_IMGPath;
	int m_ListIndex;
	void IMGClickCallback(int index, const std::string& item);
	void IMGAddCallback();
	void IMGMultiAddCallback();
	void IMGColorCallback();
	void IMGSearch();
	void IMGSetName();
	void AnimationCreateCallback();
	void AnimationChangeCallback();
	void AnimationSaveCallback();
	void FrameDeleteCallback();
	void FrameClearCallback();
	void LoadIMGName(class CSpriteComponent* component);
protected:
	CAnimationWindow();
	virtual ~CAnimationWindow();
public:
	const std::string& GetSelectIMGItem()	const
	{
		return m_SelectIMGItem;
	}
	void SetSelectComponent(class CSpriteComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

