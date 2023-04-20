#pragma once
#include "Editor\EditorWindow.h"

class CTransformWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorInputText* m_Pos[3];
	class CEditorInputText* m_Rot[3];
	class CEditorInputText* m_Scale[3];
	CSharedPtr<class CSceneComponent>	m_SelectComponent;
protected:
	CTransformWindow();
	virtual ~CTransformWindow();
public:
	void SetSelectComponent(class CSceneComponent* component);
	void SetPos(const Vector3& pos);
	void SetRotation(const Vector3& rot);
	void SetScale(const Vector3& scale);
	virtual bool Init();
	virtual void Update(float deltaTime);
	void PosXCallback();
	void PosYCallback();
	void PosZCallback();
	void RotXCallback();
	void RotYCallback();
	void RotZCallback();
	void ScaleXCallback();
	void ScaleYCallback();
	void ScaleZCallback();
};

