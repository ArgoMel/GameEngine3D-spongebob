#pragma once
#include "ComponentWidgetList.h"

class CSpriteComponentWidgetList : public CComponentWidgetList
{
private:
	friend class CEditorWindow;
	class CEditorInputText* m_PlayTime;
	class CEditorInputText* m_PlayScale;
	class CEditorInputText* m_AnimationClass;
	class CEditorListBox* m_AnimationList;
	class CEditorCheckBox* m_LoopCheck;
	class CEditorCheckBox* m_ReverseCheck;
	class CEditorInputText* m_SequenceName;
	class CEditorInputText* m_Animation2DDataName;
	class CEditorInputText* m_ImageName;
	class CEditorImage* m_Image;
	class CSpriteComponent* m_Sprite;
	void SelectTextureButtonClick();
	void CreateAnimationButtonClick();
	void AnimationSelectCallback(int index, const std::string& item);
	void AddAnimation2DData();
	void DeleteAnimation2DData();
	void ModifyAnimation2DData();
protected:
	CSpriteComponentWidgetList();
	virtual ~CSpriteComponentWidgetList();
public:
	void SetSpriteContent(class CSpriteComponent* sprite);
	void SetSelectAnimationSequence2DName(const std::string& name);
	virtual bool Init();
};

