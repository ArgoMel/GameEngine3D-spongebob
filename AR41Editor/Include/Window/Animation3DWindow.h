#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CAnimation3DWindow : public CEditorWindow
{
	friend class CEditorGUIManager;
	//애니메이션 
	class CEditorInputText* m_AnimationName;
	class CEditorInputText* m_CurAnimName;	//현재 애니메이션 정보
	class CEditorInputText* m_ChangeAnimName;	//다음 애니메이션 정보
	class CEditorInputText* m_BlendScale[3];
	class CEditorInputText* m_BlendPos[3];
	class CEditorInputText* m_BlendRot[4];
	class CEditorInputText* m_GlobalTime;
	class CEditorInputText* m_SequenceProgress;
	class CEditorInputText* m_ChangeTimeAcc;
	class CEditorInputText* m_ChangeTime;
	class CEditorCheckBox* m_Play;
	CEditorTree<class CAnimationData*>* m_AnimTree;
	class CEditorInputText* m_AnimName;
	class CEditorInputText* m_AnimTime;
	class CEditorInputText* m_AnimFrameTime;
	class CEditorInputText* m_AnimPlayTime;
	class CEditorInputText* m_AnimPlayScale;
	class CEditorCheckBox*	m_AnimLoop;
	class CEditorCheckBox*	m_AnimReverse;
	//시퀸스 정보
	class CEditorInputText* m_SequenceAnimName;
	class CEditorCheckBox* m_Loop;
	class CEditorCheckBox* m_End;
	class CEditorInputText* m_StartTime;
	class CEditorInputText* m_EndTime;
	class CEditorInputText* m_TimeLength;
	class CEditorInputText* m_FrameTime;
	class CEditorInputText* m_PlayTime;
	class CEditorInputText* m_PlayScale;
	class CEditorInputText* m_StartFrame;
	class CEditorInputText* m_EndFrame;
	class CEditorInputText* m_FrameLength;
	class CEditorInputText* m_FrameMode;
	class CEditorInputText* m_ChangeFrame;
   // std::vector<BoneKeyFrame*>	m_vecKeyFrame;
	//스켈레톤
	class CEditorInputText* m_SkeletoneName;
	class CEditorInputText* m_BoneCount;
//얘네 할거면 트리로
	//class CEditorInputText* m_BoneName;
	//class CEditorInputText* m_SocketName;
	CSharedPtr<class CAnimationSequence>	m_SelectAnimationSequence;
	CSharedPtr<class CAnimation>	m_SelectAnimation;
	CSharedPtr<class CSkeleton>		m_SelectSkeleton;
	void UpdateInfo();
	void SequenceChangeInfo();
	void SequenceSaveInfo();
	void AnimTreeCallback(CEditorTreeItem<class CAnimationData*>* node, const std::string& item);
protected:
	CAnimation3DWindow();
	virtual ~CAnimation3DWindow();
public:
	void SetSelectComponent(class CAnimationMeshComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
	bool AddItem(class CAnimationData* data, const std::string& name, const std::string& parentName = "AnimationData");
	void Clear();
	void ClearSelect();
	void ChangeAnimation3D(const TCHAR* path);
};

