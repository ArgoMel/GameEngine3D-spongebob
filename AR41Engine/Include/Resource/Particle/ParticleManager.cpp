#include "ParticleManager.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
}

bool CParticleManager::Init()
{
	CreateParticle("LoadingBubble");
	CParticle* particle = FindParticle("LoadingBubble");
	particle->SetMaterial("Bubble");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleStartMin(Vector3(-100.f, -100.f, -100.f));
	particle->SetParticleStartMax(Vector3(100.f, 100.f, 100.f));
	particle->SetParticleSpawnCountMax(1000);
	particle->SetParticleScaleMin(Vector3(10.f, 10.f, 10.f));
	particle->SetParticleScaleMax(Vector3(40.f, 40.f, 40.f));
	particle->SetParticleLifeTimeMin(2.f);
	particle->SetParticleLifeTimeMax(3.f);
	particle->SetParticleColorMin(Vector4(1.f, 0.f, 0.f, 1.f));
	particle->SetParticleColorMax(Vector4(0.f, 1.f, 0.f, 1.f));
	particle->SetParticleSpeedMin(100.f);
	particle->SetParticleSpeedMax(200.f);
	particle->SetParticleMoveEnable(true);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDir(Vector3(0.f, 1.f, 0.f));
	particle->SetParticleMoveDirEnable(true);
	particle->SetParticleMoveAngle(Vector3(0.f, 0.f, 0.f));

	CreateParticle("SpongebobAtk");
	particle = FindParticle("SpongebobAtk");
	particle->SetMaterial("Bubble");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleSpawnCountMax(1000);
	particle->SetParticleScaleMin(Vector3(10.f, 10.f, 10.f));
	particle->SetParticleScaleMax(Vector3(40.f, 40.f, 40.f));
	particle->SetParticleLifeTimeMin(0.5f);
	particle->SetParticleLifeTimeMax(1.f);
	particle->SetParticleSpeedMin(100.f);
	particle->SetParticleSpeedMax(200.f);
	particle->SetParticleMoveEnable(true);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDirEnable(true);
	particle->SetParticleMoveDir(Vector3(1.f, 1.f, -1.f));

	CreateParticle("BashBubble");
	particle = FindParticle("BashBubble");
	particle->SetMaterial("Bubble");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleStartMin(Vector3(-100.f, 0.f, -100.f));
	particle->SetParticleStartMax(Vector3(100.f, 0.f, 100.f));
	particle->SetParticleSpawnCountMax(1000);
	particle->SetParticleScaleMin(Vector3(30.f, 30.f, 30.f));
	particle->SetParticleScaleMax(Vector3(60.f, 60.f, 60.f));
	particle->SetParticleLifeTimeMin(0.5f);
	particle->SetParticleLifeTimeMax(1.f);
	particle->SetParticleSpeedMin(100.f);
	particle->SetParticleSpeedMax(200.f);
	particle->SetParticleMoveEnable(true);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDirEnable(true);
	particle->SetParticleMoveAngle(Vector3(45.f, 45.f, 45.f));

	CreateParticle("PatrickAtk");
	particle = FindParticle("PatrickAtk");
	particle->SetMaterial("ShockWave");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleStartMin(Vector3(0.f, 50.f, 0.f));
	particle->SetParticleStartMax(Vector3(0.f, 50.f, 0.f));
	particle->SetParticleSpawnCountMax(1);
	particle->SetParticleScaleMin(Vector3(300.f, 300.f, 300.f));
	particle->SetParticleScaleMax(Vector3(300.f, 300.f, 300.f));
	particle->SetParticleLifeTimeMin(1.f);
	particle->SetParticleLifeTimeMax(1.f);
	particle->SetParticleCamDir(EParticle_Cam_Dir::Side);

    // �� �ٴڿ��� �ö���� ��ǰ ��ƼŬ
    CreateParticle("GroundBubble");
    particle = FindParticle("GroundBubble");
    particle->SetMaterial("Bubble");
    particle->SetParticleSpawnTime(0.8f);
    particle->SetParticleStartMin(Vector3(-10.f, -10.f, -10.f));
    particle->SetParticleStartMax(Vector3(10.f, 10.f, 10.f));
    particle->SetParticleSpawnCountMax(1000);
    particle->SetParticleScaleMin(Vector3(5.f, 5.f, 5.f));
    particle->SetParticleScaleMax(Vector3(60.f, 60.f, 60.f));
    particle->SetParticleLifeTimeMin(8.f);
    particle->SetParticleLifeTimeMax(12.f);
    particle->SetParticleColorMin(Vector4(0.5f, 0.f, 1.f, 0.5f));
    particle->SetParticleColorMax(Vector4(0.7f, 0.f, 1.f, 0.7f));
    particle->SetParticleSpeedMin(40.f);
    particle->SetParticleSpeedMax(60.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(0.f, 1.f, 0.f));
    particle->SetParticleMoveDirEnable(true);
    particle->SetParticleMoveAngle(Vector3(0.f, 0.f, 5.f));

    // ���Ͱ� �׾��� �� �����Ǵ� ��ǰ ��ƼŬ
    CreateParticle("MonsterBubble");
    particle = FindParticle("MonsterBubble");
    particle->SetMaterial("Bubble");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(-50.f, -50.f, -50.f));
    particle->SetParticleStartMax(Vector3(50.f, 200.f, 50.f));
    particle->SetParticleSpawnCountMax(100);
    particle->SetParticleScaleMin(Vector3(5.f, 5.f, 5.f));
    particle->SetParticleScaleMax(Vector3(60.f, 60.f, 60.f));
    particle->SetParticleLifeTimeMin(1.f);
    particle->SetParticleLifeTimeMax(2.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleSpeedMin(120.f);
    particle->SetParticleSpeedMax(200.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(1.f, 1.f, 1.f));
    particle->SetParticleMoveDirEnable(true);
    particle->SetParticleMoveAngle(Vector3(-90.f, 5.f, 125.f));

    // tikis�� ����� �� �����Ǵ� ��ƼŬ
    CreateParticle("Glow");
    particle = FindParticle("Glow");
    particle->SetMaterial("Glow");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(20.f, 20.f, 20.f));
    particle->SetParticleStartMax(Vector3(40.f, 40.f, 40.f));
    particle->SetParticleSpawnCountMax(5);
    particle->SetParticleScaleMin(Vector3(25.f, 25.f, 25.f));
    particle->SetParticleScaleMax(Vector3(30.f, 30.f, 30.f));
    particle->SetParticleLifeTimeMin(0.3f);
    particle->SetParticleLifeTimeMax(1.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 0.f, 0.5f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 0.f, 0.5f));
    particle->SetParticleSpeedMin(0.f);
    particle->SetParticleSpeedMax(2.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(1.f, 1.f, 1.f));
    particle->SetParticleMoveDirEnable(false);
    particle->SetParticleMoveAngle(Vector3(5.f, 5.f, 5.f));

    // �κ� �������� �� ��ƼŬ
    CreateParticle("Fire");
    particle = FindParticle("Fire");
    particle->SetMaterial("Fire");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(-250.f, 0.f, -250.f));
    particle->SetParticleStartMax(Vector3(250.f, 0.f, 250.f));
    particle->SetParticleSpawnCountMax(4);
    particle->SetParticleScaleMin(Vector3(800.f, 800.f, 800.f));
    particle->SetParticleScaleMax(Vector3(2400.f, 2400.f, 2400.f));
    particle->SetParticleLifeTimeMin(2.f);
    particle->SetParticleLifeTimeMax(4.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 1.f, 0.5f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 0.5f));
    particle->SetParticleSpeedMin(2.f);
    particle->SetParticleSpeedMax(4.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(-1.f, 0.f, 1.f));
    particle->SetParticleMoveDirEnable(true);
    particle->SetParticleMoveAngle(Vector3(45.f, 5.f, 45.f));

    // �κ� �������� �� ��ƼŬ2
    CreateParticle("Fire2");
    particle = FindParticle("Fire2");
    particle->SetMaterial("Fire2");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(-250.f, 0.f, -250.f));
    particle->SetParticleStartMax(Vector3(250.f, 0.f, 250.f));
    particle->SetParticleSpawnCountMax(4);
    particle->SetParticleScaleMin(Vector3(1800.f, 1800.f, 1800.f));
    particle->SetParticleScaleMax(Vector3(2400.f, 2400.f, 2400.f));
    particle->SetParticleLifeTimeMin(2.f);
    particle->SetParticleLifeTimeMax(4.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 1.f, 0.5f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 0.5f));
    particle->SetParticleSpeedMin(2.f);
    particle->SetParticleSpeedMax(4.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(-1.f, 0.f, 1.f));
    particle->SetParticleMoveDirEnable(true);
    particle->SetParticleMoveAngle(Vector3(45.f, 5.f, 45.f));

    // ���ø�ī��Ʈ�� ���� ��ƼŬ
    CreateParticle("Explosion");
    particle = FindParticle("Explosion");
    particle->SetMaterial("Flare");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleStartMax(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleSpawnCountMax(1);
    particle->SetParticleScaleMin(Vector3(1500.f, 1500.f, 1500.f));
    particle->SetParticleScaleMax(Vector3(1500.f, 1500.f, 1500.f));
    particle->SetParticleLifeTimeMin(3.f);
    particle->SetParticleLifeTimeMax(4.f);
    particle->SetParticleColorMin(Vector4(1.f, 0.3f, 0.f, 1.f));
    particle->SetParticleColorMax(Vector4(1.f, 0.3f, 0.f, 1.f));
    particle->SetParticleSpeedMin(0.f);
    particle->SetParticleSpeedMax(2.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(0.f, -1.f, 0.f));
    particle->SetParticleMoveDirEnable(false);
    particle->SetParticleMoveAngle(Vector3(5.f, 5.f, 5.f));

    // ���ø�ī��Ʈ�� �������� ��ƼŬ
    CreateParticle("AfterExplosion");
    particle = FindParticle("AfterExplosion");
    particle->SetMaterial("Glow");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(-50.f, -50.f, -50.f));
    particle->SetParticleStartMax(Vector3(50.f, 200.f, 50.f));
    particle->SetParticleSpawnCountMax(100);
    particle->SetParticleScaleMin(Vector3(15.f, 15.f, 15.f));
    particle->SetParticleScaleMax(Vector3(15.f, 30.f, 15.f));
    particle->SetParticleLifeTimeMin(1.f);
    particle->SetParticleLifeTimeMax(2.f);
    particle->SetParticleColorMin(Vector4(1.f, 0.5f, 0.f, 1.f));
    particle->SetParticleColorMax(Vector4(1.f, 0.5f, 0.f, 1.f));
    particle->SetParticleSpeedMin(120.f);
    particle->SetParticleSpeedMax(200.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(1.f, 1.f, 1.f));
    particle->SetParticleMoveDirEnable(true);
    particle->SetParticleMoveAngle(Vector3(-90.f, 5.f, 125.f));

    // ����, �ܸ� ���� ��ƼŬ
    CreateParticle("MonsterExplosion");
    particle = FindParticle("MonsterExplosion");
    particle->SetMaterial("Flare");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleStartMax(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleSpawnCountMax(1);
    particle->SetParticleScaleMin(Vector3(1500.f, 1500.f, 1500.f));
    particle->SetParticleScaleMax(Vector3(1500.f, 1500.f, 1500.f));
    particle->SetParticleLifeTimeMin(3.f);
    particle->SetParticleLifeTimeMax(4.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 0.f, 1.f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 0.f, 1.f));
    particle->SetParticleSpeedMin(0.f);
    particle->SetParticleSpeedMax(2.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(0.f, -1.f, 0.f));
    particle->SetParticleMoveDirEnable(false);
    particle->SetParticleMoveAngle(Vector3(5.f, 5.f, 5.f));

    // �ٴ� ����
    CreateParticle("Glister");
    particle = FindParticle("Glister");
    particle->SetMaterial("Glow");
    particle->SetParticleSpawnTime(0.1f);
    particle->SetParticleStartMin(Vector3(-2000.f, 0.f, -2000.f));
    particle->SetParticleStartMax(Vector3(2000.f, 10.f, 2000.f));
    particle->SetParticleSpawnCountMax(10000);
    particle->SetParticleScaleMin(Vector3(10.f, 50.f, 50.f));
    particle->SetParticleScaleMax(Vector3(70.f, 60.f, 100.f));
    particle->SetParticleLifeTimeMin(3.f);
    particle->SetParticleLifeTimeMax(4.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 1.f, 0.f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleSpeedMin(2.f);
    particle->SetParticleSpeedMax(4.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(1.f, 1.f, 1.f));
    particle->SetParticleMoveDirEnable(true);
    particle->SetParticleMoveAngle(Vector3(5.f, 5.f, 5.f));

    // ���� ����ǰ
    CreateParticle("Foam");
    particle = FindParticle("Foam");
    particle->SetMaterial("Glow");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(-15.f, 0.f, -15.f));
    particle->SetParticleStartMax(Vector3(15.f, 0.f, 15.f));
    particle->SetParticleSpawnCountMax(1000);
    particle->SetParticleScaleMin(Vector3(10.f, 10.f, 10.f));
    particle->SetParticleScaleMax(Vector3(10.f, 10.f, 10.f));
    particle->SetParticleLifeTimeMin(1.5f);
    particle->SetParticleLifeTimeMax(2.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleSpeedMin(8.f);
    particle->SetParticleSpeedMax(10.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(0.f, 1.f, 0.f));
    particle->SetParticleMoveDirEnable(true);
    particle->SetParticleMoveAngle(Vector3(25.f, 0.f, 25.f));

    // ���� ���ٱ�
    CreateParticle("Shower");
    particle = FindParticle("Shower");
    particle->SetMaterial("Shower");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleStartMax(Vector3(50.f, 0.f, 50.f));
    particle->SetParticleSpawnCountMax(100);
    particle->SetParticleScaleMin(Vector3(50.f, 50.f, 50.f));
    particle->SetParticleScaleMax(Vector3(50.f, 50.f, 50.f));
    particle->SetParticleLifeTimeMin(4.f);
    particle->SetParticleLifeTimeMax(6.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleSpeedMin(15.f);
    particle->SetParticleSpeedMax(15.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(true);
    particle->SetParticleMoveDir(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleMoveDirEnable(false);
    particle->SetParticleMoveAngle(Vector3(0.f, 0.f, 0.f));

    // ���� ���� �ٴ��� ��� �κп� ����� ��
    CreateParticle("WaterRing");
    particle = FindParticle("WaterRing");
    particle->SetMaterial("WaterRing");
    particle->SetParticleSpawnTime(0.05f);
    particle->SetParticleStartMin(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleStartMax(Vector3(10.f, 0.f, 10.f));
    particle->SetParticleSpawnCountMax(3);
    particle->SetParticleScaleMin(Vector3(200.f, 200.f, 200.f));
    particle->SetParticleScaleMax(Vector3(300.f, 300.f, 300.f));
    particle->SetParticleLifeTimeMin(4.f);
    particle->SetParticleLifeTimeMax(6.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleSpeedMin(2.f);
    particle->SetParticleSpeedMax(3.f);
    particle->SetParticleMoveEnable(false);
    particle->SetParticleGravityEnable(false);
    particle->SetParticleMoveDir(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleMoveDirEnable(false);
    particle->SetParticleMoveAngle(Vector3(0.f, 0.f, 0.f));

    // ���� ���ٱ�
    CreateParticle("Shower");
    particle = FindParticle("Shower");
    particle->SetMaterial("Shower");
    particle->SetParticleSpawnTime(0.01f);
    particle->SetParticleStartMin(Vector3(0.f, 0.f, 0.f));
    particle->SetParticleStartMax(Vector3(5.f, 0.f, 5.f));
    particle->SetParticleSpawnCountMax(30);
    particle->SetParticleScaleMin(Vector3(1000.f, 1000.f, 1000.f));
    particle->SetParticleScaleMax(Vector3(1000.f, 1000.f, 1000.f));
    particle->SetParticleLifeTimeMin(4.f);
    particle->SetParticleLifeTimeMax(6.f);
    particle->SetParticleColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 1.f));
    particle->SetParticleSpeedMin(5.f);
    particle->SetParticleSpeedMax(5.f);
    particle->SetParticleMoveEnable(true);
    particle->SetParticleGravityEnable(true);
    particle->SetParticleMoveDir(Vector3(0.f, 0.f, 0.f));
	return true;
}

bool CParticleManager::CreateParticle(const std::string& name)
{
	CParticle* particle = FindParticle(name);
	if (particle)
	{
		return false;
	}
	particle = new CParticle;
	particle->SetName(name);
	if (!particle->Init())
	{
		SAFE_RELEASE(particle);
		return false;
	}
	m_mapParticle.insert(std::make_pair(name, particle));
	return true;
}

CParticle* CParticleManager::FindParticle(const std::string& name)
{
	auto	iter = m_mapParticle.find(name);
	if (iter == m_mapParticle.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CParticleManager::ReleaseParticle(const std::string& name)
{
	auto	iter = m_mapParticle.find(name);
	if (iter != m_mapParticle.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapParticle.erase(iter);
		}
	}
}