#pragma once
#include "Editor\EditorWindow.h"

class CAnimation2DWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_Animation2DClassList;
	class CEditorListBox* m_Animation2DSequenceList;
	class CEditorListBox* m_SequenceFrameList;
	class CEditorImage* m_Sequence2DImage;
	class CEditorInputText* m_StartX;
	class CEditorInputText* m_StartY;
	class CEditorInputText* m_EndX;
	class CEditorInputText* m_EndY;
	class CEditorInputText* m_Count;
	CSharedPtr<class CTexture>  m_SelectTexture;
	std::string		m_Animation2DSelectName;
	std::string		m_Animation2DSequenceSelectName;
	std::vector<Animation2DFrameData>	m_vecFrame;
	void LoadAnimation2DClassList();
	void LoadAnimation2DSequenceList();
	void CreateAnimation2D();
	void SelectTextureButton();
	void CreateAnimationSequence2D();
	void AddFrameClick();
	void AddAllFrameClick();
	void Animation2DSelectCallback(int index, const std::string& item);
	void SequenceSelectCallback(int index, const std::string& item);
	void FrameSelectCallback(int index, const std::string& item);
protected:
	CAnimation2DWindow();
	virtual ~CAnimation2DWindow();
public:
	const std::string& GetAnimation2DSelectName()	const
	{
		return m_Animation2DSelectName;
	}
	const std::string& GetAnimation2DSequenceSelectName()	const
	{
		return m_Animation2DSequenceSelectName;
	}
	virtual bool Init();
	virtual void Update(float deltaTime);
};

