#include "Animation.h"
#include "../Device.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/AnimationUpdateConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/Shader/AnimationUpdateShader.h"

std::unordered_map<std::string, CAnimation*> CAnimation::m_mapAnimationCDO;

CAnimation::CAnimation()
	: m_Owner(nullptr)
	, m_Play(true)
	, m_CurAnimation(nullptr)
	, m_ChangeAnimation(nullptr)
	, m_InstancingBoneBuffer(nullptr)
	, m_GlobalTime(0.f)
	, m_SequenceProgress(0.f)
	, m_ChangeTimeAcc(0.f)
	, m_ChangeTime(0.2f)
{
	m_ClassName = "Animation";
	SetTypeID<CAnimation>();
	m_AnimationUpdateCBuffer = new CAnimationUpdateConstantBuffer;
	m_OutputBuffer = new CStructuredBuffer;
	m_BoneBuffer = new CStructuredBuffer;
}

CAnimation::CAnimation(const CAnimation& anim)
	: CRef(anim)
	, m_Owner(anim.m_Owner)
{
	m_Play = anim.m_Play;
	m_GlobalTime = 0.f;
	m_SequenceProgress = 0.f;
	m_ChangeTimeAcc = 0.f;
	m_ChangeTime = anim.m_ChangeTime;
	m_AnimationUpdateShader = anim.m_AnimationUpdateShader;
	if (anim.m_AnimationUpdateCBuffer)
	{
		m_AnimationUpdateCBuffer = anim.m_AnimationUpdateCBuffer->Clone();
	}
	if (anim.m_OutputBuffer)
	{
		m_OutputBuffer = anim.m_OutputBuffer->Clone();
	}
	if (anim.m_BoneBuffer)
	{
		m_BoneBuffer = anim.m_BoneBuffer->Clone();
	}
	m_Skeleton = anim.m_Skeleton;
	m_mapAnimation.clear();
	auto	iter = anim.m_mapAnimation.begin();
	auto	iterEnd = anim.m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		CAnimationData* data = iter->second->Clone();
		if (data->GetName() == anim.m_CurAnimation->GetName())
		{
			m_CurAnimation = data;
		}
		m_mapAnimation.insert(std::make_pair(iter->first, data));
	}
}

