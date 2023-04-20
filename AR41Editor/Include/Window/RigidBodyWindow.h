#pragma once
#include "Editor\EditorWindow.h"

class CRigidBodyWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_Check;
	class CEditorCheckBox* m_Gravity;
	class CEditorCheckBox* m_Ground;
	class CEditorInputText* m_Force[3];
	class CEditorInputText* m_Accel[3];
	class CEditorInputText* m_AccelA[3];
	class CEditorInputText* m_Velocity[3];
	class CEditorInputText* m_MaxVelocity[3];
	class CEditorInputText* m_MinVelocity[3];
	class CEditorInputText* m_Mass;
	class CEditorInputText* m_FricCoeff;
	class CEditorInputText* m_GravityForce;
	CSharedPtr<class CRigidBody>	m_SelectRigidBody;
	void RigidBodyChangeCallback();
protected:
	CRigidBodyWindow();
	virtual ~CRigidBodyWindow();
public:
	void SetSelectComponent(class CRigidBody* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

