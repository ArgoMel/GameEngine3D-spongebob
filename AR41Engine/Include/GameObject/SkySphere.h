#pragma once
#include "GameObject.h"

class CSkySphere : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CMaterial>	m_Material;
protected:
	CSkySphere();
	CSkySphere(const CSkySphere& obj);
	virtual ~CSkySphere();
public:
	void SetSkyTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	virtual void Start();
	virtual bool Init();
	virtual CSkySphere* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
};