CAnimation::~CAnimation()
{
	SAFE_DELETE(m_OutputBuffer);
	SAFE_DELETE(m_BoneBuffer);
	SAFE_DELETE(m_AnimationUpdateCBuffer);
	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

CScene* CAnimation::GetScene() const
{
	if (!m_Owner)
	{
		return nullptr;
	}
	return m_Owner->GetScene();
}

void CAnimation::SetSkeleton(CSkeleton* skeleton)
{
	m_Skeleton = skeleton;
}

void CAnimation::Start()
{
	m_AnimationUpdateShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("AnimationUpdateShader");

	m_AnimationUpdateCBuffer->Init();
	m_AnimationUpdateCBuffer->SetBoneCount((int)m_Skeleton->GetBoneCount());
	m_vecBoneInfo.resize(m_Skeleton->GetBoneCount());

	m_OutputBuffer->Init("OutputBone", sizeof(Matrix), (unsigned int)m_Skeleton->GetBoneCount(), 0);

	m_BoneBuffer->Init("OutputBone", sizeof(OutputBoneInfo), (unsigned int)m_Skeleton->GetBoneCount(), 1,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, D3D11_CPU_ACCESS_READ);
}

bool CAnimation::Init()
{
	return true;
}

void CAnimation::Update(float deltaTime)
{
	if (!m_Play || !m_CurAnimation || !m_Skeleton || m_CurAnimation->m_Sequence->GetKeyFrameCount() == 0)
	{
		return;
	}
	m_GlobalTime += deltaTime * m_CurAnimation->m_PlayScale;
	bool	change = false;
	bool	changeEnd = false;
	m_CurAnimation->GetAnimationSequence()->SetEnd(false);
	if (m_ChangeAnimation)
	{
		change = true;
		m_ChangeTimeAcc += deltaTime;
		if (m_ChangeTimeAcc >= m_ChangeTime)
		{
			m_ChangeTimeAcc = m_ChangeTime;
			changeEnd = true;
		}
	}
	else
	{
		m_SequenceProgress = m_GlobalTime / m_CurAnimation->m_PlayTime;
		if (m_GlobalTime >= m_CurAnimation->m_PlayTime)
		{
			m_GlobalTime = m_CurAnimation->m_PlayTime;
			m_CurAnimation->GetAnimationSequence()->SetEnd(true);
		}
		m_AnimationUpdateCBuffer->SetChangeAnimation(false);
		m_AnimationUpdateCBuffer->SetChangeRatio(0.f);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(0);
		float	animationTime = m_GlobalTime + m_CurAnimation->m_Sequence->m_StartTime;
		int	startFrame = 0;
		int	endFrame = m_CurAnimation->m_Sequence->m_FrameLength;
		int	frameIndex = (int)(m_GlobalTime / m_CurAnimation->m_Sequence->m_FrameTime);
		int	nextFrameIndex = frameIndex + 1;
		if (frameIndex >= endFrame)
		{
			frameIndex = endFrame - 1;
		}
		if (nextFrameIndex >= endFrame)
		{
			nextFrameIndex = endFrame - 1;
		}
		float	ratio = (m_GlobalTime - m_CurAnimation->m_Sequence->m_FrameTime * frameIndex) / 
			m_CurAnimation->m_Sequence->m_FrameTime;
		m_AnimationUpdateCBuffer->SetFrameCount(endFrame);
		m_AnimationUpdateCBuffer->SetCurrentFrame(frameIndex);
		m_AnimationUpdateCBuffer->SetNextFrame(nextFrameIndex);
		m_AnimationUpdateCBuffer->SetRatio(ratio);
		size_t	size = m_CurAnimation->m_vecNotify.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (!m_CurAnimation->m_vecNotify[i]->call && m_CurAnimation->m_vecNotify[i]->frame == frameIndex)
			{
				m_CurAnimation->m_vecNotify[i]->call = true;
				m_CurAnimation->m_vecNotify[i]->function();
			}
		}
		if (m_CurAnimation->GetAnimationSequence()->IsSequenceEnd())
		{
			if (m_CurAnimation->m_EndFunction)
			{
				m_CurAnimation->m_EndFunction();
			}
			if (m_CurAnimation->m_Loop)
			{
				m_GlobalTime = 0.f;
				size_t	size = m_CurAnimation->m_vecNotify.size();
				for (size_t i = 0; i < size; ++i)
				{
					m_CurAnimation->m_vecNotify[i]->call = false;
				}
			}
		}
	}
	if (change)
	{
		m_AnimationUpdateCBuffer->SetChangeRatio(m_ChangeTimeAcc / m_ChangeTime);
		m_AnimationUpdateCBuffer->SetChangeAnimation(true);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(m_ChangeAnimation->m_Sequence->m_FrameLength);
		m_ChangeAnimation->m_Sequence->SetChangeShader();
	}
	m_AnimationUpdateCBuffer->SetRowIndex(m_Owner->GetInstanceID());
	m_OutputBuffer->SetShader();
	m_AnimationUpdateCBuffer->UpdateBuffer();
	m_Skeleton->SetShader();
	m_BoneBuffer->SetShader();
	m_InstancingBoneBuffer->SetShader();
	m_CurAnimation->m_Sequence->SetShader();
	unsigned int groupX = (unsigned int)m_Skeleton->GetBoneCount() / 256 + 1;
	m_AnimationUpdateShader->Excute(groupX, 1, 1);
	m_InstancingBoneBuffer->ResetShader();
	m_BoneBuffer->ResetShader();
	m_CurAnimation->m_Sequence->ResetShader();
	m_Skeleton->ResetShader();
	m_OutputBuffer->ResetShader();
	if (change)
	{
		m_ChangeAnimation->m_Sequence->ResetChangeShader();
	}
	if (changeEnd)
	{
		m_CurAnimation = m_ChangeAnimation;
		m_ChangeAnimation = nullptr;
		m_ChangeTimeAcc = 0.f;
		m_GlobalTime = 0.f;
	}

	// 구조화 버퍼에 있는 본 정보를 DataBuffer로 복사한다.
	// 리소스를 복사가능하지만 성능이 구려져서 주석처리
	//CDevice::GetInst()->GetContext()->CopyResource(m_BoneDataBuffer, m_BoneBuffer->GetBuffer());

	//본행렬은 무기 집을거 아니면 필요없다
	D3D11_MAPPED_SUBRESOURCE	map = {};
	CDevice::GetInst()->GetContext()->Map(m_BoneBuffer->GetBuffer(), 0, D3D11_MAP_READ, 0, &map);
	memcpy(&m_vecBoneInfo[0], map.pData, sizeof(OutputBoneInfo) * m_vecBoneInfo.size());
	CDevice::GetInst()->GetContext()->Unmap(m_BoneBuffer->GetBuffer(), 0);
	m_Skeleton->Update(deltaTime, m_vecBoneInfo, m_Owner);
}

