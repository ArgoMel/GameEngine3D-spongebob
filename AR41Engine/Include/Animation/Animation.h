#pragma once
#include "../Ref.h"
#include "AnimationData.h"

class CAnimation : public CRef
{
private:
	friend class CAnimationMeshComponent;
	friend class CSceneManager;
	friend class CScene;
	static std::unordered_map<std::string, CAnimation*> m_mapAnimationCDO;
protected:
	class CAnimationMeshComponent* m_Owner;
	class CAnimationUpdateConstantBuffer* m_AnimationUpdateCBuffer;
//접근 못하는(outputbuffer)읽기전용버퍼 떄문에 본버퍼 만들고 새로 복사해서 본버퍼에 접근
	class CStructuredBuffer* m_OutputBuffer;
	class CStructuredBuffer* m_BoneBuffer;
	class CStructuredBuffer* m_InstancingBoneBuffer;
	CSharedPtr<class CComputeShader>	m_AnimationUpdateShader;
	CSharedPtr<class CSkeleton>	m_Skeleton;
	CAnimationData* m_CurAnimation;
	CAnimationData* m_ChangeAnimation;
	std::string	m_ClassName;
	std::string	m_NullName;
	Vector3		m_BlendScale;
	Vector3		m_BlendPos;
	Vector4		m_BlendRot;
	bool		m_Play;
	float		m_GlobalTime;
	float		m_SequenceProgress;
	float		m_ChangeTimeAcc;
	float		m_ChangeTime;
	std::unordered_map<std::string, CAnimationData*>  m_mapAnimation;
	std::vector<OutputBoneInfo>		m_vecBoneInfo;
	CAnimation();
	CAnimation(const CAnimation& anim);
	virtual ~CAnimation();
public:
	static void AddAnimationCDO(const std::string& name, CAnimation* CDO)
	{
		m_mapAnimationCDO.insert(std::make_pair(name, CDO));
	}
	static CAnimation* FindCDO(const std::string& name)
	{
		auto	iter = m_mapAnimationCDO.find(name);
		if (iter == m_mapAnimationCDO.end())
		{
			return nullptr;
		}
		return iter->second;
	}
	class CScene* GetScene()	const;
	CAnimationData* GetCurrentAnimation()	const
	{
		return m_CurAnimation;
	}
	const std::string& GetCurrentAnimationName()	const
	{
		if (!m_CurAnimation)
		{
			return m_NullName;
		}
		return m_CurAnimation->GetName();
	}
	float GetCurrentAnimationTime()	const
	{
		if (!m_CurAnimation)
		{
			return -1.f;
		}
		return m_CurAnimation->GetAnimationTime();
	}
	class CAnimationSequence* GetCurrentAnimationSequence()	const
	{
		if (!m_CurAnimation)
		{
			return nullptr;
		}
		return m_CurAnimation->GetAnimationSequence();
	}
	CAnimationData* GetChangeAnimation()	const
	{
		return m_ChangeAnimation;
	}
	const std::string& GetAnimationClassName()	const
	{
		return m_ClassName;
	}
	const Vector3& GetBlendScale()	const
	{
		return m_BlendScale;
	}
	const Vector3& GetBlendPos()	const
	{
		return m_BlendPos;
	}
	const Vector4& GetBlendRot()	const
	{
		return m_BlendRot;
	}
	bool GetAnimationPlay()	const
	{
		return m_Play;
	}
	float GetAnimationGlobalTime()	const
	{
		return m_GlobalTime;
	}
	float GetAnimationSequenceProgress()	const
	{
		return m_SequenceProgress;
	}
	float GetAnimationChangeTimeAcc()	const
	{
		return m_ChangeTimeAcc;
	}
	float GetAnimationChangeTime()	const
	{
		return m_ChangeTime;
	}
	int GetAnimationCount()	const
	{
		return (int)m_mapAnimation.size();
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
	void SetInstancingBoneBuffer(class CStructuredBuffer* buffer)
	{
		m_InstancingBoneBuffer = buffer;
	}
	void SetSkeleton(class CSkeleton* skeleton);
	void Play()
	{
		m_Play = true;
	}
	void Stop()
	{
		m_Play = false;
	}
	bool IsPlay()	const
	{
		return m_Play;
	}
	void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	bool AddAnimation(const std::string& name, const std::string& sequenceName, float playTime = 1.f, 
		float playScale = 1.f, bool loop = false, bool reverse = false);
	bool AddAnimation(const std::string& name, class CAnimationSequence* sequence, float playTime = 1.f, 
		float playScale = 1.f, bool loop = false, bool reverse = false);
	void SetPlayTime(const std::string& name, float playTime);
	void SetPlayScale(const std::string& name, float playScale);
	void SetLoop(const std::string& name, bool loop);
	void SetReverse(const std::string& name, bool reverse);
	void SetCurrentAnimation(const std::string& name);
	void ChangeAnimation(const std::string& name);
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual CAnimation* Clone();
	void SetShader();
	void ResetShader();
	CAnimationData* FindAnimation(const std::string& name);
	template <typename T>
	void SetCurrentEndFunction(const std::string& animationName, T* obj, void(T::* func)())
	{
		CAnimationData* animation = FindAnimation(animationName);
		if (!animation)
		{
			return;
		}
		animation->SetEndFunction<T>(obj, func);
	}
	template <typename T>
	void AddCurrentNotify(const std::string& animationName, const std::string& name, int frame, T* obj, void(T::* func)())
	{
		CAnimationData* animation = FindAnimation(animationName);
		if (!animation)
		{
			return;
		}
		animation->AddNotify<T>(name, frame, obj, func);
	}
	template <typename T>
	void AddCurrentNotify(const std::string& animationName, const std::string& name, float time, T* obj, void(T::* func)())
	{
		CAnimationData* animation = FindAnimation(animationName);
		if (!animation)
		{
			return;
		}
		animation->AddNotify<T>(name, time, obj, func);
	}
};

