#pragma once
#include "EngineInfo.h"
#include "Component/Component.h"

struct SkillInfo
{
	void* owner;
	class CGameObject* obj;
	std::function<void(CGameObject*)>	func;
	std::string	name;
	float skillTime;
	float skillMaxTime;

	SkillInfo()
		: owner(nullptr)
		, obj(nullptr)
		, skillTime(0.f)
		, skillMaxTime(0.f)
	{
	}
};

class CSkillManager
{
	DECLARE_SINGLE(CSkillManager)
private:
	std::unordered_map<std::string, SkillInfo*>		m_mapSkill;
	std::list<SkillInfo*>	m_SkillList;
public:
	bool Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	SkillInfo* FindSkill(const std::string& name);
	void ActivateSkill(const std::string& name, class CGameObject* obj);
	bool DeleteSkill(const std::string& name);
	template <typename T>
	void CreateSkill(const std::string& name,T* owner, void (T::* func)(CGameObject*),float time=0.f)
	{
		SkillInfo* skill = FindSkill(name);
		if (skill)
		{
			return;
		}
		skill = new SkillInfo;
		skill->owner = owner;
		skill->obj = nullptr;
		skill->name = name;
		// 멤버함수를 등록할때 함수주소, 객체주소를 등록해야 한다.
		skill->func = std::bind(func, owner, std::placeholders::_1);
		skill->skillTime = 0.f;
		skill->skillMaxTime = time;
		m_mapSkill.insert(std::make_pair(name, skill));
	}
};