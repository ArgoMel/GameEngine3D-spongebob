#include "LoadingThread.h"
#include "DataStream.h"
#include "../PathManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

CLoadingThread::CLoadingThread()
	: m_LoadComplete(false)
{
	m_PathName = SCENE_PATH;
}

CLoadingThread::~CLoadingThread()
{
}

void CLoadingThread::Run()
{
	CSceneManager::GetInst()->CreateNextScene(false);
	const PathInfo* Info = CPathManager::GetInst()->FindPath(m_PathName);
	m_FullPath = Info->pathMultibyte + m_FileName;
	CSceneManager::GetInst()->GetNextScene()->SetLoadingCallback<CLoadingThread>(this, &CLoadingThread::LoadingCallback);
	CSceneManager::GetInst()->GetNextScene()->Load(m_FullPath.c_str());
	m_LoadComplete = true;
}

void CLoadingThread::LoadingCallback(float rate)
{
	unsigned char	buffer[1024] = {};
	CDataStream	stream;
	stream.SetBuffer(buffer);
	stream.AddData<float>(&rate, 4);
	m_Queue.push((int)ELoadingHeader::LoadingRate, stream.GetSize(),buffer);

	Sleep(100);
}