#include "AnimationData.h"
#include "Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CAnimationData::CAnimationData()
	: m_Time(0.f)
	, m_FrameTime(0.f)
	, m_PlayTime(1.f)
	, m_PlayScale(1.f)
	, m_Loop(false)
	, m_Reverse(false)
	, m_Owner(nullptr)
{
}

CAnimationData::CAnimationData(const CAnimationData& anim)
{
	m_Name = anim.m_Name;
	m_SequenceName = anim.m_SequenceName;
	m_Sequence = anim.m_Sequence;
	m_Time = anim.m_Time;
	m_FrameTime = anim.m_FrameTime;
	m_PlayTime = anim.m_PlayTime;
	m_PlayScale = anim.m_PlayScale;
	m_Loop = anim.m_Loop;
	m_Reverse = anim.m_Reverse;
}

CAnimationData::~CAnimationData()
{
	size_t	size = m_vecNotify.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecNotify[i]);
	}
}

void CAnimationData::SetSequence(CAnimationSequence* sequence)
{
	if (sequence)
	{
		m_SequenceName = sequence->GetName();
	}
	m_Sequence = sequence;
}

void CAnimationData::Save(FILE* file)
{
	int	length = (int)m_Name.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_Name.c_str(), 1, length, file);
	length = (int)m_SequenceName.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_SequenceName.c_str(), 1, length, file);
	fwrite(&m_Time, 4, 1, file);
	fwrite(&m_FrameTime, 4, 1, file);
	fwrite(&m_PlayTime, 4, 1, file);
	fwrite(&m_PlayScale, 4, 1, file);
	fwrite(&m_Loop, 1, 1, file);
	fwrite(&m_Reverse, 1, 1, file);
}

void CAnimationData::Load(FILE* file)
{
	int	length = 0;
	char	name[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(name, 1, length, file);
	m_Name = name;
	length = 0;
	char	sequenceName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(sequenceName, 1, length, file);
	m_SequenceName = sequenceName;
	fread(&m_Time, 4, 1, file);
	fread(&m_FrameTime, 4, 1, file);
	fread(&m_PlayTime, 4, 1, file);
	fread(&m_PlayScale, 4, 1, file);
	fread(&m_Loop, 1, 1, file);
	fread(&m_Reverse, 1, 1, file);
	if (m_Owner->GetScene())
	{
		m_Sequence = m_Owner->GetScene()->GetResource()->FindAnimationSequence(sequenceName);
	}
	else
	{
		m_Sequence = CResourceManager::GetInst()->FindAnimationSequence(sequenceName);
	}
}

CAnimationData* CAnimationData::Clone()
{
	return new CAnimationData(*this);
}
