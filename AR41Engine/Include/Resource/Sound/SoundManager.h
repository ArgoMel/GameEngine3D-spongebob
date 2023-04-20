#pragma once
#include "../../EngineInfo.h"

class CSoundManager
{
private:
	friend class CResourceManager;
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, CSharedPtr<class CSound>>	m_mapSound;
	std::unordered_map<std::string, FMOD::ChannelGroup*>	m_mapChannelGroup;
	CSoundManager();
	~CSoundManager();
public:
	bool Init();
	void Update();
	bool CreateSoundChannel(const std::string& name);
	bool LoadSound(const std::string& groupName, const std::string& name,
		bool loop, const char* fileName, const std::string& pathName = SOUND_PATH);
	bool SetVolume(float volume);
	bool SetVolume(const std::string& groupName, float volume);
	bool SoundPlay(const std::string& name);
	bool SoundStop(const std::string& name);
	bool SoundPause(const std::string& name);
	bool SoundResume(const std::string& name);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& name);
	class CSound* FindSound(const std::string& name);
	void ReleaseSound(const std::string& name);
};

