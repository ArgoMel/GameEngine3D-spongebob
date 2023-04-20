#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CParticleWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
//컴포넌트 변수
	class CEditorInputText* m_SpawnTime;
	class CEditorInputText* m_SpawnCountMax;
//파티클 변수
	class CEditorInputText* m_Name;	//파티클 이름
//파티클 버퍼 변수
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
//파티클 리스트
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

