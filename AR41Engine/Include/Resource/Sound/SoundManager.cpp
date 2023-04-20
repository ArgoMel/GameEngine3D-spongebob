#include "SoundManager.h"
#include "Sound.h"

CSoundManager::CSoundManager() 
	: m_System(nullptr)
	, m_MasterGroup(nullptr)
{
}

CSoundManager::~CSoundManager()
{
	m_mapSound.clear();
	auto	iter = m_mapChannelGroup.begin();
	auto	iterEnd = m_mapChannelGroup.end();
	for (; iter != iterEnd; ++iter)
	{
		iter->second->release();
	}
	if (m_System)
	{
		m_System->release();
		m_System->close();
	}
}

bool CSoundManager::Init()
{
	FMOD_RESULT result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
	{
		return false;
	}
	result = m_System->init(500, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		return false;
	}
	result = m_System->getMasterChannelGroup(&m_MasterGroup);
	if (result != FMOD_OK)
	{
		return false;
	}
	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));
	CreateSoundChannel("BGM");
	CreateSoundChannel("Effect");
	CreateSoundChannel("UI");
	CreateSoundChannel("Voice");
	CreateSoundChannel("Editor");
	return true;
}

void CSoundManager::Update()
{
	m_System->update();
}

bool CSoundManager::CreateSoundChannel(const std::string& name)
{
	if (FindChannelGroup(name))
	{
		return false;
	}
	FMOD::ChannelGroup* group = nullptr;
	FMOD_RESULT	result = m_System->createChannelGroup(name.c_str(), &group);
	if (result != FMOD_OK)
	{
		return false;
	}
	m_MasterGroup->addGroup(group, false);
	m_mapChannelGroup.insert(std::make_pair(name, group));
	return true;
}

bool CSoundManager::LoadSound(const std::string& groupName, const std::string& name,
	bool loop, const char* fileName, const std::string& pathName)
{
	CSound* sound = FindSound(name);
	if (sound)
	{
		return true;
	}
	FMOD::ChannelGroup* group = FindChannelGroup(groupName);
	if (!group)
	{
		return false;
	}
	sound = new CSound;
	sound->SetName(name);
	if (!sound->LoadSound(m_System, group, loop, fileName, pathName))
	{
		SAFE_DELETE(sound);
		return false;
	}
	sound->SetGroupName(groupName);
	m_mapSound.insert(std::make_pair(name, sound));
	return true;
}

bool CSoundManager::SetVolume(float volume)
{
	m_MasterGroup->setVolume(volume / 100.f);
	return true;
}

bool CSoundManager::SetVolume(const std::string& groupName, float volume)
{
	FMOD::ChannelGroup* group = FindChannelGroup(groupName);
	if (!group)
	{
		return false;
	}
	group->setVolume(volume / 100.f);
	return true;
}

bool CSoundManager::SoundPlay(const std::string& name)
{
	CSound* sound = FindSound(name);
	if (!sound)
	{
		return false;
	}
	sound->Play();
	return true;
}

bool CSoundManager::SoundStop(const std::string& name)
{
	CSound* sound = FindSound(name);
	if (!sound)
	{
		return false;
	}
	sound->Stop();
	return true;
}

bool CSoundManager::SoundPause(const std::string& name)
{
	CSound* sound = FindSound(name);
	if (!sound)
	{
		return false;
	}
	sound->Pause();
	return true;
}

bool CSoundManager::SoundResume(const std::string& name)
{
	CSound* sound = FindSound(name);
	if (!sound)
	{
		return false;
	}
	sound->Resume();
	return true;
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& name)
{
	auto	iter = m_mapChannelGroup.find(name);
	if (iter == m_mapChannelGroup.end())
	{
		return nullptr;
	}
	return iter->second;
}

CSound* CSoundManager::FindSound(const std::string& name)
{
	auto	iter = m_mapSound.find(name);
	if (iter == m_mapSound.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CSoundManager::ReleaseSound(const std::string& name)
{
	auto	iter = m_mapSound.find(name);
	if (iter == m_mapSound.end())
	{
		return;
	}
	if (iter->second->GetRefCount() == 1)
	{
		m_mapSound.erase(iter);
	}
}