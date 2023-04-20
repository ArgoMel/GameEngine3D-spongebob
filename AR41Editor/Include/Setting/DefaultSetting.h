#pragma once
#include "EngineSetting.h"

class CDefaultSetting : public CEngineSetting
{
private:
    void CreateCDO();
    void LoadResource();
    void SetInput();
    void SetCollision();

    // Sound
    void LoadSound();
    void LoadSfx();

    // Player
    void LoadSpongebob();
    void LoadPatrick();
    void LoadSandy();

    // Monster
    void LoadRoboSponge();
    void LoadKingJellyfish();
    void LoadJellyfish();
    void LoadTikis();
    void LoadEnemies();

    // NPC
    void LoadMrKrabs();
    void LoadSquidward();
    void LoadPatric_Npc();
    void LoadBus();
    void LoadStaticNpcs();

    // Object
    void LoadCommonObj();
    void LoadCollectibleItems();
    void LoadBuildings();
    void LoadJellyfishFieldsObj();
    void LoadCBLabObjects();
public:
    CDefaultSetting();
    virtual ~CDefaultSetting();
    virtual void Init();
};

