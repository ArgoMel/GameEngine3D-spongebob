#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CParticleWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
//������Ʈ ����
	class CEditorInputText* m_SpawnTime;
	class CEditorInputText* m_SpawnCountMax;
//��ƼŬ ����
	class CEditorInputText* m_Name;	//��ƼŬ �̸�
//��ƼŬ ���� ����
	class CEditorComboBox* m_ParticleCamDir;
	class CEditorCheckBox* m_ParticleSpawnEnable;
	class CEditorCheckBox* m_ParticleMoveEnable;
	class CEditorCheckBox* m_ParticleGravity;
	class CEditorCheckBox* m_ParticleMoveDirEnable;
	class CEditorInputText* m_ParticleLifeTimeMin;
	class CEditorInputText* m_ParticleLifeTimeMax;
	class CEditorInputText* m_ParticleStartMin[3];
	class CEditorInputText* m_ParticleStartMax[3];
	class CEditorInputText* m_ParticleScaleMin[3];
	class CEditorInputText* m_ParticleScaleMax[3];
	class CEditorInputText* m_ParticleMoveDir[3];
	class CEditorInputText* m_ParticleMoveAngle[3];
	class CEditorInputText* m_ParticleColorMin[4];
	class CEditorInputText* m_ParticleColorMax[4];
	CSharedPtr<class CParticleComponent>	m_SelectComponent;
	CSharedPtr<class CParticle>	m_SelectParticle;
//��ƼŬ ����Ʈ
	class CEditorListBox* m_ParticleList;
	std::string			m_SelectListItem;
	void ListClickCallback(int index, const std::string& item);
	void ParticleChangeCallback();
	void ParticleLoadCallback();
protected:
	CParticleWindow();
	virtual ~CParticleWindow();
public:
	void SetSelectComponent(class CParticleComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

