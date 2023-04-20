#include "ThreadManager.h"
#include "Navigation2DThread.h"
#include "Navigation3DThread.h"
#include "../Component/SceneComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/NavigationMesh.h"
DEFINITION_SINGLE(CThreadManager)

CThreadManager::CThreadManager()
{
}

CThreadManager::~CThreadManager()
{
	{
		auto	iter = m_mapThread.begin();
		auto	iterEnd = m_mapThread.end();
		for (; iter != iterEnd; ++iter)
		{
			iter->second->Stop();
			SAFE_DELETE(iter->second);
		}
		m_mapThread.clear();
	}
	{
		auto	iter = m_mapCriticalSection.begin();
		auto	iterEnd = m_mapCriticalSection.end();
		for (; iter != iterEnd; ++iter)
		{
			::DeleteCriticalSection(iter->second);	// 전역함수를 호출
			SAFE_DELETE(iter->second);
		}
		m_mapCriticalSection.clear();
	}
}

bool CThreadManager::Init()
{
	CreateCriticalSection("Loading");
	return true;
}

bool CThreadManager::Suspend(const std::string& name)
{
	CThread* thread = FindThread(name);
	if (!thread)
	{
		return false;
	}
	thread->Suspend();
	return true;
}

bool CThreadManager::Resume(const std::string& name)
{
	CThread* thread = FindThread(name);
	if (!thread)
	{
		return false;
	}
	thread->Resume();
	return true;
}

bool CThreadManager::ReStart(const std::string& name)
{
	CThread* thread = FindThread(name);
	if (!thread)
	{
		return false;
	}
	thread->ReStart();
	return true;
}

bool CThreadManager::Delete(const std::string& name)
{
	auto	iter = m_mapThread.find(name);
	if (iter == m_mapThread.end())
	{
		return false;
	}
	iter->second->Stop();
	SAFE_DELETE(iter->second);
	m_mapThread.erase(iter);
	return true;
}

bool CThreadManager::Start(const std::string& name)
{
	CThread* thread = FindThread(name);
	if (!thread)
	{
		return false;
	}
	thread->Start();
	return true;
}

void CThreadManager::CreateNavigationThread(CSceneComponent* navComponent, bool Is2D)
{
	CScene* scene = navComponent->GetScene();
	unsigned __int64	address = (unsigned __int64)scene;
	char	sceneAddress[32] = {};
	sprintf_s(sceneAddress, "%llu", address);
	std::string	name = scene->GetName()+ "_" + navComponent->GetName() + "_"+ sceneAddress;

	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);
	for (DWORD i = 0; i < sysInfo.dwNumberOfProcessors * 2; ++i)
	{
		char	threadName[256] = {};
		sprintf_s(threadName, "%s_%d", name.c_str(), (int)i);
		CNavigationThread* thread = nullptr;
		if (Is2D)
		{
			thread = Create<CNavigation2DThread>(threadName);
		}
		else
		{
			thread = Create<CNavigation3DThread>(threadName);
		}
		thread->SetNavigationComponent(navComponent);
		thread->SetLoop(true);
		thread->Start();
		thread->Suspend();
		scene->GetNavigationManager()->AddNavigationThread(thread);
	}
}

void CThreadManager::DeleteNavigationThread(CSceneComponent* navComponent)
{
	CScene* scene = navComponent->GetScene();
	if (!scene)
	{
		return;
	}
	unsigned __int64	address = (unsigned __int64)scene;
	char	sceneAddress[32] = {};
	sprintf_s(sceneAddress, "%llu", address);
	std::string	name = navComponent->GetSceneName() + "_" + navComponent->GetName() + "_" + sceneAddress;

	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);
	for (DWORD i = 0; i < sysInfo.dwNumberOfProcessors * 2; ++i)
	{
		char	threadName[256] = {};
		sprintf_s(threadName, "%s_%d", name.c_str(), (int)i);
		CThread* thread = FindThread(threadName);
		if (!thread)
		{
			continue;
		}
		thread->ReStart();
		Delete(threadName);
	}
}

void CThreadManager::CreateNavigationThread(CNavigationMesh* navMesh)
{
	CScene* scene = navMesh->GetScene();
	unsigned __int64	address = (unsigned __int64)scene;
	char	sceneAddress[32] = {};
	sprintf_s(sceneAddress, "%llu", address);
	std::string	name = navMesh->GetSceneName() + "_" + navMesh->GetComponentName() + "_" + sceneAddress;
	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);
	for (DWORD i = 0; i < sysInfo.dwNumberOfProcessors * 2; ++i)
	{
		char	threadName[256] = {};
		sprintf_s(threadName, "%s_%d", name.c_str(), (int)i);
		CNavigation3DThread* thread = nullptr;
		thread = Create<CNavigation3DThread>(threadName);
		thread->SetNavigationMesh(navMesh);
		thread->SetLoop(true);
		thread->Start();
		thread->Suspend();
		scene->GetNavigationManager()->AddNavigationThread(thread);
	}
}

void CThreadManager::DeleteNavigationThread(CNavigationMesh* navMesh)
{
	CScene* scene = navMesh->GetScene();
	if (!scene)
	{
		return;
	}
	unsigned __int64	address = (unsigned __int64)scene;
	char	sceneAddress[32] = {};
	sprintf_s(sceneAddress, "%llu", address);
	std::string	name = navMesh->GetSceneName() + "_" + navMesh->GetComponentName() + "_" + sceneAddress;
	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);
	for (DWORD i = 0; i < sysInfo.dwNumberOfProcessors * 2; ++i)
	{
		char	threadName[256] = {};
		sprintf_s(threadName, "%s_%d", name.c_str(), (int)i);
		CThread* thread = FindThread(threadName);
		if (!thread)
		{
			continue;
		}
		thread->ReStart();
		Delete(threadName);
	}
}

CThread* CThreadManager::FindThread(const std::string& name)
{
	auto	iter = m_mapThread.find(name);
	if (iter == m_mapThread.end())
	{
		return nullptr;
	}
	return iter->second;
}

bool CThreadManager::CreateCriticalSection(const std::string& name)
{
	CRITICAL_SECTION* crt = FindCriticalSection(name);
	if (crt)
	{
		return false;
	}
	crt = new CRITICAL_SECTION;
	InitializeCriticalSection(crt);
	m_mapCriticalSection.insert(std::make_pair(name, crt));
	return true;
}

bool CThreadManager::DeleteCriticalSection(const std::string& name)
{
	auto	iter = m_mapCriticalSection.find(name);
	if (iter == m_mapCriticalSection.end())
	{
		return false;
	}
	::DeleteCriticalSection(iter->second);
	SAFE_DELETE(iter->second);
	m_mapCriticalSection.erase(iter);
	return true;
}

CRITICAL_SECTION* CThreadManager::FindCriticalSection(const std::string& name)
{
	auto	iter = m_mapCriticalSection.find(name);
	if (iter == m_mapCriticalSection.end())
	{
		return nullptr;
	}
	return iter->second;
}