bool CAnimation::AddAnimation(const std::string& name, const std::string& sequenceName, float playTime, float playScale, 
	bool loop, bool reverse)
{
	CAnimationData* anim = FindAnimation(name);
	if (anim)
	{
		return false;
	}
	CAnimationSequence* sequence = nullptr;
	if (m_Owner->GetScene())
	{
		sequence = m_Owner->GetScene()->GetResource()->FindAnimationSequence(sequenceName);
	}
	else
	{
		sequence = CResourceManager::GetInst()->FindAnimationSequence(sequenceName);
	}
	anim = new CAnimationData;
	anim->m_Sequence = sequence;
	anim->m_SequenceName = sequenceName;
	anim->m_Name = name;
	anim->m_PlayTime = playTime;
	anim->m_PlayScale = playScale;
	anim->m_Loop = loop;
	anim->m_Reverse = reverse;
	anim->m_FrameTime = playTime / sequence->GetKeyFrameCount();
	anim->m_Owner = this;
	if (m_mapAnimation.empty())
	{
		m_CurAnimation = anim;
	}
	m_mapAnimation.insert(std::make_pair(name, anim));
	return true;
}

bool CAnimation::AddAnimation(const std::string& name, CAnimationSequence* sequence, float playTime, float playScale, 
	bool loop, bool reverse)
{
	CAnimationData* anim = FindAnimation(name);
	if (anim)
	{
		return false;
	}
	anim = new CAnimationData;
	anim->m_Sequence = sequence;
	anim->m_SequenceName = sequence->GetName();
	anim->m_Name = name;
	anim->m_PlayTime = playTime;
	anim->m_PlayScale = playScale;
	anim->m_Loop = loop;
	anim->m_Reverse = reverse;
	anim->m_FrameTime = playTime / sequence->GetKeyFrameCount();
	anim->m_Owner = this;
	if (m_mapAnimation.empty())
	{
		m_CurAnimation = anim;
	}
	m_mapAnimation.insert(std::make_pair(name, anim));
	return true;
}

void CAnimation::SetPlayTime(const std::string& name, float playTime)
{
	CAnimationData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	anim->m_PlayTime = playTime;
}

void CAnimation::SetPlayScale(const std::string& name, float playScale)
{
	CAnimationData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	anim->m_PlayScale = playScale;
}

void CAnimation::SetLoop(const std::string& name, bool loop)
{
	CAnimationData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	anim->m_Loop = loop;
}

void CAnimation::SetReverse(const std::string& name, bool reverse)
{
	CAnimationData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	anim->m_Reverse = reverse;
}

void CAnimation::SetCurrentAnimation(const std::string& name)
{
	CAnimationData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	m_CurAnimation = anim;
	m_CurAnimation->m_Time = 0.f;
	m_GlobalTime = 0.f;
	size_t	size = m_CurAnimation->m_vecNotify.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->call = false;
	}
}

void CAnimation::ChangeAnimation(const std::string& name)
{
	if (m_CurAnimation->GetName() == name)
	{
		return;
	}
	m_ChangeAnimation = FindAnimation(name);
	if (!m_ChangeAnimation)
	{
		return;
	}
	size_t	size = m_CurAnimation->m_vecNotify.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->call = false;
	}
	m_CurAnimation->m_Time = 0.f;
}

void CAnimation::Save(FILE* file)
{
	CRef::Save(file);
	int	count = (int)m_mapAnimation.size();
	fwrite(&count, 4, 1, file);
	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		iter->second->Save(file);
	}
	int	length = (int)m_CurAnimation->GetName().length();
	fwrite(&length, 4, 1, file);
	fwrite(m_CurAnimation->GetName().c_str(), 1, length, file);
}

void CAnimation::Load(FILE* file)
{
	CRef::Load(file);
	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapAnimation.clear();
	m_CurAnimation = nullptr;
	int	count = 0;
	fread(&count, 4, 1, file);
	for (int i = 0; i < count; ++i)
	{
		CAnimationData* data = new CAnimationData;
		data->m_Owner = this;
		data->Load(file);
		m_mapAnimation.insert(std::make_pair(data->GetName(), data));
	}
	int	length = 0;
	char	curName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(curName, 1, length, file);
	m_CurAnimation = FindAnimation(curName);
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::SetShader()
{
	m_OutputBuffer->SetShader(11, (int)EShaderBufferType::Vertex);
}

void CAnimation::ResetShader()
{
	m_OutputBuffer->ResetShader(11, (int)EShaderBufferType::Vertex);
}

CAnimationData* CAnimation::FindAnimation(const std::string& name)
{
	auto	iter = m_mapAnimation.find(name);
	if (iter == m_mapAnimation.end())
	{
		return nullptr;
	}
	return iter->second;
}
