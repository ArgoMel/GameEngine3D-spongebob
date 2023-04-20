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
			// -1�̸� �� ��ü�� ��ġ�� ������ 1����� �ǹ��̱� ������ ������ ó���ؾ� �Ѵ�.
			// �׷��� -1�� �ƴϰ� ��ġ�� �ε����� ���� ���� �ε����� ���� ������ �ε������� �۴ٸ�
			// �̹� �տ��� �� ��ü�� �浹ó���� �����ߴٴ� �ǹ��̹Ƿ� ���� ���������� �浹ó���� �н��Ѵ�.
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
			// ���� �����ӿ� �浹�� ��Ͽ� Dest�� �ִٸ� �� ���� ���������ӿ� 
			// �浹�� �Ǿ��ٰ� ���� �������Եȴٴ� ���� �ǹ��Ѵ�.
			else if (src->CheckPrevCollision(dest))
			{
				// ���������ӿ� �浹�� ��Ͽ��� ���θ� �����Ѵ�.
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