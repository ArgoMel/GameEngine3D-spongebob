#pragma once
#include "Editor\EditorWindow.h"

class CSceneWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_SceneList;
	class CEditorInputText* m_SceneSelectName;
	std::string			m_SelectSceneItem;
	void SceneChange();
	void SceneSave();
	void SceneClickCallback(int index, const std::string& item);
	void LoadSceneDirectory();
protected:
	CSceneWindow();
	virtual ~CSceneWindow();
public:
	const std::string& GetSelectSceneItem()	const
	{
		return m_SelectSceneItem;
	}
	virtual bool Init();
	virtual void Update(float deltaTime);
};

