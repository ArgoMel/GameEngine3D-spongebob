#include "Navigation.h"
#include "../Component/TileMapComponent.h"

CNavigation::CNavigation()
	: m_TileMap(nullptr)
	, m_Shape(ETileShape::Rect)
	, m_CountX(0)
	, m_CountY(0)
{
}

CNavigation::~CNavigation()
{
	size_t	size = m_vecNode.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecNode[i]);
	}
}

bool CNavigation::SortNode(NavNode* src, NavNode* dest)
{
	return src->total > dest->total;
}

bool CNavigation::FindNode(NavNode* node, NavNode* endNode, const Vector2& end, std::list<Vector2>& pathList)
{
	auto	iter = node->searchDirList.begin();
	auto	iterEnd = node->searchDirList.end();
	for (; iter != iterEnd; ++iter)
	{
		NavNode* corner = GetCorner(*iter, node, endNode, end);
		if (!corner)
		{
			continue;
		}
		// ã���� ��尡 ���� ����� ��θ� ������ش�.
		if (corner == endNode)
		{
			m_vecUseNode.push_back(corner);
			pathList.push_back(end);
			NavNode* pathNode = node;
			while (pathNode)
			{
				pathList.push_front(pathNode->center);
				pathNode = pathNode->parent;
			}
			pathList.pop_front();
			return true;
		}
		float cost = 0.f;		// �̵� ����� �����ش�.
		switch (*iter)
		{
		case ENodeDir::T:
		case ENodeDir::B:
			cost = node->cost + node->size.y;
			break;
		case ENodeDir::R:
		case ENodeDir::L:
			cost = node->cost + node->size.x;
			break;
		case ENodeDir::RT:
		case ENodeDir::RB:
		case ENodeDir::LB:
		case ENodeDir::LT:
			cost = node->cost + node->center.Distance(corner->center);
			break;
		}
		// ã���� ��尡 ������Ͽ� �� �ִ� ����� ����� ���Ͽ� ���� ������� ��ü�Ѵ�.
		if (corner->nodeType == ENavNodeType::Open)
		{
			if (corner->cost > cost)
			{
				corner->cost = cost;
				corner->total = corner->cost + corner->dist;
				corner->parent = node;
				AddDir(*iter, corner);// ������ ������ �־��ش�.
			}
		}
		else
		{
			corner->nodeType = ENavNodeType::Open;
			corner->parent = node;
			corner->cost = cost;
			corner->dist = corner->center.Distance(end);
			corner->total = corner->cost + corner->dist;
			m_vecOpen.push_back(corner);
			m_vecUseNode.push_back(corner);
			AddDir(*iter, corner);
		}
	}
	return false;
}

