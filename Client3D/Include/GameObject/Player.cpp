#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"
#include "PatrickObject.h"
#include "SpongebobMissile.h"
#include "Input.h"
#include "Engine.h"
#include "Device.h"
#include "PathManager.h"
#include "SkillManager.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/RigidBody.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"
#include "Component/ParticleComponent.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Scene/NavigationManager3D.h"
#include "Resource/Material/Material.h"
#include "Resource/Animation/SkeletonSocket.h"
#include "Animation/Animation.h"
#include "../UI/PauseUI.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"

CPlayer::CPlayer()
	: m_Speed(500.f)
	, m_InflictAngle(0.f)
	, m_SpaceTime(1.f)
	, m_LassoDistance(0.f)
	, m_KeyCount(0)
	, m_MainCharacter(EMain_Character::Max)
	, m_BellyAttackTime(0.f)
	, m_IsLoading(false)
	, m_IsDoubleJump(false)
	, m_OnCollision(false)
	, m_CanPickUp(false)
	, m_IsStop(false)
{
	SetTypeID<CPlayer>();
	m_ObjectTypeName = "Player";
}

CPlayer::CPlayer(const CPlayer& Obj) 
	: CGameObject(Obj)
	, m_Speed(Obj.m_Speed)
	, m_InflictAngle(0.f)
	, m_SpaceTime(1.f)
	, m_LassoDistance(0.f)
	, m_KeyCount(0)
	, m_MainCharacter(EMain_Character::Max)
	, m_IsDoubleJump(false)
	, m_OnCollision(false)
	, m_CanPickUp(false)
	, m_IsStop(false)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Arm = (CTargetArm*)FindComponent("Arm");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
	m_Cube = (CColliderOBB3D*)FindComponent("Cube");
	m_HeadCube = (CColliderCube*)FindComponent("HeadCube");
	m_TailCube = (CColliderCube*)FindComponent("TailCube");
	m_FrontCube = (CColliderOBB3D*)FindComponent("FrontCube");
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CPlayer::~CPlayer()
{
	if (m_LoadData != m_PlayerData)
	{
		SaveCharacter();
	}
}

void CPlayer::Destroy()
{
	CGameObject::Destroy();
	if (m_PlayerUI)
	{
		m_PlayerUI->Destroy();
	}
	if (m_PauseUI)
	{
		m_PauseUI->Destroy();
	}
	if (m_Scene)
	{
		m_Scene->GetResource()->SoundPlay(m_Scene->GetName());
	}
	CInput::GetInst()->ClearCallback();
}

