#include "Collider.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneCollision.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Engine.h"
#include "../CollisionManager.h"

CCollider::CCollider()
{
	SetTypeID<CCollider>();
	m_ComponentTypeName = "Collider";
	m_Result.src = this;
	m_MouseCollision = false;
}

CCollider::CCollider(const CCollider& component)
	: CSceneComponent(component)
{
	m_Mesh = component.m_Mesh;
	m_Color = component.m_Color;
	m_ColliderType = component.m_ColliderType;
	m_Profile = component.m_Profile;
	m_Result.src = this;
	m_MouseCollision = false;
}

CCollider::~CCollider()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeletePrevCollisionCollider(this);
		(*iter)->m_Result.src = *iter;
		(*iter)->m_Result.dest = this;
		(*iter)->CallCollisionCallback(ECollision_Result::Release);
		m_Result.src = this;
		m_Result.dest = *iter;
		CallCollisionCallback(ECollision_Result::Release);
	}
}

void CCollider::DeletePrevCollisionCollider(CCollider* collider)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (*iter == collider)
		{
			m_PrevCollisionList.erase(iter);
			return;
		}
	}
}

bool CCollider::CheckPrevCollision(CCollider* collider)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (*iter == collider)
		{
			return true;
		}
	}
	return false;
}

void CCollider::CheckPrevCollisionColliderSection()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();
	for (; iter != iterEnd;)
	{
		bool	check = false;
		auto	iterIndex = m_CurrentSectionList.begin();
		auto	iterIndexEnd = m_CurrentSectionList.end();
		for (; iterIndex != iterIndexEnd; ++iterIndex)
		{
			// 이전 충돌물체의 충돌영역 목록을 반복한다.
			auto	iterDestIndex = (*iter)->m_CurrentSectionList.begin();
			auto	iterDestIndexEnd = (*iter)->m_CurrentSectionList.end();
			for (; iterDestIndex != iterDestIndexEnd; ++iterDestIndex)
			{
				if (*iterIndex == *iterDestIndex)
				{
					check = true;
					break;
				}
			}
			if (check)
			{
				break;
			}
		}
		// 서로 겹치는 충돌영역이 없으면 이전 프레임에 충돌하고 있다가 서로 다른 영역으로 충돌체가 포함된 것이다.
		// 둘이 충돌이 해제되었다는 통보를 해주어야 한다.
		if (!check)
		{
			m_Result.src = this;
			m_Result.dest = *iter;
			CallCollisionCallback(ECollision_Result::Release);
			(*iter)->m_Result.src = *iter;
			(*iter)->m_Result.dest = this;
			(*iter)->CallCollisionCallback(ECollision_Result::Release);
			(*iter)->DeletePrevCollisionCollider(this);
			iter = m_PrevCollisionList.erase(iter);
			iterEnd = m_PrevCollisionList.end();
			continue;
		}
		++iter;
	}
}

void CCollider::SendPrevCollisionEnd()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeletePrevCollisionCollider(this);
		(*iter)->m_Result.src = *iter;
		(*iter)->m_Result.dest = this;
		(*iter)->CallCollisionCallback(ECollision_Result::Release);
		m_Result.src = this;
		m_Result.dest = *iter;
		CallCollisionCallback(ECollision_Result::Release);
	}
	m_PrevCollisionList.clear();
}

void CCollider::CallCollisionCallback(ECollision_Result type)
{
	if (m_CollisionCallback[(int)type])
	{
		m_CollisionCallback[(int)type](m_Result);
	}
}

void CCollider::CallCollisionMouseCallback(ECollision_Result type)
{
	if (type == ECollision_Result::Release)
	{
		m_MouseCollision = false;
	}
	if (m_CollisionMouseCallback[(int)type])
	{
		m_CollisionMouseCallback[(int)type](m_MouseResult);
	}
}

void CCollider::ClearFrame()
{
	m_CurrentSectionList.clear();
}

int CCollider::CheckOverlapSection(CCollider* dest)
{
	int	overlapCount = 0;
	int	minIndex = INT_MAX;
	auto	iter1 = m_CurrentSectionList.begin();
	auto	iter1End = m_CurrentSectionList.end();
	for (; iter1 != iter1End; ++iter1)
	{
		auto	iter2 = dest->m_CurrentSectionList.begin();
		auto	iter2End = dest->m_CurrentSectionList.end();
		for (; iter2 != iter2End; ++iter2)
		{
			if (*iter1 == *iter2)
			{
				++overlapCount;
				if (*iter1 < minIndex)
				{
					minIndex = *iter1;
				}
				break;
			}
		}
	}
	if (overlapCount < 2)
	{
		return -1;
	}
	return minIndex;
}

void CCollider::SetCollisionProfile(const std::string& name)
{
	m_Profile = CCollisionManager::GetInst()->FindProfile(name);
}

void CCollider::Start()
{
	CSceneComponent::Start();
	if (CEngine::GetEditorMode())
	{
		CRenderManager::GetInst()->AddRenderList(this);
	}
	if (m_Scene)
	{
		m_Scene->GetCollisionManager()->AddCollider(this);
	}
}

bool CCollider::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}
	m_Profile = CCollisionManager::GetInst()->FindProfile("Default");
	if (CEngine::GetEditorMode())
	{
		m_Shader = CResourceManager::GetInst()->FindShader("ColliderShader");
	}
	return true;
}

void CCollider::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CCollider::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CCollider::Render()
{
	CSceneComponent::Render();
	if (m_PrevCollisionList.empty() && !m_MouseCollision)
	{
		m_Color = Vector4(0.f, 1.f, 0.f, 1.f);
	}
	else
	{
		m_Color = Vector4(1.f, 0.f, 0.f, 1.f);
	}
}

void CCollider::Save(FILE* file)
{
	CSceneComponent::Save(file);
	fwrite(&m_ColliderType, sizeof(ECollider_Type), 1, file);
	int	Length = (int)m_Profile->name.size();
	fwrite(&Length, sizeof(int), 1, file);
	fwrite(m_Profile->name.c_str(), 1, Length, file);
}

void CCollider::Load(FILE* file)
{
	CSceneComponent::Load(file);
	fread(&m_ColliderType, sizeof(ECollider_Type), 1, file);
	int	length = 0;
	char	name[256] = {};
	fread(&length, sizeof(int), 1, file);
	assert(length >= 0);
	fread(name, 1, length, file);
	SetCollisionProfile(name);
	if (CEngine::GetEditorMode())
	{
		m_Shader = CResourceManager::GetInst()->FindShader("ColliderShader");
	}
}
