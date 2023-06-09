#include "DefaultSetting.h"
#include "Input.h"
#include "CollisionManager.h"
#include "Scene/Scene.h"
#include "../UI/PlayerUI.h"
#include "../UI/PauseUI.h"
#include "../UI/TitleSceneUI.h"
#include "../UI/InteractUI.h"
#include "../UI/DialogUI.h"
#include "../UI/BossUI.h"

#include "../GameObject/Player.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "../GameObject/Npc/Patric.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/InfoSign.h"
#include "../GameObject/Object/BB/BusStop.h"
#include "../GameObject/Object/Common/InteractButton.h"
#include "../GameObject/Tikis/Tiki_Stone.h"
#include "../GameObject/Tikis/Tiki_Thunder.h"
#include "../GameObject/Tikis/Tiki_Wood.h"

#include "../GameObject/KingJellyfish.h"
#include "../GameObject/Jellyfish.h"
#include "../GameObject/Object/Pool.h"

#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "../GameObject/Duplicatotron.h"
#include "../GameObject/Object/IceCube.h"
#include "../GameObject/Object/CheckPoint.h"
#include "../GameObject/Object/Gate.h"
#include "../GameObject/Object/JumpTree.h"
#include "../GameObject/Object/TeleportBox.h"

#include "../GameObject/BossMonster/RoboSponge/RoboSponge.h"
#include "../GameObject/BossMonster/RoboSponge/RoboSponge_Knob.h"
#include "../GameObject/Object/CBL/CBL_BaseMesh.h"
#include "../GameObject/Object/CBL/CBL_Floor.h"
#include "../GameObject/Object/CBL/CBL_Platform.h"
#include "../GameObject/Object/Common/Trampoline.h"
#include "../GameObject/Object/Common/Collectible/UnderWear.h"

CDefaultSetting::CDefaultSetting()
{
}

CDefaultSetting::~CDefaultSetting()
{
}

void CDefaultSetting::Init()
{
    LoadResource();
	CreateCDO();
	SetInput();
	SetCollision();
}

void CDefaultSetting::CreateCDO()
{
    CScene::CreateUIWindowCDO<CPlayerUI>("PlayerUI");
    CScene::CreateUIWindowCDO<CPauseUI>("PauseUI");
    CScene::CreateUIWindowCDO<CTitleSceneUI>("TitleSceneUI");
    CScene::CreateUIWindowCDO<CInteractUI>("InteractUI");
    CScene::CreateUIWindowCDO<CDialogUI>("DialogUI");
    CScene::CreateUIWindowCDO<CBossUI>("BossUI");

    CScene::CreateObjectCDO<CPlayer>("Player");
    CScene::CreateObjectCDO<CMrKrabs>("MrKrabs");
    CScene::CreateObjectCDO<CSquidward>("Squidward");
    CScene::CreateObjectCDO<CPatric>("Patric");
    CScene::CreateObjectCDO<CBusStop>("BusStop");
    CScene::CreateObjectCDO<CTiki_Stone>("Tiki_Stone");
    CScene::CreateObjectCDO<CTiki_Thunder>("Tiki_Thunder");
    CScene::CreateObjectCDO<CTiki_Wood>("Tiki_Wood");
    CScene::CreateObjectCDO<CInteractButton>("InteractButton");
    CScene::CreateObjectCDO<CInfoSign>("InfoSign");

    CScene::CreateObjectCDO<CPool>("Pool");
    CScene::CreateObjectCDO<CKingJellyfish>("KingJellyfish");
    CScene::CreateObjectCDO<CJellyfish>("Jellyfish");

    CScene::CreateObjectCDO<CFodder>("Fodder");
    CScene::CreateObjectCDO<CHammer>("Hammer");
    //CScene::CreateObjectCDO<CDuplicatotron>("Duplicatotron");
    CScene::CreateObjectCDO<CIceCube>("IceCube");
    CScene::CreateObjectCDO<CJumpTree>("JumpTree ");
    CScene::CreateObjectCDO<CTeleportBox>("TeleportBox");
    CScene::CreateObjectCDO<CCheckPoint>("CheckPoint");
    CScene::CreateObjectCDO<CGate>("Gate");

    CScene::CreateObjectCDO<CRoboSponge>("Robo_Sponge");
    CScene::CreateObjectCDO<CRoboSponge_Knob>("RoboSponge_Knob");
    CScene::CreateObjectCDO<CCBL_BaseMesh>("CBL_BaseMesh");
    CScene::CreateObjectCDO<CCBL_Floor>("CBL_Floor");
    CScene::CreateObjectCDO<CCBL_Platform>("CBL_Platform");
    CScene::CreateObjectCDO<CTrampoline>("Trampoline");
    CScene::CreateObjectCDO<CUnderWear>("UnderWear");
}

void CDefaultSetting::LoadResource()
{
    //CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Player_Default", TEXT("Player_Default.msh"));

    // Sound
    LoadSound();
    LoadSfx();

    // Player
    LoadSpongebob();
    LoadPatrick();
    LoadSandy();

    // Monster
    LoadRoboSponge();
    LoadKingJellyfish();
    LoadJellyfish();
    LoadTikis();
    LoadEnemies();

    // NPC
    LoadMrKrabs();
    LoadSquidward();
    LoadPatric_Npc();
    LoadBus();
    LoadStaticNpcs();

    // Object
    LoadCommonObj();
    LoadCollectibleItems();
    LoadBuildings();
    LoadJellyfishFieldsObj();
    LoadCBLabObjects();
}