void CPlayer::Start()
{
	CGameObject::Start();
	if (!m_Scene)
	{
		return;
	}
	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

	m_Scene->GetResource()->SoundPlay(m_Scene->GetName());

	CInput::GetInst()->AddBindFunction<CPlayer>("W", Input_Type::Stay, this, &CPlayer::MoveFront, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("S", Input_Type::Stay, this, &CPlayer::MoveBack, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("A", Input_Type::Stay, this, &CPlayer::MoveLeft, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("D", Input_Type::Stay, this, &CPlayer::MoveRight, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("W", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("S", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("A", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("D", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("W", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("S", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("A", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("D", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Space", Input_Type::Down, this, &CPlayer::JumpDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Space", Input_Type::Stay, this, &CPlayer::JumpPush, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Space", Input_Type::Up, this, &CPlayer::JumpUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("E", Input_Type::Down, this, &CPlayer::Headbutt, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Q", Input_Type::Down, this, &CPlayer::Missile, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Tab", Input_Type::Down, this, &CPlayer::IngameUI, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("LClick", Input_Type::Down, this, &CPlayer::LClick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Down, this, &CPlayer::RClickDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Stay, this, &CPlayer::RClickPush, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Up, this, &CPlayer::RClickUp, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("F1", Input_Type::Down, this, &CPlayer::ChangeCharacter, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F2", Input_Type::Down, this, &CPlayer::Reset, m_Scene);
	//CInput::GetInst()->AddBindFunction<CPlayer>("F7", Input_Type::Down, this, &CPlayer::DebugF7, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F8", Input_Type::Down, this, &CPlayer::DebugF8, m_Scene);

	m_Cube->SetCollisionCallback<CPlayer>(ECollision_Result::Collision, this, &CPlayer::CollisionTest);
	m_Cube->SetCollisionCallback<CPlayer>(ECollision_Result::Release, this, &CPlayer::CollisionTestOut);
	m_HeadCube->SetCollisionCallback<CPlayer>(ECollision_Result::Collision, this, &CPlayer::CollisionCube);
	m_TailCube->SetCollisionCallback<CPlayer>(ECollision_Result::Collision, this, &CPlayer::CollisionCube);
	m_FrontCube->SetCollisionCallback<CPlayer>(ECollision_Result::Collision, this, &CPlayer::CollisionCube);

	m_PlayerUI = m_Scene->GetViewport()->CreateUIWindow<CPlayerUI>("PlayerUI");
	m_PauseUI = m_Scene->GetViewport()->CreateUIWindow<CPauseUI>("PauseUI");
	m_DialogUI = m_Scene->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	m_Scene->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");

	if (m_IsLoading)
	{
		CGameObject* delObj = m_Scene->FindObject("Temp");
		delObj->Destroy();
	}
	else
	{
		LoadCheck();
	}
	ChangeCharacter();
	Reset();
}

bool CPlayer::Init()
{
	CGameObject::Init();
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");
	m_Cube = CreateComponent<CColliderOBB3D>("Cube");
	m_HeadCube = CreateComponent<CColliderCube>("HeadCube");
	m_TailCube = CreateComponent<CColliderCube>("TailCube");
	m_FrontCube = CreateComponent<CColliderOBB3D>("FrontCube");
	m_Particle = CreateComponent<CParticleComponent>("Particle");

	m_Mesh->AddChild(m_Rigid);
	m_Mesh->AddChild(m_Arm);
	m_Mesh->AddChild(m_Cube);
	m_Mesh->AddChild(m_HeadCube);
	m_Mesh->AddChild(m_TailCube);
	m_Mesh->AddChild(m_FrontCube);
	m_Mesh->AddChild(m_Particle);
	m_Arm->AddChild(m_Camera);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);

	m_Arm->SetTargetOffset(0.f, 150.f, 0.f);

	m_Rigid->SetGravity(true);

	m_Cube->SetRelativePositionY(70.f);
	m_Cube->SetCollisionProfile("Player");
	m_Cube->SetBoxHalfSize(50.f, 60.f, 20.f);
	m_Cube->SetInheritRotX(true);
	m_Cube->SetInheritRotY(true);
	m_Cube->SetInheritRotZ(true);

	m_HeadCube->SetCollisionProfile("PlayerAttack");
	m_HeadCube->SetRelativePositionY(175.f);
	m_HeadCube->SetCubeSize(100.f, 75.f, 100.f);

	m_TailCube->SetCollisionProfile("PlayerAttack");
	m_TailCube->SetRelativePositionY(25.f);

	m_FrontCube->SetCollisionProfile("PlayerAttack");
	m_FrontCube->SetRelativePosition(0.f, 70.f, -80.f);
	m_FrontCube->SetBoxHalfSize(60.f, 40.f, 60.f);
	m_FrontCube->SetInheritRotY(true);

	m_Particle->SetInheritRotY(true);
	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	CameraRotationKey();
	JumpCheck();
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name+"LassoStart")
	{
		m_LassoDistance -= 500.f * DeltaTime;
		if (m_LassoDistance < -400.f)
		{
			m_Mesh->GetSkeleton()->GetSocket("Weapon")->SetOffset(-5.f, 105.f, m_LassoDistance + 505.f);
			m_FrontCube->AddRelativePositionZ((m_LassoDistance * 2.5f) * DeltaTime);
		}
	}
	if (m_FrontCube->GetEnable() && m_Name == "Spongebob")
	{
		AddRelativeRotationY(-300.f* DeltaTime);
	}
}

CPlayer* CPlayer::Clone() const
{
	return new CPlayer(*this);
}

void CPlayer::Save(FILE* File)
{
	CGameObject::Save(File);
	fwrite(&m_RespawnPos, sizeof(Vector3), 1, File);
	SaveCharacter();
}

void CPlayer::Load(FILE* File)
{
	CGameObject::Load(File);
	fread(&m_RespawnPos, sizeof(Vector3), 1, File);
	m_IsLoading = true;
	LoadCheck();
}

int CPlayer::InflictDamage(float damage)
{
	int hp = AddItem(EItemList::UnderWear,-(int)damage);
	m_IsStop = true;
	m_Cube->SetEnable(false);
	if (hp <= 0)	//사망
	{
		m_Scene->GetResource()->SoundStop(m_Name + "_Walk");
		m_Scene->GetResource()->SoundPlay(m_Name + "_Death");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"Death");
	}
	else //피격 모션
	{		
		m_Scene->GetResource()->SoundStop(m_Name + "_Walk");
		m_Scene->GetResource()->SoundPlay(m_Name + "_Damage");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"Hit");
		float angle = m_InflictAngle==0.f?  GetWorldRot().y: m_InflictAngle;
		m_InflictAngle = 0.f;
		m_Rigid->SetGround(false);
		m_Rigid->AddForce(sinf(DegreeToRadian(angle))*400.f, 250.f, cosf(DegreeToRadian(angle))*400.f);
		m_Rigid->SetVelocity(sinf(DegreeToRadian(angle))*400.f, 250.f, cosf(DegreeToRadian(angle))*400.f);
	}
	return m_PlayerData.CurHP;
}

void CPlayer::Reset()
{
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name+"MissileLoop")
	{
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
		m_Scene->GetResource()->SoundStop(m_Name+"_BubbleLoop");
		m_Scene->GetResource()->SoundPlay(m_Name+"_BubbleExplode");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"MissileEnd");
		return;
	}
	m_PlayerData.CurHP = m_PlayerData.MaxHP;
	m_PlayerUI->SetHp(m_PlayerData.CurHP);
	m_PlayerUI->SetMaxHp(m_PlayerData.MaxHP);
	m_PlayerUI->SetGlitter(m_PlayerData.ShinyFlower);
	m_PlayerUI->SetFritter(m_PlayerData.Spatula);
	m_PlayerUI->SetSocks(m_PlayerData.Socks);
	SetWorldPosition(m_RespawnPos);
	ResetIdle();
}

bool CPlayer::SaveCharacter()
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(SAVE_PATH);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, "userData.pref");
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return false;
	}
	fwrite(&m_PlayerData.MaxHP, 4, 1, file);
	fwrite(&m_PlayerData.CurHP, 4, 1, file);
	fwrite(&m_PlayerData.Socks, 4, 1, file);
	fwrite(&m_PlayerData.Spatula, 4, 1, file);
	fwrite(&m_PlayerData.ShinyFlower, 4, 1, file);
	fclose(file);
	return true;
}

bool CPlayer::LoadCharacter()
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(SAVE_PATH);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, "userData.pref");
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	fread(&m_PlayerData.MaxHP, 4, 1, file);
	fread(&m_PlayerData.CurHP, 4, 1, file);
	if(m_PlayerData.CurHP<3)
	{
		m_PlayerData.CurHP = 3;
	}
	fread(&m_PlayerData.Socks, 4, 1, file);
	fread(&m_PlayerData.Spatula, 4, 1, file);
	fread(&m_PlayerData.ShinyFlower, 4, 1, file);
	fclose(file);
	m_LoadData = m_PlayerData;
	return true;
}

