#pragma once
#include "Particle.h"

class CParticleManager
{
private:
	friend class CResourceManager;
	std::unordered_map<std::string, CSharedPtr<CParticle>>	m_mapParticle;
	CParticleManager();
	~CParticleManager();
public:
	bool Init();
	void GetParticleNames(std::vector<std::string>& vecNames)
	{
		auto	iter = m_mapParticle.begin();
		auto	iterEnd = m_mapParticle.end();
		for (; iter != iterEnd; ++iter)
		{
			vecNames.push_back(iter->first);
		}
	}
	bool CreateParticle(const std::string& name);
	CParticle* FindParticle(const std::string& name);
	void ReleaseParticle(const std::string& name);
};