void CDefaultSetting::SetInput()
{
    // Num, FNum
    CInput::GetInst()->AddBindKey("1", '1');
    CInput::GetInst()->AddBindKey("2", '2');
    CInput::GetInst()->AddBindKey("3", '3');
    CInput::GetInst()->AddBindKey("4", '4');
    CInput::GetInst()->AddBindKey("5", '5');
    CInput::GetInst()->AddBindKey("6", '6');
    CInput::GetInst()->AddBindKey("7", '7');
    CInput::GetInst()->AddBindKey("8", '8');
    CInput::GetInst()->AddBindKey("9", '9');
    CInput::GetInst()->AddBindKey("0", '0');

    CInput::GetInst()->AddBindKey("F1", VK_F1);
    CInput::GetInst()->AddBindKey("F2", VK_F2);
    CInput::GetInst()->AddBindKey("F3", VK_F3);
    CInput::GetInst()->AddBindKey("F4", VK_F4);     //editor component move
    CInput::GetInst()->AddBindKey("F5", VK_F5);     //editor pause
    CInput::GetInst()->AddBindKey("F6", VK_F6);
    CInput::GetInst()->AddBindKey("F7", VK_F7);
    CInput::GetInst()->AddBindKey("F8", VK_F8);
    CInput::GetInst()->AddBindKey("F9", VK_F9);     //mouse img on/off
    CInput::GetInst()->AddBindKey("F10", VK_F10);   //Do not use
    CInput::GetInst()->AddBindKey("F11", VK_F11);   //mouse cursor on/off
    CInput::GetInst()->AddBindKey("F12", VK_F12);   //Do not use

    // Arrow
    CInput::GetInst()->AddBindKey("W", 'W');
    CInput::GetInst()->AddBindKey("S", 'S');
    CInput::GetInst()->AddBindKey("D", 'D');
    CInput::GetInst()->AddBindKey("A", 'A');

    CInput::GetInst()->AddBindKey("Space", VK_SPACE);
    CInput::GetInst()->AddBindKey("LClick", VK_LBUTTON);
    CInput::GetInst()->AddBindKey("RClick", VK_RBUTTON);
    CInput::GetInst()->AddBindKey("MClick", VK_MBUTTON);

    CInput::GetInst()->AddBindKey("E", 'E');
    CInput::GetInst()->AddBindKey("Q", 'Q');
    CInput::GetInst()->AddBindKey("F", 'F');
    CInput::GetInst()->AddBindKey("Esc", VK_ESCAPE);
    CInput::GetInst()->AddBindKey("Tab", VK_TAB);

    //editor
    CInput::GetInst()->AddBindKey("UArrow", VK_UP);
    CInput::GetInst()->AddBindKey("DArrow", VK_DOWN);
    CInput::GetInst()->AddBindKey("LArrow", VK_LEFT);
    CInput::GetInst()->AddBindKey("RArrow", VK_RIGHT);

    CInput::GetInst()->AddBindKey("PageUp", VK_PRIOR);
    CInput::GetInst()->AddBindKey("PageDown", VK_NEXT);

    CInput::GetInst()->AddBindKey("Del", VK_DELETE);// delete object
    CInput::GetInst()->SetKeyCtrl("Del", true);
    CInput::GetInst()->AddBindKey("CtrlD", 'D');    //copy object
    CInput::GetInst()->SetKeyCtrl("CtrlD", true);
    CInput::GetInst()->AddBindKey("CtrlG", 'G');    //gizmo camera
    CInput::GetInst()->SetKeyCtrl("CtrlG", true);
    CInput::GetInst()->AddBindKey("CtrlShiftC", 'C');    //ChatGPT
    CInput::GetInst()->SetKeyCtrl("CtrlShiftC", true);
    CInput::GetInst()->SetKeyShift("CtrlShiftC", true);
    CInput::GetInst()->AddBindKey("CtrlShiftT", 'T');    //ChatWIndow
    CInput::GetInst()->SetKeyCtrl("CtrlShiftT", true);
    CInput::GetInst()->SetKeyShift("CtrlShiftT", true);
    CInput::GetInst()->AddBindKey("ShiftF1", VK_F1);    //OpenWindow
    CInput::GetInst()->SetKeyShift("ShiftF1", true);
}

