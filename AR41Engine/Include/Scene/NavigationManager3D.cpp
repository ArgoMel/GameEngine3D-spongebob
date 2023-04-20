#include "NavigationManager3D.h"
#include "NavigationMesh.h"
#include "Scene.h"
#include "CameraManager.h"
#include "../Input.h"
#include "../Thread/NavigationThread.h"
#include "../Thread/Navigation3DThread.h"
#include "../Thread/Navigation3D.h"
#include "../Thread/ThreadManager.h"
#include "../Component/CameraComponent.h"

CNavigationManager3D::CNavigationManager3D()
    : m_Terrain(nullptr)
    , m_NavigationMesh(nullptr)
{
}

CNavigationManager3D::~CNavigationManager3D()
{
    SAFE_DELETE(m_NavigationMesh);
}

void CNavigationManager3D::AddNavigationThread(CNavigationThread* thread)
{
    CNavigationManager::AddNavigationThread(thread);
}

void CNavigationManager3D::Start()
{
}

bool CNavigationManager3D::Init()
{
    return true;
}

bool CNavigationManager3D::CreateNavigationMesh(CSceneComponent* component)
{
    m_Terrain = (CTerrainComponent*)component;
    m_NavigationMesh = new CNavigationMesh;
    m_NavigationMesh->m_Mgr = this;
    m_NavigationMesh->CreateNavigation(m_Terrain);
    CThreadManager::GetInst()->CreateNavigationThread(m_NavigationMesh);
    return true;
}

void CNavigationManager3D::Move(CNavigationAgent3D* agent, const Vector3& end)
{
    if (m_vecNavThread.empty())
    {
        return;
    }
    // 가지고 있는 스레드에서 가장 일이 적게 들어있는 스레드를 판단한다.
    int selectThread = 0;
    int workCount = m_vecNavThread[0]->GetWorkCount();
    if (workCount > 0)
    {
        size_t  size = m_vecNavThread.size();
        for (size_t i = 1; i < size; ++i)
        {
            if (workCount > m_vecNavThread[i]->GetWorkCount())
            {
                workCount = m_vecNavThread[i]->GetWorkCount();
                selectThread = (int)i;
                if (workCount == 0)
                {
                    break;
                }
            }
        }
    }
    m_vecNavThread[selectThread]->AddInputData(agent, end);
}

float CNavigationManager3D::GetHeight(const Vector3& pos)
{
    if(!m_NavigationMesh)
    {
        return FLT_MIN;
    }
    return m_NavigationMesh->GetHeight(pos);
}

bool CNavigationManager3D::GetPickingPos(Vector3& result) const
{
    if (!m_NavigationMesh)
    {
        return false;
    }
    // 뷰행렬을 얻어온다.
    Matrix  matView = m_Owner->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
    matView.Inverse();
    Ray ray = CInput::GetInst()->GetRay();
    ray.pos = ray.pos.TransformCoord(matView);
    ray.dir = ray.dir.TransformNormal(matView);
    return m_NavigationMesh->GetPickingPos(result, ray);
}
