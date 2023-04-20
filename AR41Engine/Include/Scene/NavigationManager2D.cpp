#include "NavigationManager2D.h"
#include "../Thread/NavigationThread.h"

CNavigationManager2D::CNavigationManager2D()
{
}

CNavigationManager2D::~CNavigationManager2D()
{
}

void CNavigationManager2D::Start()
{
}

bool CNavigationManager2D::Init()
{
    return true;
}

void CNavigationManager2D::Move(CNavigationAgent* agent, const Vector2& end)
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
