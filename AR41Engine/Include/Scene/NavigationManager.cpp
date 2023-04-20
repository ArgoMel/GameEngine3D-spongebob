#include "NavigationManager.h"
#include "../Thread/NavigationThread.h"

CNavigationManager::CNavigationManager()
    : m_Owner(nullptr)
{
}

CNavigationManager::~CNavigationManager()
{
}

bool CNavigationManager::CreateNavigationMesh(CSceneComponent* component)
{
    return false;
}

void CNavigationManager::Move(CNavigationAgent* agent, const Vector2& end)
{
}

void CNavigationManager::Move(CNavigationAgent3D* agent, const Vector3& end)
{
}

float CNavigationManager::GetHeight(const Vector3& pos)
{
    return 0.0f;
}

bool CNavigationManager::GetPickingPos(Vector3& result) const
{
    return false;
}