int CPlayer::AddItem(const EItemList& Item, int Count)
{
	int result=0;
	switch (Item)
	{
	case EItemList::GoldenSpatula:
		SetSpatula(m_PlayerData.Spatula + Count);
		result = m_PlayerData.Spatula;
		break;
	case EItemList::ShinyFlower:
		SetShinyFlower(m_PlayerData.ShinyFlower + Count);
		result = m_PlayerData.ShinyFlower;
		break;
	case EItemList::Sock:
		SetSocks(m_PlayerData.Socks + Count);
		result = m_PlayerData.Socks;
		break;
	case EItemList::UnderWear:
		SetCurHP(m_PlayerData.CurHP + Count);
		result = m_PlayerData.CurHP;
		break;
	}
	return result;
}

void CPlayer::LoadCheck()
{
	LoadAnim();
	LoadCharacter();
}

void CPlayer::KeyDown()
{
	if (m_IsStop || !m_Rigid->GetGround())
	{
		return;
	}
	if (m_KeyCount == 0)
	{
		m_Scene->GetResource()->SoundStop(m_Name + "_Walk");
		m_Scene->GetResource()->SoundPlay(m_Name + "_Walk");
	}
	++m_KeyCount;
}

void CPlayer::MoveFront()
{
	float angle = m_Camera->GetWorldRot().y;
	Move(angle);
}

