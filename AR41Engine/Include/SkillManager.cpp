#include "SkillManager.h"

DEFINITION_SINGLE(CSkillManager)

CSkillManager::CSkillManager()
{
}

CSkillManager::~CSkillManager()
{
	auto	iter = m_mapSkill.begin();
	auto	iterEnd = m_mapSkill.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}
// ~~������Ʈ�� ~~��ų�� �ߵ��Ѵ�
bool CSkillManager::Init()
{
	//CSkill* skill = CreateSkill("owl0");
	//skill->AddSkill("owl0", skill, &CSkill::RandomSummon);
	return true;
}

void CSkillManager::Update(float deltaTime)
{
	auto	iter = m_SkillList.begin();
	auto	iterEnd = m_SkillList.end();
	for (; iter != iterEnd;)
	{
		if ((*iter)->skillTime>=0.f)
		{
			(*iter)->skillTime += deltaTime;
			(*iter)->func((*iter)->obj);
			if((*iter)->skillTime>= (*iter)->skillMaxTime)
			{
				iter = m_SkillList.erase(iter);
				iterEnd = m_SkillList.end();
				continue;
			}
		}
		++iter;
	}
}

void CSkillManager::PostUpdate(float deltaTime)
{
	//bool ������ ������Ʈ,����Ʈ�� ���� ���ϸ� �����Ҽ� ����
}

SkillInfo* CSkillManager::FindSkill(const std::string& name)
{
	auto	iter = m_mapSkill.find(name);
	if (iter == m_mapSkill.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CSkillManager::ActivateSkill(const std::string& name, class CGameObject* obj)
{
	SkillInfo* skill = FindSkill(name);
	if (!skill)
	{
		return;
	}
	skill->obj = obj;
	m_SkillList.push_back(skill);
}

bool CSkillManager::DeleteSkill(const std::string& name)
{
	SkillInfo* skill = FindSkill(name);
	if (!skill)
	{
		return false;
	}
	auto	iter = m_mapSkill.begin();
	auto	iterEnd = m_mapSkill.end();
	for (; iter != iterEnd; ++iter)
	{
		if (iter->second->name == name)
		{
			SAFE_DELETE(iter->second);
			break;
		}
	}
	return true;
}
