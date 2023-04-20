#pragma once
#include "Editor\EditorWindow.h"

class CCameraWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_CameraCB;
	class CEditorCheckBox* m_CameraHorizon;
	class CEditorCheckBox* m_CameraVertical;
	class CEditorInputText* m_CameraViewDistance;
	class CEditorInputText* m_CameraSpeed;
	CSharedPtr<class CCameraComponent>	m_SelectCamera;
	void CameraChangeCallback();
protected:
	CCameraWindow();
	virtual ~CCameraWindow();
public:
	void SetSelectComponent(class CSceneComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