void CPlayer::MoveBack()
{
	float angle = m_Camera->GetWorldRot().y - 180.f;
	Move(angle);
}

void CPlayer::MoveLeft()
{
	float angle = m_Camera->GetWorldRot().y - 90.f;
	Move(angle);
}

void CPlayer::MoveRight()
{
	float angle = m_Camera->GetWorldRot().y + 90.f;
	Move(angle);
}

void CPlayer::Move(float angle)
{
	if (m_IsStop || 0.f == CEngine::GetInst()->GetTimeScale())
	{
		return;
	}
	if (m_WallCollision.dest)
	{
		int differ = (int)abs(GetWorldRot().y - angle) % 360;
		if (differ > 100 && differ < 200)
		{
			return;
		}
		else if (differ <= 100 || (differ >= 200 && differ < 280))
		{
			float distance = m_Cube->GetInfo().length[0] * 0.85f;
			AddWorldPosition(GetWorldAxis(AXIS::AXIS_Z) * distance);
		}
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name + "Idle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name + "Walk");
	}
	else if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name + "PickUpIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name + "PickUpWalk");
	}
	SetWorldRotationY(angle + 180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CPlayer::KeyUp()
{
	--m_KeyCount;
	if (m_IsStop)
	{
		return;
	}
	if (m_KeyCount <= 0)
	{
		m_KeyCount = 0;
		m_Scene->GetResource()->SoundStop(m_Name + "_Walk");
		ResetIdle();
	}
}

void CPlayer::JumpDown()
{
	if(m_IsDoubleJump|| m_IsStop)
	{
		return;
	}
	if (m_Name=="Patrick"&&m_Weapon->GetRootComponent()->GetEnable())
	{
		return;
	}
	m_Scene->GetResource()->SoundStop(m_Name + "_Walk");
	m_Scene->GetResource()->SoundPlay(m_Name + "_Jump");
	if (!m_Rigid->GetGround() && !m_IsDoubleJump)
	{
		m_IsDoubleJump = true;
	}
	m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"JumpUp");
	m_Rigid->SetGround(false);
	m_Rigid->AddForce(0, 400.f);
	m_Rigid->AddVelocity(Vector3(0.f,400.f,0.f)); 
}

void CPlayer::JumpPush()
{
	if (m_Name != "Sandy")
	{
		return;
	}
	if(m_SpaceTime>0.f&&!m_Rigid->GetGround())
	{
		m_SpaceTime -= g_DeltaTime;
		if (m_SpaceTime <= 0.f)
		{
			m_Scene->GetResource()->SoundPlay(m_Name+"_Hovering");
			m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"SwingLoop");
			m_Weapon->Lasso("Lasso_Copter");
			m_Mesh->GetSkeleton()->GetSocket("Weapon")->SetOffset(-5.f, 180.f, 0.f);
		}
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name+"SwingLoop")
	{
		m_Rigid->SetGravityForce(-150.f);
	}
}

void CPlayer::JumpUp()
{
	if (m_Name != "Sandy")
	{
		return;
	}
	m_Scene->GetResource()->SoundStop(m_Name+"_Hovering");
	m_SpaceTime = 1.f;
	m_Rigid->SetGravityForce(-500.f);
	m_Weapon->ResetIdle();
	m_Mesh->GetSkeleton()->GetSocket("Weapon")->SetOffset(-5.f, 105.f, 105.f);
	if(m_Rigid->GetGround())
	{
		ResetIdle();
	}
	else
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"JumpDw");
	}
}

