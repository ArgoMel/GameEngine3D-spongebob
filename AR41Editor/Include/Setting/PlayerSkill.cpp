#include "PlayerSkill.h"
#include "SkillManager.h"
//#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/RigidBody.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"
#include "Component/ParticleComponent.h"
#include "Resource/Animation/SkeletonSocket.h"
#include "../GameObject/Player.h"
#include "../GameObject/Weapon.h"

CPlayerSkill::CPlayerSkill()
{
}

CPlayerSkill::~CPlayerSkill()
{
}

void CPlayerSkill::Init()
{
	CSkillManager::GetInst()->CreateSkill<CPlayerSkill>("SpongebobAttack",this, &CPlayerSkill::SpongebobAttack);
	CSkillManager::GetInst()->CreateSkill<CPlayerSkill>("PatrickAttack",this, &CPlayerSkill::PatrickAttack);
	CSkillManager::GetInst()->CreateSkill<CPlayerSkill>("SandyAttack",this, &CPlayerSkill::SandyAttack);
	CSkillManager::GetInst()->CreateSkill<CPlayerSkill>("SpongebobRClick",this, &CPlayerSkill::SpongebobRClick);
	CSkillManager::GetInst()->CreateSkill<CPlayerSkill>("PatrickRClick",this, &CPlayerSkill::PatrickRClick);
	CSkillManager::GetInst()->CreateSkill<CPlayerSkill>("SandyRClick",this, &CPlayerSkill::SandyRClick);
}

void CPlayerSkill::SpongebobAttack(class CGameObject* obj)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer* player= (CPlayer*)obj;
	scene->GetResource()->SoundPlay("Spongebob_BubbleSpin");
	player->m_Weapon->GetRootComponent()->SetEnable(true);
	player->m_Particle->SetRelativePosition(-50.f, 50.f, -80.f);
	player->m_Particle->ChangeParticle("SpongebobAtk");
	player->m_Anim[(int)EMain_Character::Spongebob]->ChangeAnimation("SpongebobAttack");
}

void CPlayerSkill::PatrickAttack(CGameObject* obj)
{	
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer* player = (CPlayer*)obj;
	if (player->m_Weapon->GetRootComponent()->GetEnable())
	{
		return;
	}
	scene->GetResource()->SoundPlay("Patrick_Attack");
	player->m_Particle->SetRelativePosition(0.f, 0.f, -80.f);
	player->m_Particle->ChangeParticle("PatrickAtk");
	float angle = player->GetWorldRot().y-180.f;
	player->m_Rigid->SetGround(false);
	player->m_Rigid->AddForce(sinf(DegreeToRadian(angle)) * 150.f, 200.f, cosf(DegreeToRadian(angle)) * 150.f);
	player->m_Rigid->SetVelocity(sinf(DegreeToRadian(angle)) * 150.f, 200.f, cosf(DegreeToRadian(angle)) * 150.f);
	player->m_Anim[(int)EMain_Character::Patrick]->ChangeAnimation("PatrickAttack");
}

void CPlayerSkill::SandyAttack(CGameObject* obj)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer* player = (CPlayer*)obj;
	player->m_Particle->ChangeParticle("PatrickAtk");
	if (player->m_Rigid->GetGround())
	{
		scene->GetResource()->SoundPlay("Sandy_Chop");
		player->m_Anim[(int)EMain_Character::Sandy]->ChangeAnimation("SandyAttack");
	}
	else
	{
		scene->GetResource()->SoundPlay("Sandy_Kick");
		player->m_Anim[(int)EMain_Character::Sandy]->ChangeAnimation("SandyKick");
	}
}

void CPlayerSkill::SpongebobRClick(CGameObject* obj)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("Spongebob_BubbleBowl_Charge");
}

void CPlayerSkill::PatrickRClick(CGameObject* obj)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer* player = (CPlayer*)obj;
	if (player->m_Weapon->GetRootComponent()->GetEnable() && !player->m_CanPickUp)
	{
		player->m_IsStop = true;
		scene->GetResource()->SoundPlay("Patrick_Throw");
		player->m_Anim[(int)EMain_Character::Patrick]->ChangeAnimation("PatrickThrow");
	}
	else if (player->m_CanPickUp)
	{
		player->m_IsStop = true;
		scene->GetResource()->SoundPlay("Patrick_Lift");
		player->m_Anim[(int)EMain_Character::Patrick]->ChangeAnimation("PatrickPickUp");
	}
}

void CPlayerSkill::SandyRClick(CGameObject* obj)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer* player = (CPlayer*)obj;
	player->m_IsStop = true;
	scene->GetResource()->SoundPlay("Sandy_LassoAttack");
	player->m_Anim[(int)EMain_Character::Sandy]->ChangeAnimation("SandyLassoStart");
	player->m_LassoDistance = 0.f;
	player->m_Mesh->GetSkeleton()->GetSocket("Weapon")->SetOffset(-5.f, 105.f, 105.f);
	player->m_Weapon->Lasso("Lasso_Start");
	player->m_Weapon->SetAngle(player->GetWorldRot().y);
	player->m_FrontCube->SetEnable(true);
	player->m_FrontCube->SetRelativePositionZ(-80.f);
}
