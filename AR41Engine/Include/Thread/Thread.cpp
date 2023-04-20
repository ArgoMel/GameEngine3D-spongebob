#include "Thread.h"
#include "Sync.h"

CThread::CThread()
    : m_Loop(false)
    , m_Suspend(false)
    , m_StartEvent(nullptr)
    , m_Thread(nullptr)
    , m_CRT{}
{

}

CThread::~CThread()
{
    DeleteCriticalSection(&m_CRT);
}

void CThread::Suspend()
{
    {
        CSync   sync(&m_CRT);
        m_Suspend = true;
    }
    SuspendThread(m_Thread);
}

void CThread::Resume()
{
    CSync   sync(&m_CRT);
    DWORD count = ResumeThread(m_Thread);
    if (count > 0)
    {
        m_Suspend = false;
    }
}

void CThread::ReStart()
{
    CSync   sync(&m_CRT);
    DWORD   count = 0;
    do
    {
        count = ResumeThread(m_Thread);
    } while (count > 0);
    m_Suspend = false;
}

void CThread::Stop()
{
    if (m_Thread)
    {
        m_Loop = false;
        Start();
        ReStart();
        WaitForSingleObject(m_Thread, INFINITE);         // 스레드가 종료될때까지 기다린다.
        CloseHandle(m_Thread);
        m_Thread = 0;
    }
}

void CThread::Start()
{
    SetEvent(m_StartEvent);
}

bool CThread::Init()
{
    m_StartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    InitializeCriticalSection(&m_CRT);
    m_Thread = (HANDLE)_beginthreadex(nullptr, 0, CThread::ThreadFunction, (void*)this, 0, nullptr);
    return true;
}

unsigned int __stdcall CThread::ThreadFunction(void* arg)
{
    CThread* thread = (CThread*)arg;
    WaitForSingleObject(thread->m_StartEvent, INFINITE);
    do
    {
        thread->Run();
    } while (thread->m_Loop);
    return 0;
}
