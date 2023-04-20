#pragma once
#include "Editor\EditorWindow.h"

class CLightWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox*	m_LightType;
	class CEditorInputText* m_Color[4];
	class CEditorInputText* m_Pos[3];
	class CEditorInputText* m_Dir[3];
	class CEditorInputText* m_Distance;
	class CEditorInputText* m_AngleIn;
	class CEditorInputText* m_AngleOut;
	class CEditorInputText* m_Att1;
	class CEditorInputText* m_Att2;
	class CEditorInputText* m_Att3;
	class CEditorInputText* m_Intensity;
	class CEditorInputText* m_ShadowBias;
	class CEditorInputText* m_ShadowResolution[2];
	CSharedPtr<class CLightComponent>	m_SelectComponent;
	void LightChangeCallback();
protected:
	CLightWindow();
	virtual ~CLightWindow();
public:
	void SetSelectComponent(class CLightComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

