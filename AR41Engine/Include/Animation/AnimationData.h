#pragma once
#include "../EngineInfo.h"
#include "../Resource/Animation/AnimationSequence.h"

struct AnimationNotify
{
	std::string	name;
	int		frame;
	float	time;
	bool	call;
	std::function<void()>	function;
	AnimationNotify() 
		: frame(0)
		, time(0.f)
		, call(false)
	{
	}
};

class CAnimationData
{
private:
	friend class CAnimation;
	class CAnimation* m_Owner;
	CSharedPtr<CAnimationSequence>	m_Sequence;
	std::string	m_Name;
	std::string	m_SequenceName;
	float	m_Time;
	float	m_FrameTime;
	float	m_PlayTime;
	float	m_PlayScale;
	bool	m_Loop;
	bool	m_Reverse;
	std::function<void()>	m_EndFunction;
	std::vector<AnimationNotify*>	m_vecNotify;
	CAnimationData();
	CAnimationData(const CAnimationData& anim);
	~CAnimationData();
public:
	class CAnimationSequence* GetAnimationSequence()	const
	{
		return m_Sequence;
	}
	const std::string& GetName()	const
	{
		return m_Name;
	}
	const std::string& GetSequenceName()	const
	{
		return m_SequenceName;
	}
	float GetAnimationTime()	const
	{
		return m_Time;
	}
	float GetAnimationFrameTime()	const
	{
		return m_FrameTime;
	}
	float GetAnimationPlayTime()	const
	{
		return m_PlayTime;
	}
	float GetAnimationPlayScale()	const
	{
		return m_PlayScale;
	}
	bool GetAnimationLoop()	const
	{
		return m_Loop;
	}
	bool GetAnimationReverse()	const
	{
		return m_Reverse;
	}
	void SetSequence(CAnimationSequence* sequence);
	void Save(FILE* file);
	void Load(FILE* file);
	CAnimationData* Clone();
	template <typename T>
	void SetEndFunction(T* obj, void(T::* func)())
	{
		m_EndFunction = std::bind(func, obj);
	}
	template <typename T>
	void AddNotify(const std::string& name, int frame, T* obj, void(T::* func)())
	{
		AnimationNotify* notify = new AnimationNotify;
		notify->name = name;
		notify->frame = frame;
		notify->time = frame * m_FrameTime;
		notify->function = std::bind(func, obj);
		m_vecNotify.push_back(notify);
	}
	template <typename T>
	void AddNotify(const std::string& name, float time, T* obj, void(T::* func)())
	{
		AnimationNotify* notify = new AnimationNotify;
		notify->name = name;
		notify->frame = time / m_FrameTime;
		notify->time = time;
		notify->function = std::bind(func, obj);
		m_vecNotify.push_back(notify);
	}
};

