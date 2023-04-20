#pragma once
#include "../Ref.h"
#include "Animation2DData.h"

class CAnimation2D : public CRef
{
private:
	friend class CSpriteComponent;
	friend class CSceneManager;
	friend class CScene;
	static std::unordered_map<std::string, CAnimation2D*> m_mapAnimationCDO;
protected:
	class CSpriteComponent* m_Owner;
	CAnimation2DData* m_CurAnimation;
	std::string	m_ClassName;
	std::string	m_NullName;
	bool	m_Play;
	std::unordered_map<std::string, CAnimation2DData*>  m_mapAnimation;
	CAnimation2D();
	CAnimation2D(const CAnimation2D& anim);
	virtual ~CAnimation2D();
public:
	static void AddAnimationCDO(const std::string& name, CAnimation2D* CDO)
	{
		m_mapAnimationCDO.insert(std::make_pair(name, CDO));
	}
	static CAnimation2D* FindCDO(const std::string& name)
	{
		auto	iter = m_mapAnimationCDO.find(name);
		if (iter == m_mapAnimationCDO.end())
		{
			return nullptr;
		}
		return iter->second;
	}
	const std::string& GetCurrentAnimationName()	const
	{
		if (!m_CurAnimation)
		{
			return m_NullName;
		}
		return m_CurAnimation->GetName();
	}
	int GetCurrentFrame()	const
	{
		if (!m_CurAnimation)
		{
			return -1;
		}
		return m_CurAnimation->GetCurrentFrame();
	}
	float GetCurrentAnimationTime()	const
	{
		if (!m_CurAnimation)
		{
			return -1.f;
		}
		return m_CurAnimation->GetAnimationTime();
	}
	class CAnimationSequence2D* GetCurrentAnimationSequence()	const
	{
		if (!m_CurAnimation)
		{
			return nullptr;
		}
		return m_CurAnimation->GetAnimationSequence();
	}
	const std::string& GetAnimationClassName()	const
	{
		return m_ClassName;
	}
	void GetAnimationNames(std::vector<std::string>& vecNames)
	{
		auto	iter = m_mapAnimation.begin();
		auto	iterEnd = m_mapAnimation.end();
		for (; iter != iterEnd; ++iter)
		{
			vecNames.push_back(iter->first);
		}
	}
	CAnimation2DData* FindAnimation(const std::string& name);
	void DeleteAnimation(const std::string& name);
	class CScene* GetScene()	const;
	void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	bool AddAnimation(const std::string& name, const std::string& sequenceName, float playTime = 1.f, float playScale = 1.f, 
		bool loop = false, bool reverse = false);
	bool AddAnimation(const std::string& name, class CAnimationSequence2D* sequence, float playTime = 1.f, float playScale = 1.f,
		bool loop = false, bool reverse = false);
	void SetPlayTime(const std::string& name, float playTime);
	void SetPlayScale(const std::string& name, float playScale);
	void SetLoop(const std::string& name, bool loop);
	void SetReverse(const std::string& name, bool reverse);
	void SetCurrentAnimation(const std::string& name);
	void ChangeAnimation(const std::string& name);
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual CAnimation2D* Clone();
	void SetShader();
	template <typename T>
	void SetCurrentEndFunction(const std::string& animationName, T* obj, void(T::* func)())
	{
		CAnimation2DData* animation = FindAnimation(animationName);
		if (!animation)
		{
			return;
		}
		animation->SetEndFunction<T>(obj, func);
	}
	template <typename T>
	void AddCurrentNotify(const std::string& animationName, const std::string& name, int frame, T* obj, void(T::* func)())
	{
		CAnimation2DData* animation = FindAnimation(animationName);
		if (!animation)
		{
			return;
		}
		animation->AddNotify<T>(name, frame, obj, func);
	}
	template <typename T>
	void AddCurrentNotify(const std::string& animationName, const std::string& name, float time, T* obj, void(T::* func)())
	{
		CAnimation2DData* animation = FindAnimation(animationName);
		if (!animation)
		{
			return;
		}
		animation->AddNotify<T>(name, time, obj, func);
	}
};

