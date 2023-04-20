#pragma once
#include "../EngineInfo.h"
#include "../Resource/Animation/AnimationSequence2D.h"

struct Animation2DNotify
{
	std::string	name;
	std::function<void()>	function;
	int		frame;
	float	time;
	bool	call;
	Animation2DNotify()
		: frame(0)
		, time(0.f)
		, call(false)
	{
	}
};

class CAnimation2DData
{
private:
	friend class CAnimation2D;
	class CAnimation2D* m_Owner;
	CSharedPtr<CAnimationSequence2D>	m_Sequence;
	std::string	m_Name;
	std::string	m_SequenceName;
	std::function<void()>	m_EndFunction;
	std::vector<Animation2DNotify*>	m_vecNotify;
	int		m_Frame;
	float	m_Time;
	float	m_FrameTime;
	float	m_PlayTime;
	float	m_PlayScale;
	bool	m_Loop;
	bool	m_Reverse;
	CAnimation2DData();
	CAnimation2DData(const CAnimation2DData& anim);
	~CAnimation2DData();
public:
	class CAnimationSequence2D* GetAnimationSequence()	const
	{
		return m_Sequence;
	}
	const std::string& GetName()	const
	{
		return m_Name;
	}
	int GetCurrentFrame()	const
	{
		return m_Frame;
	}
	float GetAnimationTime()	const
	{
		return m_Time;
	}
	void SetSequence(CAnimationSequence2D* sequence);
	void Save(FILE* file);
	void Load(FILE* file);
	CAnimation2DData* Clone();
	template <typename T>
	void SetEndFunction(T* obj, void(T::* func)())
	{
		m_EndFunction = std::bind(func, obj);
	}
	template <typename T>
	void AddNotify(const std::string& name, int frame, T* obj, void(T::* func)())
	{
		Animation2DNotify* notify = new Animation2DNotify;
		notify->name = name;
		notify->frame = frame;
		notify->time = frame * m_FrameTime;
		notify->function = std::bind(func, obj);
		m_vecNotify.push_back(notify);
	}
	template <typename T>
	void AddNotify(const std::string& name, float time, T* obj, void(T::* func)())
	{
		Animation2DNotify* notify = new Animation2DNotify;
		notify->name = name;
		notify->frame = time / m_FrameTime;
		notify->time = time;
		notify->function = std::bind(func, obj);
		m_vecNotify.push_back(notify);
	}
};

