#include "CollisionSection2D.h"
#include "../Component/Collider2D.h"

CCollisionSection2D::CCollisionSection2D()
	: m_IndexX(0)
	, m_IndexY(0)
	, m_Index(0)
{
}

CCollisionSection2D::~CCollisionSection2D()
{
}

void CCollisionSection2D::AddCollider(CCollider2D* collider)
{
	m_ColliderList.push_back(collider);
	collider->AddSectionIndex(m_Index);
}

void CCollisionSection2D::Collision(float deltaTime)
{
	if (m_ColliderList.size() < 2)
	{
		return;
	}
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();
	--iterEnd;
	auto	iter1End = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		CCollider* src = *iter;
		auto	iter1 = iter;
		++iter1;
		for (; iter1 != iter1End; ++iter1)
		{
			CCollider* dest = *iter1;
			int	OverlapSection = src->CheckOverlapSection(dest);
			// -1이면 두 물체는 겹치는 영역이 1개라는 의미이기 때문에 무조건 처리해야 한다.
			// 그래서 -1이 아니고 겹치는 인덱스중 가장 작은 인덱스가 현재 영역의 인덱스보다 작다면
			// 이미 앞에서 두 물체는 충돌처리를 수행했다는 의미이므로 현재 영역에서는 충돌처리를 패스한다.
			if (OverlapSection != -1 && OverlapSection < m_Index)
			{
				continue;
			}
			CollisionProfile* srcProfile = src->GetCollisionProfile();
			CollisionProfile* destProfile = dest->GetCollisionProfile();
			if (srcProfile->vecCollisionInteraction[(int)destProfile->channel->channel] == ECollision_Interaction::Ignore 
				||destProfile->vecCollisionInteraction[(int)srcProfile->channel->channel] == ECollision_Interaction::Ignore)
			{
				continue;
			}
			if (src->Collision(dest))
			{
				if (!src->CheckPrevCollision(dest))
				{
					src->AddPrevCollisionCollider(dest);
					dest->AddPrevCollisionCollider(src);
					src->SetCollisionResultDest(dest);
					dest->SetCollisionResultDest(src);
					src->CallCollisionCallback(ECollision_Result::Collision);
					dest->CallCollisionCallback(ECollision_Result::Collision);
				}
			}
			// 이전 프레임에 충돌된 목록에 Dest가 있다면 이 둘은 이전프레임에 
			// 충돌이 되었다가 지금 떨어지게된다는 것을 의미한다.
			else if (src->CheckPrevCollision(dest))
			{
				// 이전프레임에 충돌한 목록에서 서로를 제거한다.
				src->DeletePrevCollisionCollider(dest);
				dest->DeletePrevCollisionCollider(src);
				src->SetCollisionResultDest(dest);
				dest->SetCollisionResultDest(src);
				src->CallCollisionCallback(ECollision_Result::Release);
				dest->CallCollisionCallback(ECollision_Result::Release);
			}
		}
	}
}

CCollider* CCollisionSection2D::CollisionMouse(const Vector2& mouseWorldPos, float deltaTime)
{
	if (m_ColliderList.empty())
	{
		return nullptr;
	}
	if (m_ColliderList.size() > 1)
	{
		m_ColliderList.sort(CCollisionSection2D::ColliderSort);
	}
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->CollisionMouse(mouseWorldPos))
		{
			return *iter;
		}
	}
	return nullptr;
}

bool CCollisionSection2D::ColliderSort(CCollider2D* src, CCollider2D* dest)
{
	return src->GetMin().y < dest->GetMin().y;
}