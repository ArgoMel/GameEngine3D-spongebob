#include "CameraManager.h"
#include "../Component/CameraComponent.h"

CCameraManager::CCameraManager()
	: m_Owner(nullptr)
{
}

CCameraManager::~CCameraManager()
{
}

void CCameraManager::SetCurrentCamera(CCameraComponent* camera)
{
	m_CurrentCamera = camera;
}

void CCameraManager::SetUICamera(CCameraComponent* camera)
{
	m_UICamera = camera;
}

void CCameraManager::Start()
{
}

bool CCameraManager::Init()
{
	m_CurrentCamera = new CCameraComponent;
	m_CurrentCamera->Init();
	m_CurrentCamera->SetCameraType(ECameraType::Camera3D);
	m_UICamera = new CCameraComponent;
	m_UICamera->Init();
	m_UICamera->SetCameraType(ECameraType::CameraUI);
	return true;
}

void CCameraManager::Update(float deltaTime)
{
	if (!m_CurrentCamera->GetOwner())
	{
		m_CurrentCamera->Update(deltaTime);
	}
}

void CCameraManager::PostUpdate(float deltaTime)
{
	if (!m_CurrentCamera->GetOwner())
	{
		m_CurrentCamera->PostUpdate(deltaTime);
	}
}

void CCameraManager::Save(FILE* file)
{	
}

void CCameraManager::Load(FILE* file)
{
}