void CPlayer::JumpCheck()
{
	CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
	float Y = Nav->GetHeight(GetWorldPos());
	//땅에 붙어있을때만 높이 갱신
	if (m_Rigid->GetGround()&& !m_OnCollision)
	{
		if (Y != FLT_MAX)
		{
			SetWorldPositionY(Y);
		}
	}
	//플레이어의 속력이 음수가 되면 애니메이션 전환
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name+"JumpUp"
		&& m_Rigid->GetVelocity().y < 0.f)
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"JumpDw");
	}
	//아래로 내려오고 있을때 현재 높이와 지형의 차이가 30미만이면 아이들로 전환
	if (m_Rigid->GetVelocity().y < 0.f)
	{
		if (Y != FLT_MAX && GetWorldPos().y - Y < 30.f)
		{
			SetWorldPositionY(Y);
			m_Rigid->SetGround(true);
			BashCheck();
			if(m_KeyCount>0)
			{
				m_Scene->GetResource()->SoundPlay(m_Name + "_Walk");
			}
		}
	}
}

void CPlayer::CameraRotationKey()
{
	Vector2 mouseMove = CInput::GetInst()->GetMouseMove() * m_Camera->GetCameraSpeed() * g_DeltaTime;
	mouseMove.x = m_Camera->GetCameraHorizon() ? mouseMove.x : -mouseMove.x;
	mouseMove.y=m_Camera->GetCameraVertical() ? mouseMove.y : -mouseMove.y;
	m_Arm->AddRelativeRotationY(mouseMove.x);
	m_Arm->AddRelativeRotationX(mouseMove.y);
	if (m_Arm->GetRelativeRot().x > 70.f)
	{
		m_Arm->SetRelativeRotationX(70.f);
	}
	else if (m_Arm->GetRelativeRot().x < -20.f)
	{
		m_Arm->SetRelativeRotationX(-20.f);
	}
}

void CPlayer::Headbutt()
{
	if (m_IsStop|| m_Name != "Spongebob")
	{
		return;
	}
	if (!m_Rigid->GetGround()|| CEngine::GetInst()->GetTimeScale() == 0.f)
	{
		return;
	}
	m_IsStop = true;
	m_Scene->GetResource()->SoundStop(m_Name+"_WalkLeft");
	m_Scene->GetResource()->SoundPlay(m_Name+"_DoubleJump");
	m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"JumpUp");
	m_Rigid->SetGround(false);
	m_Rigid->AddForce(0, 800.f);
	m_Rigid->SetVelocityY(800.f);
	m_HeadCube->SetEnable(true);
}

void CPlayer::Missile()
{
	if (m_IsStop || m_Name != "Spongebob")
	{
		return;
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name+"Idle")
	{
		m_Scene->GetResource()->SoundPlay(m_Name+"_BubbleCharge");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"MissileStart");
	}
}

void CPlayer::MissileLaunch()
{
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name+"MissileStart")
	{
		m_IsStop = true;
		m_Scene->GetResource()->SoundPlay(m_Name+"_BubbleLaunch");
		m_Scene->GetResource()->SoundPlay(m_Name+"_BubbleLoop");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"MissileLoop");
		CSpongebobMissile* missile= m_Scene->CreateObject<CSpongebobMissile>("SpongebobMissile");
		float angle = GetWorldRot().y;
		missile->SetWorldPosition(GetWorldPos());
		missile->SetAngle(angle);
	}
}

void CPlayer::Patrick_PickUp()
{
	m_Weapon->GetRootComponent()->SetEnable(true);
}

void CPlayer::Patrick_Throw()
{
	CPatrickObject* PatrickObject = m_Scene->CreateObject<CPatrickObject>("PatrickObject");
	float angle = GetWorldRot().y;
	PatrickObject->SetWorldPosition(GetWorldPos());
	PatrickObject->AddWorldPositionY(100.f);
	PatrickObject->SetAngle(angle - 180.f);
	m_Weapon->GetRootComponent()->SetEnable(false);
}

void CPlayer::IngameUI()
{
	m_PlayerUI->SetAllOpacity(3.f);
}

