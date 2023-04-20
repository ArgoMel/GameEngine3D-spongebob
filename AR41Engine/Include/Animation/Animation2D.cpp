#include "Animation2D.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
std::unordered_map<std::string, CAnimation2D*> CAnimation2D::m_mapAnimationCDO;

CAnimation2D::CAnimation2D()
	: m_Owner(nullptr)
	, m_Play(true)
{
	m_ClassName = "Animation2D";
}

CAnimation2D::CAnimation2D(const CAnimation2D& anim)
	: CRef(anim)
	, m_Owner(anim.m_Owner)
	, m_CurAnimation(anim.m_CurAnimation)
{
	m_Play = anim.m_Play;
	m_mapAnimation.clear();
	auto	iter = anim.m_mapAnimation.begin();
	auto	iterEnd = anim.m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		CAnimation2DData* data = iter->second->Clone();
		if (data->GetName() == anim.m_CurAnimation->GetName())
		{
			m_CurAnimation = data;
		}
		m_mapAnimation.insert(std::make_pair(iter->first, data));
	}
}

CAnimation2D::~CAnimation2D()
{
	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

CAnimation2DData* CAnimation2D::FindAnimation(const std::string& name)
{
	auto	iter = m_mapAnimation.find(name);
	if (iter == m_mapAnimation.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CAnimation2D::DeleteAnimation(const std::string& name)
{
	auto	iter = m_mapAnimation.find(name);
	if (iter == m_mapAnimation.end())
	{
		return;
	}
	SAFE_DELETE(iter->second);
	//m_mapAnimation.erase(name);
}

CScene* CAnimation2D::GetScene() const
{
	if (!m_Owner)
	{
		return nullptr;
	}
	return m_Owner->GetScene();
}

void CAnimation2D::Start()
{
	if (m_Owner && m_CurAnimation)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
	}
}

bool CAnimation2D::Init()
{
	return true;
}

void CAnimation2D::Update(float deltaTime)
{
	if (!m_Play || !m_CurAnimation || m_CurAnimation->m_Sequence->GetFrameCount() == 0)
	{
		return;
	}
	m_CurAnimation->m_Time += deltaTime * m_CurAnimation->m_PlayScale;
	bool	animEnd = false;
	m_CurAnimation->m_FrameTime = m_CurAnimation->m_PlayTime / m_CurAnimation->m_Sequence->GetFrameCount();
	if (m_CurAnimation->m_Time >= m_CurAnimation->m_FrameTime)
	{
		m_CurAnimation->m_Time -= m_CurAnimation->m_FrameTime;
		if (m_CurAnimation->m_Reverse)
		{
			--m_CurAnimation->m_Frame;
			if (m_CurAnimation->m_Frame < 0)
			{
				animEnd = true;
			}
		}
		else
		{
			++m_CurAnimation->m_Frame;
			if (m_CurAnimation->m_Frame == m_CurAnimation->m_Sequence->GetFrameCount())
			{
				animEnd = true;
			}
		}
	}
	size_t	size = m_CurAnimation->m_vecNotify.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (!m_CurAnimation->m_vecNotify[i]->call && m_CurAnimation->m_vecNotify[i]->frame == m_CurAnimation->m_Frame)
		{
			m_CurAnimation->m_vecNotify[i]->call = true;
			m_CurAnimation->m_vecNotify[i]->function();
		}
	}
	if (animEnd)
	{
		if (m_CurAnimation->m_Loop)
		{
			if (m_CurAnimation->m_Reverse)
			{
				m_CurAnimation->m_Frame = m_CurAnimation->m_Sequence->GetFrameCount() - 1;
			}
			else
			{
				m_CurAnimation->m_Frame = 0;
			}
			size = m_CurAnimation->m_vecNotify.size();
			for (size_t i = 0; i < size; ++i)
			{
				m_CurAnimation->m_vecNotify[i]->call = false;
			}
		}
		else
		{
			if (m_CurAnimation->m_Reverse)
			{
				m_CurAnimation->m_Frame = 0;
			}
			else
			{
				m_CurAnimation->m_Frame = m_CurAnimation->m_Sequence->GetFrameCount() - 1;
			}
		}
		if (m_CurAnimation->m_EndFunction)
		{
			m_CurAnimation->m_EndFunction();
		}
	}
}

bool CAnimation2D::AddAnimation(const std::string& name, const std::string& sequenceName, float playTime, float playScale, 
	bool loop, bool reverse)
{
	CAnimation2DData* anim = FindAnimation(name);
	if (anim)
	{
		return false;
	}
	CAnimationSequence2D* sequence = nullptr;
	if (m_Owner->GetScene())
	{
		sequence = m_Owner->GetScene()->GetResource()->FindAnimationSequence2D(sequenceName);
	}
	else
	{
		sequence = CResourceManager::GetInst()->FindAnimationSequence2D(sequenceName);
	}
	if(!sequence)
	{
		CResourceManager::GetInst()->CreateAnimationSequence2D(sequenceName, "Temp", TEXT("neutral.png"));
		sequence = CResourceManager::GetInst()->FindAnimationSequence2D(sequenceName);
	}
	anim = new CAnimation2DData;
	anim->m_Sequence = sequence;
	anim->m_SequenceName = sequenceName;
	anim->m_Name = name;
	anim->m_PlayTime = playTime;
	anim->m_PlayScale = playScale;
	anim->m_Loop = loop;
	anim->m_Reverse = reverse;
	anim->m_FrameTime = playTime / sequence->GetFrameCount();
	anim->m_Owner = this;
	if (m_mapAnimation.empty())
	{
		m_CurAnimation = anim;
		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		}
	}
	m_mapAnimation.insert(std::make_pair(name, anim));
	return true;
}

