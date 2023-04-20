#pragma once
#include "../../Ref.h"

class CSound : public CRef
{
private:
    friend class CSoundManager;
    FMOD::System* m_System;
    FMOD::Sound* m_Sound;
    FMOD::ChannelGroup* m_Group;
    FMOD::Channel* m_Channel;
    std::string     m_GroupName;
    bool        m_Play;
    bool        m_Loop;
    bool        m_Pause;
    char        m_FileName[MAX_PATH];
    char        m_PathName[MAX_PATH];
    CSound();
    ~CSound();
public:
    const std::string& GetGroupName()   const
    {
        return m_GroupName;
    }
    bool GetPlay()   const
    {
        return m_Play;
    }
    bool GetLoop()  const
    {
        return m_Loop;
    }
    void SetGroupName(const std::string& groupName)
    {
        m_GroupName = groupName;
    }
    bool LoadSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop,
        const char* fileName, const std::string& pathName = SOUND_PATH);
    void Play();
    void Stop();
    void Pause();
    void Resume();
    void Save(FILE* file);
    void Load(FILE* file);
};

