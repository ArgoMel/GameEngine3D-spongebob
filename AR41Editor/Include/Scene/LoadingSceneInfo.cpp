#include "LoadingSceneInfo.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "Thread/ThreadManager.h"
#include "Thread/LoadingThread.h"
#include "Thread/ThreadQueue.h"
#include "Thread/DataStream.h"
#include "Editor/EditorGUIManager.h"
#include "../UI/LoadingUI.h"
#include "Component/ParticleComponent.h"
#include "Component/SpriteComponent.h"
#include "GameObject/GameObject.h"

#ifdef __has_include
#	if __has_include("../Window/ObjectWindow.h")
#		include "../Window/ObjectWindow.h"
//#		define have_ObjectWindow 1
//#	else
//#		define have_ObjectWindow 0
#	endif
#endif

CLoadingSceneInfo::CLoadingSceneInfo()
	: m_LoadingThread(nullptr)
	, m_LoadingQueue(nullptr)
	, m_LoadingUI(nullptr)
	, m_StartTime(0.f)
{
	m_ClassTypeName = "LoadingSceneInfo";
}

CLoadingSceneInfo::~CLoadingSceneInfo()
{
	m_Owner->GetResource()->SoundStop("LoadingUI");
}

bool CLoadingSceneInfo::Init()
{
	m_LoadingUI = m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");
	CGameObject* bg = m_Owner->CreateObject<CGameObject>("BG");
	CSpriteComponent* img = bg->CreateComponent<CSpriteComponent>("BG");
	img->SetTexture("LoadingBG", TEXT("UI\\load_bg.tga"));
	img->SetWorldScale(1920.f, 1080.f);
	img->SetWorldPosition(-960.f, -540.f);
	m_Particle = m_Owner->CreateObject<CGameObject>("Particle");
	m_Particle->CreateComponent<CParticleComponent>("Particle");
	m_Particle->SetEnable(false);
	return true;
}

void CLoadingSceneInfo::Update(float deltaTime)
{
	CSceneInfo::Update(deltaTime);
	if (m_Owner->GetSky())
	{
		m_Owner->ClearSky();
	}
	m_StartTime += deltaTime;
	if (m_StartTime > 3.f && m_StartTime < 100.f)
	{
		m_Owner->GetResource()->SoundPlay("LoadingUI");
		m_Particle->SetEnable(true);
		m_StartTime = 100.f;
	}
	else if (m_StartTime > 103.f)
	{
		CParticleComponent* particle = (CParticleComponent*)m_Particle->GetRootComponent();
		particle->DeleteCurrentParticle();
	}

	if (!m_LoadingQueue->empty())
	{
		int	header;
		int size;
		unsigned char	data[1024] = {};
		m_LoadingQueue->pop(&header, &size, data);
		CDataStream	stream;
		stream.SetBuffer(data);
		float	rate = 0.f;
		stream.GetData<float>(&rate, 4);
		m_LoadingUI->SetLoadingPercent(rate);
	}
	if (m_LoadingThread->IsLoadComplete())
	{
		CScene* nextScene = CSceneManager::GetInst()->GetNextScene();
		nextScene->GetSceneInfo()->SetFileName(m_FileName);
		nextScene->GetSceneInfo()->SetPrevFileName(m_PrevFileName);
		CSceneManager::GetInst()->ChangeNextScene();
		CThreadManager::GetInst()->Delete("Loading");
#if __has_include("../Window/ObjectWindow.h")
		CObjectWindow* objectWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
		if (objectWindow)
		{
			objectWindow->AddInput(nextScene);
			objectWindow->AddItemList(nextScene);
		}
#endif
	}
}

void CLoadingSceneInfo::SceneChangeComplete()
{
	CSceneInfo::SceneChangeComplete();
	m_LoadingThread = CThreadManager::GetInst()->Create<CLoadingThread>("Loading");
	m_LoadingThread->SetLoadingSceneFileName(m_FileName);
	m_LoadingQueue = m_LoadingThread->GetQueue();
	m_LoadingThread->Start();
}
