#pragma once
#include "../../EngineInfo.h"
#include "Material.h"

class CMaterialManager
{
private:
	friend class CResourceManager;
	std::unordered_map<std::string, CSharedPtr<CMaterial>>	m_mapMaterial;
	CMaterialManager();
	~CMaterialManager();
public:
	void GetMaterialNames(std::vector<std::string>& vecNames)
	{
		auto	iter = m_mapMaterial.begin();
		auto	iterEnd = m_mapMaterial.end();
		for (; iter != iterEnd; ++iter)
		{
			vecNames.push_back(iter->first);
		}
	}
	bool Init();
	CMaterial* FindMaterial(const std::string& name);
	void ReleaseMaterial(const std::string& name);
	template <typename T>
	T* CreateMaterial(const std::string& name)
	{
		T* material = (T*)FindMaterial(name);
		if (material)
		{
			return material;
		}
		material = new T;
		material->SetName(name);
		m_mapMaterial.insert(std::make_pair(name, material));
		return material;
	}
};