bool CAnimation2D::AddAnimation(const std::string& name, CAnimationSequence2D* sequence, float playTime, float playScale,
	bool loop, bool reverse)
{
	CAnimation2DData* anim = FindAnimation(name);
	if (anim)
	{
		return false;
	}
	anim = new CAnimation2DData;
	anim->m_Sequence = sequence;
	anim->m_SequenceName = sequence->GetName();
	anim->m_Name = name;
	anim->m_PlayTime = playTime;
	anim->m_PlayScale = playScale;
	anim->m_Loop = loop;
	anim->m_Reverse = reverse;
	anim->m_FrameTime = playTime / sequence->GetFrameCount();
	anim->m_Owner = this;
	if (m_mapAnimation.empty())
	{
		m_CurAnimation = anim;
		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		}
	}
	m_mapAnimation.insert(std::make_pair(name, anim));
	return true;
}

void CAnimation2D::SetPlayTime(const std::string& name, float playTime)
{
	CAnimation2DData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	anim->m_PlayTime = playTime;
}

void CAnimation2D::SetPlayScale(const std::string& name, float playScale)
{
	CAnimation2DData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	anim->m_PlayScale = playScale;
}

void CAnimation2D::SetLoop(const std::string& name, bool loop)
{
	CAnimation2DData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	anim->m_Loop = loop;
}

void CAnimation2D::SetReverse(const std::string& name, bool reverse)
{
	CAnimation2DData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	anim->m_Reverse = reverse;
}

void CAnimation2D::SetCurrentAnimation(const std::string& name)
{
	CAnimation2DData* anim = FindAnimation(name);
	if (!anim)
	{
		return;
	}
	m_CurAnimation = anim;
	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;
	size_t	size = m_CurAnimation->m_vecNotify.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->call = false;
	}
	if (m_Owner)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		m_Owner->SetTextureFrameIndex(0);
	}
}

void CAnimation2D::ChangeAnimation(const std::string& name)
{
	if (m_CurAnimation->GetName() == name)
	{
		return;
	}
	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;
	size_t	size = m_CurAnimation->m_vecNotify.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->call = false;
	}
	m_CurAnimation = FindAnimation(name);
	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;
	if (m_Owner)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		m_Owner->SetTextureFrameIndex(0);
	}
}

void CAnimation2D::Save(FILE* file)
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

void CAnimation2D::Load(FILE* file)
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
		CAnimation2DData* data = new CAnimation2DData;
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

CAnimation2D* CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}

void CAnimation2D::SetShader()
{
	if (!m_CurAnimation || !m_CurAnimation->m_Sequence || !m_CurAnimation->m_Sequence->GetTexture())
	{
		return;
	}
	CAnimation2DConstantBuffer* buffer = CResourceManager::GetInst()->GetAnim2DConstantBuffer();
	const Animation2DFrameData& frameData = m_CurAnimation->m_Sequence->GetFrameData(m_CurAnimation->m_Frame);
	EAnimation2DType	type = m_CurAnimation->m_Sequence->GetAnim2DType();
	if (type == EAnimation2DType::Atlas)
	{
		buffer->SetImageSize((float)m_CurAnimation->m_Sequence->GetTexture()->GetWidth(), 
			(float)m_CurAnimation->m_Sequence->GetTexture()->GetHeight());
		buffer->SetImageFrame(frameData.start, frameData.end);
	}
	else if (type == EAnimation2DType::Frame)
	{
		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
			m_Owner->SetTextureFrameIndex(m_CurAnimation->m_Frame);
		}
		else
		{
			m_CurAnimation->m_Sequence->GetTexture()->SetShader(0, (int)EShaderBufferType::Pixel, m_CurAnimation->m_Frame);
		}
	}
	else
	{
		buffer->SetFrame(m_CurAnimation->m_Frame);
	}
	buffer->SetImageType(m_CurAnimation->m_Sequence->GetAnim2DType());
	buffer->SetAnim2DEnable(true);
	buffer->UpdateBuffer();
}
