#pragma once
#include "GameObject/GameObject.h"

class CGizmo : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CTargetArm> m_Arm;
	CSharedPtr<class CCameraComponent> m_Camera;
	float m_Speed;
protected:
	CGizmo();
	CGizmo(const CGizmo& obj);
	virtual ~CGizmo();
public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGizmo* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	void SetCamera();
	void UArrow();
	void DArrow();
	void LArrow();
	void RArrow();
	void PageUp();
	void PageDown();
};

