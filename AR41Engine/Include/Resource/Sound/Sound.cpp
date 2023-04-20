#include "Sound.h"
#include "../../PathManager.h"

CSound::CSound() 
	: m_System(nullptr)
	, m_Sound(nullptr)
	, m_Group(nullptr)
	, m_Channel(nullptr)
	, m_Play(false)
	, m_Loop(false)
	, m_Pause(false)
	, m_FileName{}
	, m_PathName{}
{
}

CSound::~CSound()
{
	if (m_Sound)
	{
		m_Sound->release();
	}
}

bool CSound::LoadSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop,
	const char* fileName, const std::string& pathName)
{
	m_System = system;
	m_Group = group;
	m_Loop = loop;
	strcpy_s(m_FileName, fileName);
	strcpy_s(m_PathName, pathName.c_str());
	char	fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	if (info)
	{
		strcpy_s(fullPath, info->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	FMOD_MODE	mode = FMOD_DEFAULT;
	if (loop)
	{
		mode = FMOD_LOOP_NORMAL;
	}
	if (m_System->createSound(fullPath, mode, nullptr, &m_Sound) != FMOD_OK)
	{
		return false;
	}
	return true;
}

void CSound::Play()
{
	m_System->playSound(m_Sound, m_Group, false, &m_Channel);
	m_Play = true;
	m_Pause = false;
}

void CSound::Stop()
{
	if (m_Channel)
	{
		bool	playing = false;
		m_Channel->isPlaying(&playing);
		if (playing)
		{
			m_Channel->stop();
			m_Channel = nullptr;
			m_Play = false;
			m_Pause = false;
		}
	}
}

void CSound::Pause()
{
	if (m_Channel)
	{
		bool	playing = false;
		m_Channel->isPlaying(&playing);
		if (playing)
		{
			m_Channel->setPaused(true);
		}
		m_Play = false;
		m_Pause = true;
	}
}

void CSound::Resume()
{
	if (m_Channel)
	{
		if (m_Pause)
		{
			m_Channel->setPaused(false);
			m_Play = true;
			m_Pause = false;
		}
	}
}

void CSound::Save(FILE* file)
{
	fwrite(&m_Loop, sizeof(bool), 1, file);
	int	length = (int)m_GroupName.length();
	fwrite(&length, sizeof(int), 1, file);
	fwrite(m_GroupName.c_str(), 1, length, file);
	fwrite(m_FileName, sizeof(char), MAX_PATH, file);
	fwrite(m_PathName, sizeof(char), MAX_PATH, file);
}

void CSound::Load(FILE* file)
{
}