void CPlayer::LClick()
{
	if (m_IsStop)
	{
		return;
	}
	if (m_DialogUI->GetIsActive()||CEngine::GetInst()->GetTimeScale() == 0.f
		|| m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName()== m_Name + "Attack")
	{
		return;
	}
	m_FrontCube->SetRelativeRotationY(0.f);
	m_FrontCube->SetRelativePositionZ(-80.f);
	m_FrontCube->SetEnable(true);
	CSkillManager::GetInst()->ActivateSkill(m_Name+"Attack",this);
}

void CPlayer::RClickDown()
{
	if (m_DialogUI->GetIsActive() || m_IsStop||
		CEngine::GetInst()->GetTimeScale() == 0.f)
	{
		return;
	}
	if (m_Rigid->GetGround())
	{
		if(m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name+"Idle"||
			m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name + "PickUpIdle")
		{
			CSkillManager::GetInst()->ActivateSkill(m_Name + "RClick", this);
			return;
		}
	}
	if (m_Name == "Sandy")
	{
		return;
	}
	else if(m_Name=="Patrick" && m_Weapon->GetRootComponent()->GetEnable())
	{
		return;
	}
	m_Scene->GetResource()->SoundPlay(m_Name + "_Bash");
	m_Rigid->SetGravity(false);
	m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"BashStart");
}

void CPlayer::RClickPush()
{
	if (m_IsStop|| !m_Rigid->GetGround())
	{
		return;
	}
	if (m_DialogUI->GetIsActive() || CEngine::GetInst()->GetTimeScale() == 0.f)
	{
		return;
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "SpongebobIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("SpongebobBowl");
	}
}

void CPlayer::RClickUp()
{	
	m_Scene->GetResource()->SoundStop("Spongebob_BubbleBowl_Charge");
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "SpongebobBowl")
	{
		m_IsStop = true;
		m_Scene->GetResource()->SoundPlay("Spongebob_BubbleBowl_Throw");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("SpongebobBowlThrow");
		CBullet* bullet = m_Scene->CreateObject<CBullet>("SpongeBobBowl");
		float angle = GetWorldRot().y;
		bullet->SetWorldPosition(GetWorldPos());
		bullet->SetAngle(angle);
	}
}

void CPlayer::StartBash()
{
	m_Rigid->SetGravity(true);
	m_Rigid->AddForce(0, -10000.f);
	m_Rigid->SetVelocityY(-10000.f);
	m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"BashDw");
}

void CPlayer::BashCheck()
{
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == m_Name+"BashDw")
	{
		m_Scene->GetResource()->SoundStop(m_Name+"_Bash");
		m_Particle->SetParticle("BashBubble");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"Bash");
		m_TailCube->SetEnable(true);
		m_IsStop = true;
	}
	else
	{
		ResetIdle();
	}
}

void CPlayer::ResetIdle()
{
	if (m_Name == "Spongebob")
	{
		m_Weapon->GetRootComponent()->SetEnable(false);
	}
	m_FrontCube->SetEnable(false);
	m_Particle->SetRelativePosition(0.f, 0.f, 0.f);
	m_Particle->DeleteCurrentParticle();
	if (!m_Rigid->GetGround())
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"JumpDw");
		return;
	}
	if (m_Weapon->GetRootComponent()->GetEnable() && m_Name == "Patrick")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"PickUpIdle");
	}
	else
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation(m_Name+"Idle");
	}
	m_HeadCube->SetEnable(false);
	m_TailCube->SetEnable(false);
	m_Cube->SetEnable(true);
	m_Rigid->SetVelocity(0.f, 0.f, 0.f);
	m_IsDoubleJump = false;
	m_IsStop = false;
}

void CPlayer::SetCharName(size_t flag)
{
	if (flag >= (int)EMain_Character::Max)
	{
		flag = 0;
	}
	m_MainCharacter = static_cast<EMain_Character>(flag);

	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		m_Name = "Spongebob";
		break;
	case EMain_Character::Patrick:
		m_Name = "Patrick";
		break;
	case EMain_Character::Sandy:
		m_Name = "Sandy";
		break;
	}
}