NavNode* CNavigation::GetCorner(ENodeDir dir, NavNode* node, NavNode* endNode, const Vector2& end)
{
	switch (m_Shape)
	{
	case ETileShape::Rect:
		switch (dir)
		{
		case ENodeDir::T:
			return GetCornerRectTop(node, endNode);
		case ENodeDir::RT:
			return GetCornerRectRT(node, endNode);
		case ENodeDir::R:
			return GetCornerRectRight(node, endNode);
		case ENodeDir::RB:
			return GetCornerRectRB(node, endNode);
		case ENodeDir::B:
			return GetCornerRectBottom(node, endNode);
		case ENodeDir::LB:
			return GetCornerRectLB(node, endNode);
		case ENodeDir::L:
			return GetCornerRectLeft(node, endNode);
		case ENodeDir::LT:
			return GetCornerRectLT(node, endNode);
		}
		break;
	case ETileShape::Isometric:
		switch (dir)
		{
		case ENodeDir::T:
			return GetCornerIsometricTop(node, endNode);
		case ENodeDir::RT:
			return GetCornerIsometricRT(node, endNode);
		case ENodeDir::R:
			return GetCornerIsometricRight(node, endNode);
		case ENodeDir::RB:
			return GetCornerIsometricRB(node, endNode);
		case ENodeDir::B:
			return GetCornerIsometricBottom(node, endNode);
		case ENodeDir::LB:
			return GetCornerIsometricLB(node, endNode);
		case ENodeDir::L:
			return GetCornerIsometricLeft(node, endNode);
		case ENodeDir::LT:
			return GetCornerIsometricLT(node, endNode);
		}
		break;
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerRectTop(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ���� �ö󰡸� �������� �����ְ� ������ ���� �շ����� ��쳪
	// ������ �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexY = node->indexY;
	while (true)
	{
		++indexY;
		if (indexY >= m_CountY)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + node->indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = node->indexX + 1;
		int	cornerY = indexY;
		if (cornerX < m_CountX && cornerY + 1 < m_CountY)
		{
			// �������� ���̰� ������ ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = node->indexX - 1;
		cornerY = indexY;
		if (cornerX >= 0 && cornerY + 1 < m_CountY)
		{
			// ������ ���̰� ���� ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerRectBottom(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� �Ʒ��� �������� �������� �����ְ� ������ �Ʒ��� �շ����� ��쳪
	// ������ �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexY = node->indexY;
	while (true)
	{
		--indexY;
		if (indexY < 0)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + node->indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = node->indexX + 1;
		int	cornerY = indexY;
		if (cornerX < m_CountX && cornerY - 1 >= 0)
		{
			// �������� ���̰� ������ �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = node->indexX - 1;
		cornerY = indexY;
		if (cornerX >= 0 && cornerY - 1 >= 0)
		{
			// ������ ���̰� ���� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerRectLeft(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� �������� ���� ���� �����ְ� ���� ���� �շ����� ��쳪
	// �Ʒ��� �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	while (true)
	{
		--indexX;
		if (indexX < 0)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[node->indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = node->indexY + 1;
		if (cornerX - 1 >= 0 && cornerY < m_CountY)
		{
			// ������ ���̰� ���� ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[cornerY * m_CountX + (cornerX - 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		cornerY = node->indexY - 1;
		if (cornerX - 1 >= 0 && cornerY >= 0)
		{
			// �Ʒ����� ���̰� ���� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[cornerY * m_CountX + (cornerX - 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerRectRight(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ���������� ���� ���� �����ְ� ������ ���� �շ����� ��쳪
	// �Ʒ��� �����ְ� ������ �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	while (true)
	{
		++indexX;
		if (indexX >= m_CountX)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[node->indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = node->indexY + 1;
		if (cornerX + 1 < m_CountX && cornerY < m_CountY)
		{
			// ������ ���̰� ������ ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		cornerY = node->indexY - 1;
		if (cornerX + 1 < m_CountX && cornerY >= 0)
		{
			// �Ʒ����� ���̰� ������ �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerRectLT(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ���� ���� ���� �������� �����ְ� ������ ���� �շ����� ��쳪
	// �Ʒ��� �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		--indexX;
		++indexY;
		if (indexX < 0 || indexY >= m_CountY)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = indexY - 1;
		if (cornerX - 1 >= 0 && cornerY >= 0)
		{
			// �Ʒ����� ���̰� ���� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[cornerY * m_CountX + (cornerX - 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX + 1;
		cornerY = indexY;
		if (cornerX < m_CountX && cornerY + 1 < m_CountY)
		{
			// �������� ���̰� ������ ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// �밢�� �ڳʰ� �ƴ϶�� ���� ���� ������ �ǹǷ� ����, �׸��� ��������
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ���
		// �Ѵ�.
		NavNode* find = GetCornerRectTop(cornerNode, endNode);
		// �������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (find)
		{
			return cornerNode;
		}
		find = GetCornerRectLeft(cornerNode, endNode);
		if (find)
		{
			return cornerNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerRectRT(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ������ ���� ���� ������ �����ְ� ���� ���� �շ����� ��쳪
	// �Ʒ��� �����ְ� ������ �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		++indexX;
		++indexY;
		if (indexX >= m_CountX || indexY >= m_CountY)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = indexY - 1;
		if (cornerX + 1 < m_CountX && cornerY >= 0)
		{
			// �Ʒ����� ���̰� ������ �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX - 1;
		cornerY = indexY;
		if (cornerX >= 0 && cornerY + 1 < m_CountY)
		{
			// ������ ���̰� ���� ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// �밢�� �ڳʰ� �ƴ϶�� ������ ���� ������ �ǹǷ� ������, �׸��� ��������
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ��� �Ѵ�.
		NavNode* find = GetCornerRectTop(cornerNode, endNode);
		// �������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (find)
		{
			return cornerNode;
		}
		find = GetCornerRectRight(cornerNode, endNode);
		if (find)
		{
			return cornerNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerRectLB(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ���� �Ʒ��� ���� �������� �����ְ� ������ �Ʒ��� �շ����� ��쳪
	// ���� �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		--indexX;
		--indexY;
		if (indexX < 0 || indexY < 0)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = indexY + 1;
		if (cornerX - 1 >= 0 && cornerY < m_CountY)
		{
			// ������ ���̰� ���� ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[cornerY * m_CountX + (cornerX - 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX + 1;
		cornerY = indexY;
		if (cornerX < m_CountX && cornerY - 1 >= 0)
		{
			// �������� ���̰� ������ �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
				return cornerNode;
		}
		// �밢�� �ڳʰ� �ƴ϶�� ���� �Ʒ��� ������ �ǹǷ� ����, �׸��� �Ʒ�������
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ��� �Ѵ�.
		NavNode* find = GetCornerRectBottom(cornerNode, endNode);
		// �Ʒ������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (find)
		{
			return cornerNode;
		}
		find = GetCornerRectLeft(cornerNode, endNode);
		if (find)
		{
			return cornerNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerRectRB(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ������ �Ʒ��� ���� ������ �����ְ� ���� �Ʒ��� �շ����� ��쳪
	// ���� �����ְ� ������ ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		++indexX;
		--indexY;
		if (indexX >= m_CountX || indexY < 0)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = indexY + 1;
		if (cornerX + 1 < m_CountX && cornerY < m_CountY)
		{
			// ������ ���̰� ������ ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[cornerY * m_CountX + (cornerX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX - 1;
		cornerY = indexY;
		if (cornerX >= 0 && cornerY - 1 >= 0)
		{
			// ������ ���̰� ���� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// �밢�� �ڳʰ� �ƴ϶�� ������ �Ʒ��� ������ �ǹǷ� ������, �׸��� �Ʒ�������
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ��� �Ѵ�.
		NavNode* find = GetCornerRectBottom(cornerNode, endNode);
		// �Ʒ������� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (find)
		{
			return cornerNode;
		}
		find = GetCornerRectRight(cornerNode, endNode);
		if (find)
		{
			return cornerNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerIsometricTop(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ���� ���� ���� �Ʒ��� �����ְ� ������ �շ����� ��쳪
	// ������ �Ʒ��� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		indexY += 2;
		if (indexY >= m_CountY)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		if (indexY % 2 == 0)
		{
			cornerX = indexX - 1;
		}
		int	cornerY = indexY - 1;
		if (indexX - 1 >= 0)
		{
			// ���� �Ʒ��� ���̰� ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX - 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		if (indexY % 2 == 1)
		{
			cornerX = indexX + 1;
		}
		if (indexX + 1 < m_CountX)
		{
			// ������ �Ʒ��� ���̰� �������� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// �밢�� �ڳʰ� �ƴ϶�� ���� ������ �ǹǷ� ������ ��, �׸��� ���� ����
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ��� �Ѵ�.
		NavNode* find = GetCornerIsometricRT(cornerNode, endNode);
		// ������ ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (find)
		{
			return cornerNode;
		}
		find = GetCornerIsometricLT(cornerNode, endNode);
		if (find)
		{
			return cornerNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerIsometricBottom(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� �Ʒ��� ���� ���� ���� �����ְ� ������ �շ����� ��쳪
	// ������ ���� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		indexY -= 2;
		if (indexY < 0)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		if (indexY % 2 == 0)
		{
			cornerX = indexX - 1;
		}
		int	cornerY = indexY + 1;
		if (indexX - 1 >= 0)
		{
			// ���� ���� ���̰� ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX - 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		if (indexY % 2 == 1)
		{
			cornerX = indexX + 1;
		}
		if (indexX + 1 < m_CountX)
		{
			// ������ ���� ���̰� �������� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// �밢�� �ڳʰ� �ƴ϶�� �Ʒ��� ������ �ǹǷ� ������ �Ʒ�, �׸��� ���� �Ʒ���
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ��� �Ѵ�.
		NavNode* find = GetCornerIsometricRB(cornerNode, endNode);
		// ������ �Ʒ��� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (find)
		{
			return cornerNode;
		}
		find = GetCornerIsometricLB(cornerNode, endNode);
		if (find)
		{
			return cornerNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerIsometricLeft(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� �������� ���� ������ ���� �����ְ� ���� �շ����� ��쳪
	// ������ �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		--indexX;
		if (indexX < 0)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		if (indexY % 2 == 1)
		{
			cornerX = indexX + 1;
		}
		int	cornerY = indexY + 1;
		if (cornerX < m_CountX && indexY + 2 < m_CountY)
		{
			// ������ ���� ���̰� �� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY + 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		if (indexY % 2 == 1)
		{
			cornerX = indexX + 1;
		}
		cornerY = indexY - 1;
		if (cornerX < m_CountX && indexY - 2 >= 0)
		{
			// ������ �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY - 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// �밢�� �ڳʰ� �ƴ϶�� �������� ������ �ǹǷ� ���� ��, �׸��� ���� �Ʒ���
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ��� �Ѵ�.
		NavNode* find = GetCornerIsometricLT(cornerNode, endNode);
		// ���� ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (find)
		{
			return cornerNode;
		}
		find = GetCornerIsometricLB(cornerNode, endNode);
		if (find)
		{
			return cornerNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerIsometricRight(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ���������� ���� ���� ���� �����ְ� ���� �շ����� ��쳪
	// ���� �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		++indexX;
		if (indexX >= m_CountX)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		if (indexY % 2 == 0)
		{
			cornerX = indexX - 1;
		}
		int	cornerY = indexY + 1;
		if (cornerX >= 0 && indexY + 2 < m_CountY)
		{
			// ���� ���� ���̰� �� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY + 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		if (indexY % 2 == 0)
		{
			cornerX = indexX - 1;
		}
		cornerY = indexY - 1;
		if (cornerX >= 0 && indexY - 2 >= 0)
		{
			// ���� �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY - 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// �밢�� �ڳʰ� �ƴ϶�� ���������� ������ �ǹǷ� ������ ��, �׸��� ������ �Ʒ���
		// �ڳʰ� �ִ����� �Ǵ��Ͽ� �ڳʰ� �ִٸ� ���� ��带 �ڳʵ�� �Ǵ��ϵ��� �Ѵ�.
		NavNode* find = GetCornerIsometricRT(cornerNode, endNode);
		// ������ ���� �˻��ϴ� �� �ڳʰ� �ִٸ� �� ��带 �ڳʷ� �ν��Ѵ�.
		if (find)
		{
			return cornerNode;
		}
		find = GetCornerIsometricRB(cornerNode, endNode);
		if (find)
		{
			return cornerNode;
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerIsometricLT(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ���� ���� �ö󰡸� ������ ���� �����ְ� ���� �շ����� ��쳪
	// ���� �Ʒ��� �����ְ� ������ �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		if (indexY % 2 == 0)
		{
			--indexX;
		}
		++indexY;
		if (indexY >= m_CountY || indexX < 0)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = indexY + 1;
		if (indexY % 2 == 1)
		{
			++cornerX;
		}
		if (cornerX < m_CountX && indexY + 2 < m_CountY)
		{
			// ������ ���� ���̰� ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY + 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		if (indexY % 2 == 0)
		{
			--cornerX;
		}
		cornerY = indexY - 1;
		if (indexX - 1 >= 0 && cornerY >= 0)
		{
			// ���� �Ʒ��� ���̰� ������ �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX - 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerIsometricRT(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ������ ���� �ö󰡸� ���� ���� �����ְ� ���� �շ����� ��쳪
	// ������ �Ʒ��� �����ְ� �������� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		if (indexY % 2 == 1)
		{
			++indexX;
		}
		++indexY;
		if (indexY >= m_CountY || indexX >= m_CountX)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = indexY + 1;
		if (indexY % 2 == 0)
		{
			--cornerX;
		}
		if (cornerX >= 0 && indexY + 2 < m_CountY)
		{
			// ���� ���� ���̰� ���� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY + 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		if (indexY % 2 == 1)
		{
			++cornerX;
		}
		cornerY = indexY - 1;
		if (cornerX < m_CountX && cornerY >= 0)
		{
			// ������ �Ʒ��� ���̰� �������� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerIsometricLB(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ���� �Ʒ��� �������� ���� ���� �����ְ� ������ �շ����� ��쳪
	// ������ �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		if (indexY % 2 == 0)
		{
			--indexX;
		}
		--indexY;
		if (indexY < 0 || indexX < 0)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = indexY + 1;
		if (indexY % 2 == 0)
		{
			--cornerX;
		}
		if (indexX - 1 >= 0 && cornerY < m_CountY)
		{
			// ���� ���� ���̰� ������ �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX - 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		if (indexY % 2 == 1)
		{
			++cornerX;
		}
		cornerY = indexY - 1;
		if (cornerX < m_CountX && indexY - 2 >= 0)
		{
			// ������ �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY - 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
	}
	return nullptr;
}

NavNode* CNavigation::GetCornerIsometricRB(NavNode* node, NavNode* endNode)
{
	// ��ĭ�� ������ �Ʒ��� �������� ������ ���� �����ְ� �������� �շ����� ��쳪
	// ���� �Ʒ��� �����ְ� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
	int	indexX = node->indexX;
	int	indexY = node->indexY;
	while (true)
	{
		if (indexY % 2 == 1)
		{
			++indexX;
		}
		--indexY;
		if (indexY < 0 || indexX >= m_CountX)
		{
			return nullptr;
		}
		NavNode* cornerNode = m_vecNode[indexY * m_CountX + indexX];
		if (cornerNode == endNode)
		{
			return cornerNode;
		}
		else if (cornerNode->nodeType == ENavNodeType::Close)
		{
			return nullptr;
		}
		else if (cornerNode->tileOption == ETileOption::Wall)
		{
			return nullptr;
		}
		int	cornerX = indexX;
		int	cornerY = indexY + 1;
		if (indexY % 2 == 1)
		{
			++cornerX;
		}
		if (indexX + 1 < m_CountX && cornerY < m_CountY)
		{
			// ������ ���� ���̰� �������� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		cornerX = indexX;
		if (indexY % 2 == 0)
		{
			--cornerX;
		}
		cornerY = indexY - 1;
		if (cornerX >= 0 && indexY - 2 >= 0)
		{
			// ���� �Ʒ��� ���̰� �Ʒ��� �Ϲ��� ���
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY - 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
	}
	return nullptr;
}

void CNavigation::AddDir(ENodeDir dir, NavNode* node)
{
	node->searchDirList.clear();
	switch (m_Shape)
	{
	case ETileShape::Rect:
		switch (dir)
		{
		case ENodeDir::T:
			node->searchDirList.push_back(ENodeDir::T);
			node->searchDirList.push_back(ENodeDir::LT);
			node->searchDirList.push_back(ENodeDir::RT);
			break;
		case ENodeDir::RT:
			node->searchDirList.push_back(ENodeDir::RT);
			node->searchDirList.push_back(ENodeDir::T);
			node->searchDirList.push_back(ENodeDir::R);
			node->searchDirList.push_back(ENodeDir::LT);
			node->searchDirList.push_back(ENodeDir::RB);
			break;
		case ENodeDir::R:
			node->searchDirList.push_back(ENodeDir::R);
			node->searchDirList.push_back(ENodeDir::RT);
			node->searchDirList.push_back(ENodeDir::RB);
			break;
		case ENodeDir::RB:
			node->searchDirList.push_back(ENodeDir::RB);
			node->searchDirList.push_back(ENodeDir::R);
			node->searchDirList.push_back(ENodeDir::B);
			node->searchDirList.push_back(ENodeDir::RT);
			node->searchDirList.push_back(ENodeDir::LB);
			break;
		case ENodeDir::B:
			node->searchDirList.push_back(ENodeDir::B);
			node->searchDirList.push_back(ENodeDir::LB);
			node->searchDirList.push_back(ENodeDir::RB);
			break;
		case ENodeDir::LB:
			node->searchDirList.push_back(ENodeDir::LB);
			node->searchDirList.push_back(ENodeDir::L);
			node->searchDirList.push_back(ENodeDir::B);
			node->searchDirList.push_back(ENodeDir::LT);
			node->searchDirList.push_back(ENodeDir::RB);
			break;
		case ENodeDir::L:
			node->searchDirList.push_back(ENodeDir::L);
			node->searchDirList.push_back(ENodeDir::LT);
			node->searchDirList.push_back(ENodeDir::LB);
			break;
		case ENodeDir::LT:
			node->searchDirList.push_back(ENodeDir::LT);
			node->searchDirList.push_back(ENodeDir::T);
			node->searchDirList.push_back(ENodeDir::L);
			node->searchDirList.push_back(ENodeDir::RT);
			node->searchDirList.push_back(ENodeDir::LB);
			break;
		}
		break;
	case ETileShape::Isometric:
		switch (dir)
		{
		case ENodeDir::T:
			node->searchDirList.push_back(ENodeDir::T);
			node->searchDirList.push_back(ENodeDir::RT);
			node->searchDirList.push_back(ENodeDir::LT);
			node->searchDirList.push_back(ENodeDir::R);
			node->searchDirList.push_back(ENodeDir::L);
			break;
		case ENodeDir::RT:
			node->searchDirList.push_back(ENodeDir::RT);
			node->searchDirList.push_back(ENodeDir::T);
			node->searchDirList.push_back(ENodeDir::R);
			break;
		case ENodeDir::R:
			node->searchDirList.push_back(ENodeDir::R);
			node->searchDirList.push_back(ENodeDir::RT);
			node->searchDirList.push_back(ENodeDir::RB);
			node->searchDirList.push_back(ENodeDir::T);
			node->searchDirList.push_back(ENodeDir::B);
			break;
		case ENodeDir::RB:
			node->searchDirList.push_back(ENodeDir::RB);
			node->searchDirList.push_back(ENodeDir::R);
			node->searchDirList.push_back(ENodeDir::B);
			break;
		case ENodeDir::B:
			node->searchDirList.push_back(ENodeDir::B);
			node->searchDirList.push_back(ENodeDir::RB);
			node->searchDirList.push_back(ENodeDir::LB);
			node->searchDirList.push_back(ENodeDir::R);
			node->searchDirList.push_back(ENodeDir::L);
			break;
		case ENodeDir::LB:
			node->searchDirList.push_back(ENodeDir::LB);
			node->searchDirList.push_back(ENodeDir::L);
			node->searchDirList.push_back(ENodeDir::B);
			break;
		case ENodeDir::L:
			node->searchDirList.push_back(ENodeDir::L);
			node->searchDirList.push_back(ENodeDir::LT);
			node->searchDirList.push_back(ENodeDir::LB);
			node->searchDirList.push_back(ENodeDir::T);
			node->searchDirList.push_back(ENodeDir::B);
			break;
		case ENodeDir::LT:
			node->searchDirList.push_back(ENodeDir::LT);
			node->searchDirList.push_back(ENodeDir::L);
			node->searchDirList.push_back(ENodeDir::T);
			break;
		}
		break;
	}
}

void CNavigation::CreateNavigation(CTileMapComponent* tileMap)
{
	m_TileMap = tileMap;
	m_Shape = m_TileMap->GetShape();
	m_CountX = m_TileMap->GetCountX();
	m_CountY = m_TileMap->GetCountY();
	m_TileSize = m_TileMap->GetTileSize();
	int	count = m_CountX * m_CountY;
	for (int i = 0; i < count; ++i)
	{
		NavNode* node = new NavNode;
		node->tileOption = m_TileMap->GetTile(i)->GetTileOption();
		node->pos = m_TileMap->GetTile(i)->GetPos();
		node->size = m_TileMap->GetTileSize();
		node->center = node->pos + node->size * 0.5f;
		node->indexX = m_TileMap->GetTile(i)->GetIndexX();
		node->indexY = m_TileMap->GetTile(i)->GetIndexY();
		node->index = m_TileMap->GetTile(i)->GetIndex();
		m_vecNode.push_back(node);
	}
}

bool CNavigation::FindPath(const Vector2& start, const Vector2& end, std::list<Vector2>& pathList)
{
	pathList.clear();
	int	startIndex = m_TileMap->GetTileIndex(start);
	if (startIndex == -1)
	{
		return false;
	}
	int endIndex = m_TileMap->GetTileIndex(end);
	if (endIndex == -1)
	{
		return false;
	}
	NavNode* startNode = m_vecNode[startIndex];
	NavNode* endNode = m_vecNode[endIndex];
	if (endNode->tileOption == ETileOption::Wall)
	{
		return false;
	}
	else if (endNode == startNode)
	{
		pathList.push_back(end);
		return true;
	}
	size_t size = m_vecUseNode.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecUseNode[i]->nodeType = ENavNodeType::None;
		m_vecUseNode[i]->cost = FLT_MAX;
		m_vecUseNode[i]->dist = FLT_MAX;
		m_vecUseNode[i]->total = FLT_MAX;
		m_vecUseNode[i]->parent = nullptr;
		m_vecUseNode[i]->searchDirList.clear();
	}
	m_vecUseNode.clear();
	startNode->nodeType = ENavNodeType::Open;
	startNode->cost = 0.f;
	startNode->dist = startNode->center.Distance(end);
	startNode->total = startNode->dist;
	for (int i = 0; i < (int)ENodeDir::End; ++i)
	{
		startNode->searchDirList.push_back((ENodeDir)i);
	}
	m_vecOpen.push_back(startNode);
	m_vecUseNode.push_back(startNode);
	while (!m_vecOpen.empty())
	{
		NavNode* node = m_vecOpen.back();		// ���� ��Ͽ��� ��带 �����´�.
		m_vecOpen.pop_back();
		node->nodeType = ENavNodeType::Close;
		// ���� ��尡 �˻��ؾ��� ������ Ÿ���� �˻��Ͽ� �ڳʸ� ������Ͽ� �־��ش�.
		if (FindNode(node, endNode, end, pathList))
		{
			break;
		}
		// ���� ��Ͽ� �ִ� ��带 ����� ū ��忡�� ���� ��� ������ �������ش�.
		if (m_vecOpen.size() >= 2)
		{
			std::sort(m_vecOpen.begin(), m_vecOpen.end(), CNavigation::SortNode);
		}
	}
	m_vecOpen.clear();
	return !pathList.empty();
}