void CDefaultSetting::SetCollision()
{
    CCollisionManager::GetInst()->CreateChannel("Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("Monster", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("Invisible", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("Marker", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("Wall", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("Collectible", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("Npc", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("DetectArea", ECollision_Interaction::Collision);

    CCollisionManager::GetInst()->CreateProfile("Player", "Player", true);
    CCollisionManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
    CCollisionManager::GetInst()->CreateProfile("Monster", "Monster", true);
    CCollisionManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);
    CCollisionManager::GetInst()->CreateProfile("Invisible", "Invisible", true, ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateProfile("Marker", "Marker", true, ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateProfile("Wall", "Wall", true);
    CCollisionManager::GetInst()->CreateProfile("Collectible", "Collectible", true);
    CCollisionManager::GetInst()->CreateProfile("Npc", "Npc", true);
    CCollisionManager::GetInst()->CreateProfile("DetectArea", "DetectArea", true, ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "MonsterAttack", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "Marker", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "Player", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "Marker", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Default", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Monster", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "Monster", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Marker", "Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("Marker", "PlayerAttack", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("Wall", "Wall", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteractionAllChannel("Collectible", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Collectible", "Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteractionAllChannel("Npc", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Npc", "Player", ECollision_Interaction::Collision);
}

void CDefaultSetting::LoadSound()
{
    //base volume
    CResourceManager::GetInst()->SetVolume("Effect", 30);   //VFX
    CResourceManager::GetInst()->SetVolume("BGM", 30);  //Music
    CResourceManager::GetInst()->SetVolume("UI", 30);   //Talk

    // 로딩 UI
    CResourceManager::GetInst()->LoadSound("Effect", "LoadingUI_First", false, "UI/SFX_SB_Spongball_Bubble_010.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "LoadingUI_Second", false, "UI/SFX_SB_Spongball_Bubble_008.ogg", SOUND_PATH); // first와 같이 재생
    CResourceManager::GetInst()->LoadSound("Effect", "LoadingUI", false, "UI/SFX_Bubbles_Add_001.ogg");

    // Bikini Bottom
    CResourceManager::GetInst()->LoadSound("BGM", "BikiniCity", false, "Map/MUS_BikiniBottomTheme.ogg", SOUND_PATH);

    // Jellyfish Field
    CResourceManager::GetInst()->LoadSound("BGM", "JellyFish", false, "Map/MUS_JellyfishFieldsTheme.ogg", SOUND_PATH);

    // BossStage BGM
    CResourceManager::GetInst()->LoadSound("BGM", "KingJellyFish", true, "Map/MUS_BossFightTheme.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("BGM", "RoboSpongebob", true, "Map/MUS_BossFightTheme.ogg", SOUND_PATH);

    //Tile BGM
    CResourceManager::GetInst()->LoadSound("UI", "UI_Backward", false, "Sfx/UI/SFX_UI_Backward_001.ogg");
    CResourceManager::GetInst()->LoadSound("BGM", "Title_Bgm", false, "Music/MUS_WavesSeagullsBg.ogg");

}

void CDefaultSetting::LoadSfx()
{
    // Traffic
    CResourceManager::GetInst()->LoadSound("Effect", "BusStop", false, "Sfx/Traffic/Bus_pullin.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "BusGo", false, "Sfx/Traffic/Bus_pullout.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "BussCall", false, "Sfx/Traffic/sfx_call_taxi.ogg");

    // Tikis
    // Tiki Wood
    CResourceManager::GetInst()->LoadSound("Effect", "TikiWoodHit1", false, "Sfx/Enemies/Tikis/TikiWood/SFX_Tiki_Wd_Dest_BassAdd_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiWoodHit2", false, "Sfx/Enemies/Tikis/TikiWood/SFX_Tiki_Wd_Dest_BassAdd_002.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiWoodHit3", false, "Sfx/Enemies/Tikis/TikiWood/SFX_Tiki_Wd_Dest_BassAdd_003.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiWoodDie1", false, "Sfx/Enemies/Tikis/TikiWood/SFX_Tiki_Wd_Dest_CrunchAdd_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiWoodDie2", false, "Sfx/Enemies/Tikis/TikiWood/SFX_Tiki_Wd_Dest_CrunchAdd_002.ogg");

    // Tiki Stone
    CResourceManager::GetInst()->LoadSound("Effect", "TikiStoneHit1", false, "Sfx/Enemies/Tikis/TikiStone/SFX_Tiki_Stone_Impact_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiStoneHit2", false, "Sfx/Enemies/Tikis/TikiStone/SFX_Tiki_Stone_Impact_002.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiStoneHit3", false, "Sfx/Enemies/Tikis/TikiStone/SFX_Tiki_Stone_Impact_003.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiStoneHit4", false, "Sfx/Enemies/Tikis/TikiStone/SFX_Tiki_Stone_Impact_004.ogg");

    // Tiki Thunder
    CResourceManager::GetInst()->LoadSound("Effect", "TikiThunderHit1", false, "Sfx/Enemies/Tikis/SFX_Tiki_Thunder_Activate_V2_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiThunderHit2", false, "Sfx/Enemies/Tikis/SFX_Tiki_Thunder_Activate_V2_002.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiThunderHit3", false, "Sfx/Enemies/Tikis/SFX_Tiki_Thunder_Activate_V2_003.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiThunderDie1", false, "Sfx/Enemies/Tikis/SFX_Tiki_Thunder_Destroy_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiThunderDie2", false, "Sfx/Enemies/Tikis/SFX_Tiki_Thunder_Destroy_002.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "TikiThunderDie3", false, "Sfx/Enemies/Tikis/SFX_Tiki_Thunder_Destroy_003.ogg");

    // Item
    // Button
    CResourceManager::GetInst()->LoadSound("Effect", "ButtonPress", false, "Sfx/Item/Button_press.ogg");

    // Collectible
    CResourceManager::GetInst()->LoadSound("Effect", "GetCollectible", false, "Sfx/Item/SFX_Collect.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "GetShiny", false, "Sfx/Item/Shiny/SFX_collect_shiny.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "CreateShiny1", false, "Sfx/Item/Shiny/SFX_ShinyThing_add_chime_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "CreateShiny2", false, "Sfx/Item/Shiny/SFX_ShinyThing_add_chime_002.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "CreateShiny3", false, "Sfx/Item/Shiny/SFX_ShinyThing_add_chime_003.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "CreateShiny4", false, "Sfx/Item/Shiny/SFX_ShinyThing_add_ding_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "CreateShiny5", false, "Sfx/Item/Shiny/SFX_ShinyThing_add_sparkle_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "CreateShiny6", false, "Sfx/Item/Shiny/SFX_ShinyThing_add_sparkle_002.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "CreateShiny7", false, "Sfx/Item/Shiny/SFX_ShinyThing_add_sparkle_003.ogg");


    // Robo Sponge
    CResourceManager::GetInst()->LoadSound("Effect", "RoboSponge_AttackVertic", false, "Sfx/Bosses/RoboSponge/SFX_RSB_ChopSwing_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "RoboSponge_AttackHoriz", false, "Sfx/Bosses/RoboSponge/SFX_RSB_Swipe.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "RoboSponge_AttackWords", false, "Sfx/Bosses/RoboSponge/SFX_RSB_KAH_Reso_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "RoboSponge_Explode", false, "Sfx/Bosses/RoboSponge/SFX_RSB_Explode_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "HitPlat1", false, "Sfx/Bosses/RoboSponge/SFX_RSB_HitPlat_V1_001.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "HitPlat2", false, "Sfx/Bosses/RoboSponge/SFX_RSB_HitPlat_V1_002.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "HitPlat3", false, "Sfx/Bosses/RoboSponge/SFX_RSB_HitPlat_V1_003.ogg");
    CResourceManager::GetInst()->LoadSound("Effect", "RoboSponge_Laugh", false, "Sfx/Bosses/RoboSponge/SFX_RSB_Laugh.ogg");

}

void CDefaultSetting::LoadSpongebob()
{
    // Spongebob
    CResourceManager* resourceManager = CResourceManager::GetInst();
    resourceManager->LoadMesh(nullptr, MeshType::Animation, "SpongebobWeapon", TEXT("Spongebob\\wand_bubble_wand.msh"));
    resourceManager->LoadSkeleton(nullptr, "SpongebobWandSkeleton", TEXT("Spongebob\\wand_bubble_wand.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("SpongebobWeapon", "SpongebobWandSkeleton");

    resourceManager->LoadMesh(nullptr, MeshType::Animation, "Spongebob", TEXT("Spongebob\\Spongebob_mesh.msh"));
    resourceManager->LoadSkeleton(nullptr, "SpongebobSkeleton", TEXT("Spongebob\\Spongebob_mesh.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("Spongebob", "SpongebobSkeleton");
    resourceManager->AddSocket("SpongebobSkeleton", "MiddleFinger3_R", "Weapon");
    resourceManager->LoadAnimationSequence("Spongebob_Idle", TEXT("Spongebob\\Anim_Spongebob_Idle.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Walk", TEXT("Spongebob\\Anim_Spongebob_Walk.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Attack", TEXT("Spongebob\\Anim_Spongebob_BubbleSpin.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_JumpDw", TEXT("Spongebob\\Anim_Spongebob_Jump_Dw.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_JumpUp", TEXT("Spongebob\\Anim_Spongebob_Jump_Up.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BashStart", TEXT("Spongebob\\Anim_Spongebob_BubbleBash_Dw_Start.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BashDw", TEXT("Spongebob\\Anim_Spongebob_Bubblebash_Dw.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Bash", TEXT("Spongebob\\Anim_Spongebob_BubbleBash.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Death", TEXT("Spongebob\\Anim_Spongebob_Death_02.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_Hit", TEXT("Spongebob\\Anim_Spongebob_Hit_01.sqc"), MESH_PATH);
    //전용 모션
    resourceManager->LoadAnimationSequence("Spongebob_Bowl", TEXT("Spongebob\\Anim_Spongebob_Bubble_Bowl_Loop.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BowlThrow", TEXT("Spongebob\\Anim_Spongebob_Bubble_Bowl_Throw.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BounceStart", TEXT("Spongebob\\Anim_Spongebob_BubbleBounce_Start.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BounceLoop", TEXT("Spongebob\\Anim_Spongebob_BubbleBounce_Loop.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_BounceLanding", TEXT("Spongebob\\Anim_Spongebob_BubbleBounce_Landing.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_MissileEnd", TEXT("Spongebob\\Anim_Spongebob_Cruise_Missile_Release_End.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_MissileLoop", TEXT("Spongebob\\Anim_Spongebob_Cruise_Missile_Release_Loop.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Spongebob_MissileStart", TEXT("Spongebob\\Anim_Spongebob_Cruise_Missile_Release_Start.sqc"), MESH_PATH);

    resourceManager->LoadMesh(nullptr, MeshType::Animation, "SpongebobMissile", TEXT("Cruise_Missile\\SK_CruiseMissle.msh"));
    resourceManager->LoadSkeleton(nullptr, "SpongebobMissileSkeleton", TEXT("Cruise_Missile\\SK_CruiseMissle.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("SpongebobMissile", "SpongebobMissileSkeleton");
    resourceManager->LoadAnimationSequence("SpongebobMissile_Idle", TEXT("Cruise_Missile\\Anim_Cruise_Missile_Idle.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("SpongebobMissile_Start", TEXT("Cruise_Missile\\Anim_Cruise_Missile_Start.sqc"), MESH_PATH);

    resourceManager->LoadSound("Effect", "Spongebob_BubbleSpin", false, "Spongebob/SB_bubble_spin_rework_v2.ogg", SOUND_PATH); // 해파리채 공격
    resourceManager->LoadSound("Effect", "Spongebob_Bash", false, "Spongebob/SFX_SB_BubbleBash_V2.ogg", SOUND_PATH); // 해파리채 공격 강타
    resourceManager->LoadSound("Effect", "Spongebob_Jump", false, "Spongebob/SB_jump_sngl.ogg", SOUND_PATH); // 점프
    resourceManager->LoadSound("Effect", "Spongebob_DoubleJump", false, "Spongebob/SB_jump_dub.ogg", SOUND_PATH); // 이단 점프
    resourceManager->LoadSound("Effect", "Spongebob_BubbleExplode", false, "Spongebob/SFX_SB_CruiseBubble_Explode.ogg", SOUND_PATH); // 미사일 명중
    resourceManager->LoadSound("Effect", "Spongebob_BubbleLaunch", false, "Spongebob/SFX_SB_CruiseBubble_Launch.ogg", SOUND_PATH); // 미사일 발사
    resourceManager->LoadSound("Effect", "Spongebob_BubbleLoop", true, "Spongebob/SFX_SB_CruiseBubble_Loop.ogg", SOUND_PATH); // 미사일 이동
    resourceManager->LoadSound("Effect", "Spongebob_BubbleCharge", false, "Spongebob/SFX_SB_CruiseMissile_Charge.ogg", SOUND_PATH); // 미사일 충전
    resourceManager->LoadSound("Effect", "Spongebob_Walk", true, "Spongebob/SFX_SB_Run_L.ogg", SOUND_PATH); // 걷기 왼발
    //  resourceManager->LoadSound("Effect", "Spongebob_WalkRight", true, "Spongebob/SFX_SB_Run_R.ogg", SOUND_PATH); // 걷기 오른발
    resourceManager->LoadSound("Effect", "Spongebob_BubbleBowl_Charge", true, "Spongebob/SFX_SB_BubbleBowl_Charge.ogg");
    resourceManager->LoadSound("Effect", "Spongebob_BubbleBowl_Throw", false, "Spongebob/SFX_SB_BubbleBowl_Throw.ogg");
    resourceManager->LoadSound("Effect", "Spongebob_Damage", false, "Spongebob/SB_ouch1.ogg", SOUND_PATH); // 다쳤을 때
    resourceManager->LoadSound("Effect", "Spongebob_Death", false, "Spongebob/SFX_SB_Death.ogg", SOUND_PATH); // 죽음
}

void CDefaultSetting::LoadPatrick()
{
    CResourceManager* resourceManager = CResourceManager::GetInst();
    resourceManager->LoadMesh(nullptr, MeshType::Animation, "PatrickWeapon", TEXT("Objects/JellyfishFields/Ice.msh"), MESH_PATH);
    resourceManager->LoadSkeleton(nullptr, "IceSkeleton", TEXT("Objects/JellyfishFields/Ice.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("PatrickWeapon", "IceSkeleton");

    resourceManager->LoadMesh(nullptr, MeshType::Animation, "Patrick", TEXT("Patrick/Patrick11.msh"), MESH_PATH);
    resourceManager->LoadSkeleton(nullptr, "PatrickSkeleton", TEXT("Patrick/Patrick11.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("Patrick", "PatrickSkeleton");
    resourceManager->AddSocket("PatrickSkeleton", "jt_hand_R", "Weapon", Vector3(-10.f, 45.f, -50.f));

    resourceManager->LoadAnimationSequence("Patrick_Idle", TEXT("Patrick/Patrick_Idle.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_Walk", TEXT("Patrick/Patrick_Walk.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_Attack", TEXT("Patrick/Patrick_BellyAttack.sqc"), MESH_PATH);
    //resourceManager->LoadAnimationSequence("Patrick_Run", TEXT("Patrick/Patrick_Run.sqc"), MESH_PATH);
    //resourceManager->LoadAnimationSequence("Patrick_DoubleJump", TEXT("Patrick/Patrick_DoubleJump.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_JumpUp", TEXT("Patrick/Patrick_JumpUp.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_JumpDw", TEXT("Patrick/Anim_Patrick_Jump_DW.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_BashStart", TEXT("Patrick/Anim_Patrick_Slam_Start.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_BashDw", TEXT("Patrick/Anim_Patrick_Slam_Loop.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_Bash", TEXT("Patrick/Anim_Patrick_Slam_End.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_Hit", TEXT("Patrick\\anim_patrick_Hit.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_Death", TEXT("Patrick\\Anim_Patrick_Death_01.sqc"), MESH_PATH);

    resourceManager->LoadAnimationSequence("Patrick_PickUp", TEXT("Patrick/Patrick_PickUp.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_PickUpIdle", TEXT("Patrick/Patrick_PickUpIdle.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_PickUpWalk", TEXT("Patrick/Patrick_PickUpWalk.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Patrick_Throw", TEXT("Patrick/Patrick_Throw.sqc"), MESH_PATH);

    resourceManager->LoadSound("Effect", "Patrick_Attack", false, "Patrick/SFX_PS_BellyAttack.ogg");
    resourceManager->LoadSound("Effect", "Patrick_Jump", false, "Patrick/SFX_PS_Jump.ogg");
    resourceManager->LoadSound("Effect", "Patrick_Walk", true, "Patrick/Pat_step1.ogg");
    resourceManager->LoadSound("Effect", "Patrick_Bash", false, "Patrick/SFX_PS_BodySlam.ogg");
    resourceManager->LoadSound("Effect", "Patrick_Lift", false, "Patrick/SFX_PS_Lift.ogg");
    resourceManager->LoadSound("Effect", "Patrick_Throw", false, "Patrick/SFX_PS_Throw.ogg");
    resourceManager->LoadSound("Effect", "Patrick_Damage", false, "Patrick/Pat_Ouch1.ogg");
    resourceManager->LoadSound("Effect", "Patrick_Death", false, "Patrick/Pat_Ouch2.ogg");
}

void CDefaultSetting::LoadSandy()
{
    CResourceManager* resourceManager = CResourceManager::GetInst();
    resourceManager->LoadMesh(nullptr, MeshType::Animation, "SandyWeapon", TEXT("Sandy\\Lasso\\Lasso.msh"));
    resourceManager->LoadSkeleton(nullptr, "LassoSkeleton", TEXT("Sandy\\Lasso\\Lasso.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("SandyWeapon", "LassoSkeleton");

    resourceManager->LoadAnimationSequence("Lasso_Idle", TEXT("Sandy\\Lasso\\Anim_lasso_Pose_Straight_Rope.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Lasso_Start", TEXT("Sandy\\Lasso\\Anim_lasso_attack_start.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Lasso_End", TEXT("Sandy\\Lasso\\Anim_lasso_attack_end.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Lasso_Copter", TEXT("Sandy\\Lasso\\Anim_lasso_copter.sqc"), MESH_PATH);

    resourceManager->LoadMesh(nullptr, MeshType::Animation, "Sandy", TEXT("Sandy/Sandy_Idle.msh"), MESH_PATH);
    resourceManager->LoadSkeleton(nullptr, "SandySkeleton", TEXT("Sandy/Sandy_Idle.bne"), MESH_PATH);
    resourceManager->SetMeshSkeleton("Sandy", "SandySkeleton");
    resourceManager->AddSocket("SandySkeleton", "jt_lasso", "Weapon", Vector3(-5.f, 105.f, 105.f));

    resourceManager->LoadAnimationSequence("Sandy_Idle", TEXT("Sandy/Sandy_Idle.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_Walk", TEXT("Sandy/Sandy_Walk.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_Run", TEXT("Sandy/Sandy_Run.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_JumpDw", TEXT("Sandy/Sandy_JumpDW.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_JumpUp", TEXT("Sandy/Sandy_JumpUp.sqc"), MESH_PATH);

    resourceManager->LoadAnimationSequence("Sandy_Attack", TEXT("Sandy/Sandy_Karate_Chop.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_Karate_Kick", TEXT("Sandy/Sandy_Karate_Kick.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_Lasso_Start", TEXT("Sandy/Sandy_Lasso_Start.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_Death", TEXT("Sandy/Sandy_Death.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_Hit", TEXT("Sandy/Anim_sandy_Hit_02.sqc"), MESH_PATH);
    resourceManager->LoadAnimationSequence("Sandy_Swing_Loop", TEXT("Sandy/Anim_Sandy_Swing_Loop.sqc"), MESH_PATH);

    resourceManager->LoadSound("Effect", "Sandy_Chop", false, "Sandy/SFX_SC_Chop.ogg", SOUND_PATH); // 주먹질
    resourceManager->LoadSound("Effect", "Sandy_Kick", false, "Sandy/SFX_SC_Kick.ogg", SOUND_PATH); // 발차기
    resourceManager->LoadSound("Effect", "Sandy_Jump", false, "Sandy/SFX_SC_Jump.ogg", SOUND_PATH); // 점프
    resourceManager->LoadSound("Effect", "Sandy_LassoAttack", false, "Sandy/SFX_SC_LassoAttack_Throw.ogg", SOUND_PATH); // 올가미 공격
    resourceManager->LoadSound("Effect", "Sandy_LassoAttack_End", false, "Sandy/SFX_SC_LassoAttack_End.ogg", SOUND_PATH); // 올가미 공격 끝
    resourceManager->LoadSound("Effect", "Sandy_Hovering", true, "Sandy/SFX_SC_Lasso_Copter.ogg");
    resourceManager->LoadSound("Effect", "Sandy_Walk", true, "Sandy/SFX_SC_Step_001.ogg", SOUND_PATH); // 걷기
    resourceManager->LoadSound("Effect", "Sandy_Damage", false, "Sandy/SC_Ouch1.ogg", SOUND_PATH); // 다쳤을 때
    resourceManager->LoadSound("Effect", "Sandy_Death", false, "Sandy/SC_Ouch2.ogg");
}

void CDefaultSetting::LoadRoboSponge()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Robo_Sponge", TEXT("Robo_Sponge/Robo_Sponge.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Robo_Sponge_Skeleton", TEXT("Robo_Sponge/Robo_Sponge.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Robo_Sponge", "Robo_Sponge_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Horiz_L", TEXT("Robo_Sponge/Robo_Sponge_Attack_Horiz_L.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Horiz_L_Pose", TEXT("Robo_Sponge/Robo_Sponge_Attack_Horiz_L_Pose.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Horiz_R", TEXT("Robo_Sponge/Robo_Sponge_Attack_Horiz_R.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Horiz_R_Pose", TEXT("Robo_Sponge/Robo_Sponge_Attack_Horiz_R_Pose.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Vertic_L_Loop", TEXT("Robo_Sponge/Robo_Sponge_Attack_Vertic_L_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Vertic_L_Start", TEXT("Robo_Sponge/Robo_Sponge_Attack_Vertic_L_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Vertic_R_Loop", TEXT("Robo_Sponge/Robo_Sponge_Attack_Vertic_R_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Vertic_R_Start", TEXT("Robo_Sponge/Robo_Sponge_Attack_Vertic_R_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Words_Loop", TEXT("Robo_Sponge/Robo_Sponge_Attack_Words_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Attack_Words_Start", TEXT("Robo_Sponge/Robo_Sponge_Attack_Words_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Hit1", TEXT("Robo_Sponge/Robo_Sponge_Hit1.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Hit2", TEXT("Robo_Sponge/Robo_Sponge_Hit2.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Hit2_Pose", TEXT("Robo_Sponge/Robo_Sponge_Hit2_Pose.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Idle", TEXT("Robo_Sponge/Robo_Sponge_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Vertic_L_Hold", TEXT("Robo_Sponge/Robo_Sponge_Vertic_L_Hold.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Vertic_R_Hold", TEXT("Robo_Sponge/Robo_Sponge_Vertic_R_Hold.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Victory", TEXT("Robo_Sponge/Robo_Sponge_Victory.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Robo_Sponge_Death", TEXT("Robo_Sponge/Robo_Sponge_Death.sqc"), MESH_PATH);

    // RoboSponge Word Attack
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "KAH", TEXT("Robo_Sponge/AttackWords/KAH.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "RAH", TEXT("Robo_Sponge/AttackWords/RAH.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "TAE", TEXT("Robo_Sponge/AttackWords/TAE.msh"), MESH_PATH);

    // RoboSponge Socket Mesh
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "GreenKnob", TEXT("Robo_Sponge/GreenKnob.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BrokenKnob", TEXT("Robo_Sponge/BorkenKnob.msh"), MESH_PATH);
}

void CDefaultSetting::LoadKingJellyfish()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "KingJellyfish", TEXT("KingJellyfish/KingJellyfish.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "KingJellyfishSkeleton", TEXT("KingJellyfish/KingJellyfish.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("KingJellyfish", "KingJellyfishSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Idle", TEXT("KingJellyfish/bossJellyKing_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Angry", TEXT("KingJellyfish/bossJellyKing_angry.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Attack", TEXT("KingJellyfish/bossJellyKing_attack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_Damage", TEXT("KingJellyfish/bossJellyKing_damage.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_OnGround", TEXT("KingJellyfish/bossJellyKing_onGround.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("KingJellyfish_SpawnJellyfish", TEXT("KingJellyfish/bossJellyKing_spawnJellyfish.sqc"), MESH_PATH);

    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Idle", true, "KingJellyfish/SFX_Boss_JFK_Move.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Charge", false, "KingJellyfish/KJ_Charge.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Angry", false, "KingJellyfish/KJ_Taunt.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Damage", false, "KingJellyfish/SFX_Boss_JFK_Grunt.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_SpawnJellyfish", false, "KingJellyfish/SFX_Boss_JFK_Birth_New.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_OnGround", false, "KingJellyfish/SFX_Boss_JFK_Land_001.ogg", SOUND_PATH);

    // 캐릭터 이펙트 사운드
    // CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Pulse", false, "KingJellyfish/SFX_Boss_JFK_Pulse.ogg", SOUND_PATH);
    // CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Shield_Down", false, "KingJellyfish/SFX_Boss_JFK_Shield_Down.ogg", SOUND_PATH);
    // CResourceManager::GetInst()->LoadSound("Effect", "KingJellyfish_Shield_Up", false, "KingJellyfish/SFX_Boss_JFK_Shield_Up.ogg", SOUND_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "KingJellyfish_Electric", TEXT("KingJellyfish/KingJellyfish_Electric.msh"), MESH_PATH);

}

void CDefaultSetting::LoadJellyfish()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Jellyfish", TEXT("Jellyfish/Jellyfish.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "JellyfishSkeleton", TEXT("Jellyfish/Jellyfish.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Jellyfish", "JellyfishSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Jellyfish_Attack", TEXT("Jellyfish/Jellyfish_attack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Jellyfish_Death", TEXT("Jellyfish/Jellyfish_death.sqc"), MESH_PATH);

    CResourceManager::GetInst()->LoadSound("Effect", "Jellyfish_Attack", false, "Jellyfish/SFX_Enemy_Jellyfish_Attack_Original.ogg", SOUND_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Jellyfish_Electric", TEXT("Jellyfish/Jellyfish_Electric.msh"), MESH_PATH);
}

void CDefaultSetting::LoadTikis()
{
    // Woods -> Normal
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Tiki_Woods", TEXT("Tikis/Tiki_Woods.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Tiki_Woods_Skeleton", TEXT("Tikis/Tiki_Woods.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Tiki_Woods", "Tiki_Woods_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Woods_Idle", TEXT("Tikis/Tiki_Woods_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Woods_Die", TEXT("Tikis/Tiki_Woods_Die.sqc"), MESH_PATH);


    // Stone -> Unbreakerble
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Tiki_Stone", TEXT("Tikis/Tiki_Stone.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Tiki_Stone_Skeleton", TEXT("Tikis/Tiki_Stone.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Tiki_Stone", "Tiki_Stone_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Stone_Idle", TEXT("Tikis/Tiki_Stone_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Stone_Die", TEXT("Tikis/Tiki_Stone_Die.sqc"), MESH_PATH);


    // Thunder -> Bomb
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Tiki_Thunder", TEXT("Tikis/Tiki_Thunder.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Tiki_Thunder_Skeleton", TEXT("Tikis/Tiki_Thunder.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Tiki_Thunder", "Tiki_Thunder_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Thunder_Idle", TEXT("Tikis/Tiki_Thunder_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Tiki_Thunder_Die", TEXT("Tikis/Tiki_Thunder_Die.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadEnemies()
{
    // Fodder
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Fodder", TEXT("Enemies/Fodder/SK_Enemy_Fodder.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "FodderSkeleton", TEXT("Enemies/Fodder/SK_Enemy_Fodder.bne"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Fodder", TEXT("Enemies/Fodder/SK_Enemy_Fodder.msh"), MESH_PATH);

    //CResourceManager::GetInst()->LoadSkeleton(nullptr, "FodderSkeleton", TEXT("Enemies/Fodder/FodderMesh1.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Fodder", "FodderSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Fodder_Walk", TEXT("Enemies/Fodder/Fodder_Walk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Fodder_Attack", TEXT("Enemies/Fodder/Fodder_Attack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Fodder_Dead", TEXT("Enemies/Fodder/Fodder_Dead.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Fodder_Notice", TEXT("Enemies/Fodder/Fodder_Notice.sqc"), MESH_PATH);

    // Fodder_Debris
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "FodderDebris1", TEXT("Enemies/Fodder/Fodder_Debris1Mesh.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "FodderDebris2", TEXT("Enemies/Fodder/Fodder_Debris2Mesh.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "FodderDebris3", TEXT("Enemies/Fodder/Fodder_Debris3Mesh.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "FodderDebris4", TEXT("Enemies/Fodder/Fodder_Debris4Mesh.msh"), MESH_PATH);


    // Hammer
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Hammer", TEXT("Enemies/Hammer/HammerMesh.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "HammerSkeleton", TEXT("Enemies/Hammer/HammerMesh.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Hammer", "HammerSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Hammer_Attack", TEXT("Enemies/Hammer/Hammer_Attack.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Hammer_Notice", TEXT("Enemies/Hammer/Hammer_Notice.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Hammer_Walk", TEXT("Enemies/Hammer/Hammer_Walk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Hammer_Dead", TEXT("Enemies/Hammer/Hammer_Dead.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Hammer_Stunned", TEXT("Enemies/Hammer/Hammer_Stunned.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Hammer_Lassoed", TEXT("Enemies/Hammer/Hammer_Lassoed.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Hammer_LassoedStart", TEXT("Enemies/Hammer/Hammer_LassoedStart.sqc"), MESH_PATH);


    // Hammer_Debris
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "HammerDebris1", TEXT("Enemies/Hammer/Hammer_Debris1Mesh.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "HammerDebris2", TEXT("Enemies/Hammer/Hammer_Debris2Mesh.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "HammerDebris3", TEXT("Enemies/Hammer/Hammer_Debris3Mesh.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "HammerDebris4", TEXT("Enemies/Hammer/Hammer_Debris4Mesh.msh"), MESH_PATH);

    // Duplicatotron    
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Duplicatotron", TEXT("Enemies/Duplicatotron/DuplicatotronMeshReal.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "DuplicatotronMeshSkeleton", TEXT("Enemies/DuplicatotronMesh/DuplicatotronMeshReal.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Duplicatotron", "DuplicatotronSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Duplicatotron_Idle", TEXT("Enemies/Duplicatotron/Duplicatotron_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Duplicatotron_Destroyed", TEXT("Enemies/Duplicatotron/Duplicatotron_Destroyed.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Duplicatotron_SpawnEnemies", TEXT("Enemies/Duplicatotron/Duplicatotron_SpawnEnemies.sqc"), MESH_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DuplicatotronCan", TEXT("Enemies/Duplicatotron/DuplicatotronCan.msh"), MESH_PATH);

    // Sound
    CResourceManager::GetInst()->LoadSound("Effect", "Fodder_Attack", false, "Enemies/Fodder_Attack.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "Ham_Hit", false, "Enemies/SFX_Ham_Hit_002.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "Robot_Explode", false, "Enemies/SFX_Robot_Explode_002.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "Dupli_idle", false, "Enemies/Dupli_idle.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "Dupli_full", false, "Enemies/Dupli_full.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "Dupli_destroy", false, "Enemies/Dupli_destroy.ogg", SOUND_PATH);
}

void CDefaultSetting::LoadMrKrabs()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "MrKrabs", TEXT("MrKrabs/MrKrabs.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "MrKrabs_Skeleton", TEXT("MrKrabs/MrKrabs.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("MrKrabs", "MrKrabs_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Angry_Loop", TEXT("MrKrabs/MrKrabs_Angry_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Angry_Start", TEXT("MrKrabs/MrKrabs_Angry_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Deceptive_Loop", TEXT("MrKrabs/MrKrabs_Deceptive_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Deceptive_Start", TEXT("MrKrabs/MrKrabs_Deceptive_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Greedy_Loop", TEXT("MrKrabs/MrKrabs_Greedy_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Greedy_Start", TEXT("MrKrabs/MrKrabs_Greedy_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Idle", TEXT("MrKrabs/MrKrabs_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("MrKrabs_Laughing", TEXT("MrKrabs/MrKrabs_Laughing.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadSquidward()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Squidward", TEXT("Squidward/Squidward.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Squidward_Skeleton", TEXT("Squidward/Squidward.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Squidward", "Squidward_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Angry_Loop", TEXT("Squidward/Squidward_Angry_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Angry_Start", TEXT("Squidward/Squidward_Angry_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Annoyed_Loop", TEXT("Squidward/Squidward_Annoyed_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Annoyed_Start", TEXT("Squidward/Squidward_Annoyed_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Happy_Loop", TEXT("Squidward/Squidward_Happy_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Happy_Start", TEXT("Squidward/Squidward_Happy_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Hurt_Loop", TEXT("Squidward/Squidward_Hurt_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Hurt_Start", TEXT("Squidward/Squidward_Hurt_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Idle", TEXT("Squidward/Squidward_Idle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Sarcastic_Loop", TEXT("Squidward/Squidward_Sarcastic_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Sarcastic_Start", TEXT("Squidward/Squidward_Sarcastic_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Talk", TEXT("Squidward/Squidward_Talk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Talk_Idle", TEXT("Squidward/Squidward_Talk_Idle.sqc"), MESH_PATH);

    // Squidward Bandage
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Squidward_Bandage", TEXT("Squidward/Cutscene/Squidward_Bandage.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Squidward_Bandage_Skeleton", TEXT("Squidward/Cutscene/Squidward_Bandage.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Squidward_Bandage", "Squidward_Bandage_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Intro1", TEXT("Squidward/Cutscene/Squidward_Intro1.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Intro2", TEXT("Squidward/Cutscene/Squidward_Intro2.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Squidward_Outro1", TEXT("Squidward/Cutscene/Squidward_Outro1.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadPatric_Npc()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Patric_Npc", TEXT("Patric_Npc/Patric.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Patric_Npc_Skeleton", TEXT("Patric_Npc/Patric.bne"), MESH_PATH);

    CResourceManager::GetInst()->SetMeshSkeleton("Patric_Npc", "Patric_Npc_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Confused", TEXT("Patric_Npc/Patric_Npc_Confused.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Default", TEXT("Patric_Npc/Patric_Npc_Default.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Excited", TEXT("Patric_Npc/Patric_Npc_Excited.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Scowl_Loop", TEXT("Patric_Npc/Patric_Npc_Scowl_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Scowl_Start", TEXT("Patric_Npc/Patric_Npc_Scowl_Start.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Talk", TEXT("Patric_Npc/Patric_Npc_Talk.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Thinking_Loop", TEXT("Patric_Npc/Patric_Npc_Thinking_Loop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Patric_Npc_Thinking_Start", TEXT("Patric_Npc/Patric_Npc_Thinking_Start.sqc"), MESH_PATH);
}

void CDefaultSetting::LoadBus()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Bus_Driver", TEXT("Bus/Bus_Driver.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "Bus_Driver_Skeleton", TEXT("Bus/Bus_Driver.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Bus_Driver", "Bus_Driver_Skeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Bus_Driver_Drive", TEXT("Bus/Bus_Driver_Drive.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("Bus_Driver_Stop", TEXT("Bus/Bus_Driver_Stop.sqc"), MESH_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Bus_Stop", TEXT("Bus/BusStop.msh"), MESH_PATH);
}

void CDefaultSetting::LoadStaticNpcs()
{
    // Taxi
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Taxi_Driver", TEXT("Taxi/Taxi.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Taxi_Stop", TEXT("Taxi/TaxiStop.msh"), MESH_PATH);

    // InfoSign
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "InfoSign", TEXT("Static_Npcs/InfoSign.msh"), MESH_PATH);
}

void CDefaultSetting::LoadCommonObj()
{
    // InteractButton
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Button", TEXT("Object/Common/Button.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "ButtonPlate", TEXT("Object/Common/ButtonPlate.msh"), MESH_PATH);

    // Trampoline
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Trampoline", TEXT("Object/Common/Trampoline.msh"), MESH_PATH);
}

void CDefaultSetting::LoadCollectibleItems()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Collectible_Bubble", TEXT("Object/Common/Collectible_Bubble.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "GoldenSpatula", TEXT("Object/Common/GoldenSpatula.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Sock", TEXT("Object/Common/Sock.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "UnderWear", TEXT("Object/Common/UnderWear.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "RedFlower", TEXT("Object/Common/RedFlower.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "YellowFlower", TEXT("Object/Common/YellowFlower.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "GreenFlower", TEXT("Object/Common/GreenFlower.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BlueFlower", TEXT("Object/Common/BlueFlower.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "PurpleFlower", TEXT("Object/Common/PurpleFlower.msh"), MESH_PATH);
}

void CDefaultSetting::LoadBuildings()
{
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BikiniBottomRoad", TEXT("Buildings/BikiniBottom/BikiniBottomRoad.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "PineAppleHouse", TEXT("Buildings/BikiniBottom/PineAppleHouse.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "PatrickHouse", TEXT("Buildings/BikiniBottom/PatrickHouse.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SquidHouse", TEXT("Buildings/BikiniBottom/SquidHouse.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "ChumBucket", TEXT("Buildings/BikiniBottom/ChumBucket.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Buliding_02", TEXT("Buildings/BikiniBottom/Buliding_02.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Buliding_05", TEXT("Buildings/BikiniBottom/Buliding_05.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "krustykrab", TEXT("Buildings/BikiniBottom/krustykrab.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "krustykrab_Enter", TEXT("Buildings/BikiniBottom/krustykrab_Enter.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "krustykrab_Shell", TEXT("Buildings/BikiniBottom/krustykrab_Shell.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "TaxiStop", TEXT("Buildings/BikiniBottom/TaxiStop.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "RedTree", TEXT("Buildings/BikiniBottom/RedTree.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "PurpleTree", TEXT("Buildings/BikiniBottom/PurpleTree.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Rock", TEXT("Buildings/BikiniBottom/Rock.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Rock2", TEXT("Buildings/BikiniBottom/Rock2.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Missile", TEXT("Buildings/BikiniBottom/Missile.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "InfoSign", TEXT("Buildings/BikiniBottom/InfoSign.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DoubleTree", TEXT("Buildings/BikiniBottom/DoubleTree.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "CoconutTree", TEXT("Buildings/BikiniBottom/CoconutTree.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SM_BB_FloatSign_01", TEXT("Buildings/BikiniBottom/SM_BB_FloatSign_01.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "fountain", TEXT("Buildings/BikiniBottom/fountain.msh"));
}

void CDefaultSetting::LoadJellyfishFieldsObj()
{
    // 해파리 동산 맵 메쉬
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishfieldsSign", TEXT("Buildings/JellyfishField/JellyfishfieldsSign.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Water", TEXT("Buildings/JellyfishField/Water.msh"));

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishFieldBoss", TEXT("Buildings/JellyfishField/JellyfishFieldBoss.msh"));
    //CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyFishFieldTestKKB", TEXT("Buildings/JellyfishField/JellyfishScene.msh"));

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishField", TEXT("Buildings/JellyfishField/JellyfishField.msh"));

    //CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishField1", TEXT("Buildings/JellyfishField/JellyfishField1.msh"), MESH_PATH);
    //CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishField2", TEXT("Buildings/JellyfishField/JellyfishField2.msh"), MESH_PATH);
    //CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishField3", TEXT("Buildings/JellyfishField/JellyfishField3.msh"), MESH_PATH);
    //CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishField4", TEXT("Buildings/JellyfishField/JellyfishField4.msh"), MESH_PATH);
    //CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JellyfishField5", TEXT("Buildings/JellyfishField/JellyfishField5.msh"), MESH_PATH);

    if (!CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JFBackground", TEXT("Objects/JellyfishFields/JFBackground.msh"), MESH_PATH))
    {
        CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JFBackground", TEXT("Objects/JellyfishFields/JFBackground.fbx"), MESH_PATH);
    }

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DoubleTree_Red", TEXT("Objects/JellyfishFields/DoubleTree_Red.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DoubleTree_Yellow", TEXT("Objects/JellyfishFields/DoubleTree_Yellow.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "DoubleTree_Purple", TEXT("Objects/JellyfishFields/DoubleTree_Purple.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SmallTree_Red", TEXT("Objects/JellyfishFields/SmallTree_Red.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SmallTree_Purple", TEXT("Objects/JellyfishFields/SmallTree_Purple.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Seaflower", TEXT("Objects/JellyfishFields/Seaflower.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BouncingTree", TEXT("Objects/JellyfishFields/BouncingTree.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Clam", TEXT("Objects/JellyfishFields/Clam.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Bridge", TEXT("Objects/JellyfishFields/Bridge2.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Missile", TEXT("Objects/JellyfishFields/Missile.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Gate", TEXT("Objects/JellyfishFields/Gate.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "WaterFall", TEXT("Objects/JellyfishFields/WaterFall.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "TwoRock", TEXT("Objects/JellyfishFields/TwoRock.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "WaterFallSmall", TEXT("Objects/JellyfishFields/WaterfallSmall.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "WaterFall", TEXT("Buildings/JellyfishField/WaterFall.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "WaterFallSmall", TEXT("Buildings/JellyfishField/WaterFallSmall.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "TwoRock", TEXT("Buildings/JellyfishField/TwoRock.msh"), MESH_PATH);

    // SM_JF_Teeter_Rock
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SM_JF_Teeter_Rock_01", TEXT("Objects/JellyfishFields/SM_JF_Teeter_Rock_01.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "SM_JF_Teeter_Rock_02", TEXT("Objects/JellyfishFields/SM_JF_Teeter_Rock_02.msh"));

    // CheckPoint
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "CheckPointMesh", TEXT("Objects/JellyfishFields/CheckPointMesh.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadSkeleton(nullptr, "CheckPointMeshSkeleton", TEXT("Objects/JellyfishFields/CheckPointMesh.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("CheckPointMesh", "CheckPointMeshSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("CheckPointIdle", TEXT("Objects/JellyfishFields/CheckPointIdle.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("CheckPointPopUp", TEXT("Objects/JellyfishFields/CheckPointPopUp.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("CheckPointInactive", TEXT("Objects/JellyfishFields/CheckPointInactive.sqc"), MESH_PATH);

    // JumpTreeTop == Animation
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "JumpTreeTop", TEXT("Objects/JellyfishFields/JumpTreeTopMesh.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "JumpTreeTopSkeleton", TEXT("Objects/JellyfishFields/JumpTreeTopMesh.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("JumpTreeTop", "JumpTreeTopSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("JumpTreeTop_Bounce", TEXT("Objects/JellyfishFields/JumpTreeTop_Bounce.sqc"), MESH_PATH);

    // JumpTreeBottom == Static
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "JumpTreeBottom", TEXT("Objects/JellyfishFields/JumpTreeBottomMesh.msh"), MESH_PATH);

    // Cannon
    if (!CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Cannon", TEXT("Objects/JellyfishFields/Cannon.msh"), MESH_PATH))
    {
        CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Cannon", TEXT("Objects/JellyfishFields/Cannon.fbx"), MESH_PATH);
    }

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "CannonSkeleton", TEXT("Objects/JellyfishFields/Cannon.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Cannon", "CannonSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Cannon_Shoot", TEXT("Objects/JellyfishFields/Cannon.sqc"), MESH_PATH);

    // Pufferfish
    if (!CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Pufferfish", TEXT("Objects/JellyfishFields/Puffer.msh"), MESH_PATH))
    {
        CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "Pufferfish", TEXT("Objects/JellyfishFields/Puffer.fbx"), MESH_PATH);
    }

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "PufferfishSkeleton", TEXT("Objects/JellyfishFields/Puffer.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("Pufferfish", "PufferfishSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("Pufferfish_Contact", TEXT("Objects/JellyfishFields/Puffer.sqc"), MESH_PATH);

    // Grass
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "Grass1", TEXT("Map/JFF/Grass1.msh"), MESH_PATH);

    // TeleportBox
    if (!CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "TeleportBox", TEXT("Objects/JellyfishFields/TeleportBoxMesh.msh"), MESH_PATH))
    {
        CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Animation, "TeleportBox", TEXT("Objects/JellyfishFields/TeleportBoxMesh.fbx"), MESH_PATH);
    }

    CResourceManager::GetInst()->LoadSkeleton(nullptr, "TeleportBoxSkeleton", TEXT("Objects/JellyfishFields/TeleportBoxMesh.bne"), MESH_PATH);
    CResourceManager::GetInst()->SetMeshSkeleton("TeleportBox", "TeleportBoxSkeleton");

    CResourceManager::GetInst()->LoadAnimationSequence("TeleportBox_Opening", TEXT("Objects/JellyfishFields/TeleportBox_Opening.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("TeleportBox_OpeningLoop", TEXT("Objects/JellyfishFields/TeleportBox_OpeningLoop.sqc"), MESH_PATH);
    CResourceManager::GetInst()->LoadAnimationSequence("TeleportBox_Closed", TEXT("Objects/JellyfishFields/TeleportBox_Closed.sqc"), MESH_PATH);

    // Gate
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "GateArm", TEXT("Objects/JellyfishFields/GateArm.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "GateBottom", TEXT("Objects/JellyfishFields/GateBottom.msh"), MESH_PATH);

    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BossWater", TEXT("Objects/JellyfishFields/BossWater.msh"));
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "BossMapWater", TEXT("Objects/JellyfishFields/BossWater.msh"));

    // Sound
    CResourceManager::GetInst()->LoadSound("Effect", "Box_Teleport", false, "Objects/Box_shuffle_open.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "Gate_Opening", false, "Objects/SFX_GateOpenLong_3sec.ogg", SOUND_PATH);
    CResourceManager::GetInst()->LoadSound("Effect", "CheckPoint", false, "Objects/SFX_CheckPoint.ogg", SOUND_PATH);

}

void CDefaultSetting::LoadCBLabObjects()
{
    // Platform
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "CBL_Platform", TEXT("Map/CBLab/CBLab_Platform.msh"), MESH_PATH);

    // Floor = Ground
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "CBL_InnerFloor", TEXT("Map/CBLab/CBLab_Ground1.msh"), MESH_PATH);
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "CBL_OuterFloor", TEXT("Map/CBLab/CBLab_Ground2.msh"), MESH_PATH);

    // BaseMesh
    CResourceManager::GetInst()->LoadMesh(nullptr, MeshType::Static, "CBL_BaseMesh", TEXT("Map/CBLab/CBLab_BaseMesh.msh"), MESH_PATH);
}