void CPlayer::ChangeCharacter()
{
	SetCharName((int)m_MainCharacter+1);
	m_Mesh->SetAnimation(m_Anim[(int)m_MainCharacter]);
	m_Mesh->ClearMaterial();
	m_Mesh->SetMesh(m_ReserveMesh[(int)m_MainCharacter]);
	m_Anim[(int)m_MainCharacter]->Start();

	if (m_Weapon)
	{
		m_Weapon->Destroy();
		m_Weapon = m_Scene->CreateObject<CWeapon>("Temp");
		AddChildToSocket("Weapon", m_Weapon);
		m_Weapon->SetMesh(m_Name + "Weapon");
		m_Weapon->GetRootComponent()->SetEnable(false);
	}
	else
	{
		m_Weapon = m_Scene->CreateObject<CWeapon>("Temp");
		AddChildToSocket("Weapon", m_Weapon);
	}

	if(m_Name=="Patrick")
	{
		m_TailCube->SetCubeSize(500.f, 50.f, 500.f);
	}
	else if(m_Name == "Sandy")
	{
		m_Mesh->GetMaterial(1)->SetOpacity(0.5f);
		m_Mesh->GetMaterial(1)->SetRenderState("AlphaBlend");
		m_Mesh->GetMaterial(1)->SetEmissiveColor(0.1f, 0.1f, 0.1f, 0.5f);
		m_TailCube->SetCubeSize(300.f, 50.f, 300.f);
	}
	else
	{
		m_TailCube->SetCubeSize(300.f, 50.f, 300.f);
	}
}

void CPlayer::DebugF7()
{
	InflictDamage();
}

void CPlayer::DebugF8()
{
	m_HeadCube->SetEnable(false);
	m_TailCube->SetEnable(false);
	m_FrontCube->SetEnable(false);
	m_Particle->SetRelativePosition(0.f, 0.f, 0.f);
	m_Particle->DeleteCurrentParticle();
	m_Rigid->SetVelocity(0.f, 0.f, 0.f);
	m_IsDoubleJump = false;
	m_IsStop = false;

	m_Cube->SetEnable(false);
	m_Anim[(int)m_MainCharacter]->Stop();
}

void CPlayer::CollisionTest(const CollisionResult& result)
{
	std::string name = result.dest->GetCollisionProfile()->name;
	if (name == "Wall")
	{
		float height = GetWorldPos().y>m_PrevPos.y? GetWorldPos().y : m_PrevPos.y;
		height-=result.hitPoint.y;
		if(height>=-10.f)
		{
			m_OnCollision = true;
			m_Rigid->SetGround(true);
			BashCheck();
		}
		else if (GetWorldPos().y > m_PrevPos.y)
		{
			m_Rigid->SetVelocity(0.f, 0.f, 0.f);
		}
		else
		{
			m_OnCollision = false;
			m_WallCollision = result;
		}
	}
}

void CPlayer::CollisionTestOut(const CollisionResult& result)
{
	m_WallCollision.dest = nullptr;
	m_WallCollision.src = nullptr;
	m_WallCollision.hitPoint = Vector3(0.f,0.f,0.f);
	m_OnCollision = false;

	//충돌 해제중에 씬전환하면 컴포넌트에 쓰레기값이 들어있어서 에러
	if (result.dest->GetName() == "InfoSignCollider")
	{
		return;
	}

	m_Rigid->SetGround(false);
	m_Scene->GetResource()->SoundStop(m_Name + "_Walk");
}

void CPlayer::CollisionCube(const CollisionResult& result)
{
	std::string name = result.dest->GetCollisionProfile()->name;
	if(name == "Monster")
	{
		result.dest->GetOwner()->InflictDamage();
	}
}

