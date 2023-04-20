#include "CollisionSection3D.h"
#include "../Component/Collider3D.h"

CCollisionSection3D::CCollisionSection3D()
	: m_IndexX(0)
	, m_IndexY(0)
	, m_IndexZ(0)
	, m_Index(0)
	, m_Radius(0.f)
{
}

CCollisionSection3D::~CCollisionSection3D()
{
}

void CCollisionSection3D::AddCollider(CCollider3D* collider)
{
	m_ColliderList.push_back(collider);
	collider->AddSectionIndex(m_Index);
}

void CCollisionSection3D::Collision(float deltaTime)
{
	if (m_ColliderList.size() < 2)
	{
		return;
	}
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	iterEnd--;
	auto iter1End = m_ColliderList.end();
	for (; iter != iterEnd; iter++)
	{
		CCollider* src = *iter;
		auto iter1 = iter;
		iter1++;
		for (; iter1 != iter1End; iter1++)
		{
			CCollider* dest = *iter1;
			int	overlapSection = src->CheckOverlapSection(dest);
			if (overlapSection != -1 && overlapSection < m_Index)
			{
				continue;
			}
			CollisionProfile* srcProfile = src->GetCollisionProfile();
			CollisionProfile* destProfile = dest->GetCollisionProfile();
			if (srcProfile->vecCollisionInteraction[(int)destProfile->channel->channel] == ECollision_Interaction::Ignore ||
				destProfile->vecCollisionInteraction[(int)srcProfile->channel->channel] == ECollision_Interaction::Ignore)
			{
				continue;
			}
			if (src->Collision(dest))
			{
				// ���������ӿ� �浹������ ���ٸ� ������ش�.
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
			// ���� �����ӿ� �浹�� ��Ͽ� Dest�� �ִٸ� �� ���� ���������ӿ� �浹�� �Ǿ��ٰ� ���� �������Եȴٴ�
			// ���� �ǹ��Ѵ�.
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