void CPlayer::LoadAnim()
{
	size_t size = (size_t)EMain_Character::Max;
	for (size_t i = 0; i < size;++i)
	{
		SetCharName(i);
		m_ReserveMesh[i] = CResourceManager::GetInst()->FindMesh(m_Name);
		if (m_ReserveMesh[i])
		{
			m_ReserveMesh[i]->AddRenderCount();
		}
		m_Mesh->SetMesh(m_ReserveMesh[i]);
		m_Anim[i] = m_Mesh->SetAnimation<CAnimation>(m_Name+"Animation");
		m_Anim[i]->AddAnimation(m_Name+"Idle", m_Name + "_Idle", 1.f, 1.f, true);
		m_Anim[i]->AddAnimation(m_Name+"Walk", m_Name + "_Walk", 1.f, 1.f, true);
		m_Anim[i]->AddAnimation(m_Name+"Attack", m_Name + "_Attack", 1.f, 1.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>(m_Name + "Attack", this, &CPlayer::ResetIdle);
		m_Anim[i]->AddAnimation(m_Name+"JumpDw", m_Name + "_JumpDw", 1.f, 1.f, false);
		m_Anim[i]->AddAnimation(m_Name+"JumpUp", m_Name + "_JumpUp", 1.f, 1.f, false);
		m_Anim[i]->AddAnimation(m_Name+"BashStart", m_Name + "_BashStart", 1.f, 2.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>(m_Name + "BashStart", this, &CPlayer::StartBash);
		m_Anim[i]->AddAnimation(m_Name+"BashDw", m_Name + "_BashDw", 1.f, 1.f, true);
		m_Anim[i]->AddAnimation(m_Name+"Bash", m_Name + "_Bash", 1.f, 2.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>(m_Name + "Bash", this, &CPlayer::ResetIdle);
		m_Anim[i]->AddAnimation(m_Name + "Hit", m_Name + "_Hit", 1.f, 1.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>(m_Name + "Hit", this, &CPlayer::ResetIdle);
		m_Anim[i]->AddAnimation(m_Name + "Death", m_Name + "_Death", 1.f, 1.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>(m_Name + "Death", this, &CPlayer::Reset);
		//spongebob
		m_Anim[i]->AddAnimation("SpongebobBowl", "Spongebob_Bowl", 1.f, 1.f, true);
		m_Anim[i]->AddAnimation("SpongebobBowlThrow", "Spongebob_BowlThrow", 1.f, 1.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>("SpongebobBowlThrow", this, &CPlayer::ResetIdle);
		m_Anim[i]->AddAnimation("SpongebobMissileStart", "Spongebob_MissileStart", 1.f, 1.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>("SpongebobMissileStart", this, &CPlayer::MissileLaunch);
		m_Anim[i]->AddAnimation("SpongebobMissileLoop", "Spongebob_MissileLoop", 1.f, 1.f, true);
		m_Anim[i]->AddAnimation("SpongebobMissileEnd", "Spongebob_MissileEnd", 1.f, 1.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>("SpongebobMissileEnd", this, &CPlayer::ResetIdle);
		//patrick
		m_Anim[i]->AddAnimation("PatrickPickUpIdle", "Patrick_PickUpIdle", 1.f, 1.f, true);
		m_Anim[i]->AddAnimation("PatrickPickUpWalk", "Patrick_PickUpWalk", 1.f, 1.f, true);
		m_Anim[i]->AddAnimation("PatrickPickUp", "Patrick_PickUp", 1.f, 1.f, false);
		m_Anim[i]->AddCurrentNotify<CPlayer>("PatrickPickUp", "PatrickPickUp", 0.6f, this, &CPlayer::Patrick_PickUp);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>("PatrickPickUp", this, &CPlayer::ResetIdle);
		m_Anim[i]->AddAnimation("PatrickThrow", "Patrick_Throw", 1.f, 1.f, false);
		m_Anim[i]->AddCurrentNotify<CPlayer>("PatrickThrow", "PatrickThrow", 0.6f, this, &CPlayer::Patrick_Throw);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>("PatrickThrow", this, &CPlayer::ResetIdle);
		//sandy
		m_Anim[i]->AddAnimation("SandyKick", "Sandy_Karate_Kick", 1.f, 1.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>("SandyKick", this, &CPlayer::ResetIdle);
		m_Anim[i]->AddAnimation("SandyLassoStart", "Sandy_Lasso_Start", 1.f, 1.f, false);
		m_Anim[i]->SetCurrentEndFunction<CPlayer>("SandyLassoStart", this, &CPlayer::ResetIdle);
		m_Anim[i]->AddAnimation("SandySwingLoop", "Sandy_Swing_Loop", 1.f, 1.f, true);
	}
}