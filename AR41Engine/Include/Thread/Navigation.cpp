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
		// 찾아준 노드가 도착 노드라면 경로를 만들어준다.
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
		float cost = 0.f;		// 이동 비용을 구해준다.
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
		// 찾아준 노드가 열린목록에 들어가 있는 노드라면 비용을 비교하여 작은 비용으로 교체한다.
		if (corner->nodeType == ENavNodeType::Open)
		{
			if (corner->cost > cost)
			{
				corner->cost = cost;
				corner->total = corner->cost + corner->dist;
				corner->parent = node;
				AddDir(*iter, corner);// 조사할 방향을 넣어준다.
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
	// 한칸씩 위로 올라가며 오른쪽이 막혀있고 오른쪽 위가 뚫려있을 경우나
	// 왼쪽이 막혀있고 왼쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 오른쪽이 벽이고 오른쪽 위가 일반일 경우
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
			// 왼쪽이 벽이고 왼쪽 위가 일반일 경우
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
	// 한칸씩 아래로 내려가며 오른쪽이 막혀있고 오른쪽 아래가 뚫려있을 경우나
	// 왼쪽이 막혀있고 왼쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 오른쪽이 벽이고 오른쪽 아래가 일반일 경우
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
			// 왼쪽이 벽이고 왼쪽 아래가 일반일 경우
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
	// 한칸씩 왼쪽으로 가며 위가 막혀있고 왼쪽 위가 뚫려있을 경우나
	// 아래가 막혀있고 왼쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 위쪽이 벽이고 왼쪽 위가 일반일 경우
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
			// 아래쪽이 벽이고 왼쪽 아래가 일반일 경우
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
	// 한칸씩 오른쪽으로 가며 위가 막혀있고 오른쪽 위가 뚫려있을 경우나
	// 아래가 막혀있고 오른쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 위쪽이 벽이고 오른쪽 위가 일반일 경우
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
			// 아래쪽이 벽이고 오른쪽 아래가 일반일 경우
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
	// 한칸씩 왼쪽 위로 가며 오른쪽이 막혀있고 오른쪽 위가 뚫려있을 경우나
	// 아래가 막혀있고 왼쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 아래쪽이 벽이고 왼쪽 아래가 일반일 경우
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
			// 오른쪽이 벽이고 오른쪽 위가 일반일 경우
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// 대각선 코너가 아니라면 왼쪽 위로 진행이 되므로 왼쪽, 그리고 위쪽으로
		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록
		// 한다.
		NavNode* find = GetCornerRectTop(cornerNode, endNode);
		// 위쪽으로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
	// 한칸씩 오른쪽 위로 가며 왼쪽이 막혀있고 왼쪽 위가 뚫려있을 경우나
	// 아래가 막혀있고 오른쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 아래쪽이 벽이고 오른쪽 아래가 일반일 경우
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
			// 왼쪽이 벽이고 왼쪽 위가 일반일 경우
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY + 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// 대각선 코너가 아니라면 오른쪽 위로 진행이 되므로 오른쪽, 그리고 위쪽으로
		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록 한다.
		NavNode* find = GetCornerRectTop(cornerNode, endNode);
		// 위쪽으로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
	// 한칸씩 왼쪽 아래로 가며 오른쪽이 막혀있고 오른쪽 아래가 뚫려있을 경우나
	// 위가 막혀있고 왼쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 위쪽이 벽이고 왼쪽 위가 일반일 경우
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
			// 오른쪽이 벽이고 오른쪽 아래가 일반일 경우
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
				return cornerNode;
		}
		// 대각선 코너가 아니라면 왼쪽 아래로 진행이 되므로 왼쪽, 그리고 아래쪽으로
		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록 한다.
		NavNode* find = GetCornerRectBottom(cornerNode, endNode);
		// 아래쪽으로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
	// 한칸씩 오른쪽 아래로 가며 왼쪽이 막혀있고 왼쪽 아래가 뚫려있을 경우나
	// 위가 막혀있고 오른쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 위쪽이 벽이고 오른쪽 위가 일반일 경우
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
			// 왼쪽이 벽이고 왼쪽 아래가 일반일 경우
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(cornerY - 1) * m_CountX + cornerX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// 대각선 코너가 아니라면 오른쪽 아래로 진행이 되므로 오른쪽, 그리고 아래쪽으로
		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록 한다.
		NavNode* find = GetCornerRectBottom(cornerNode, endNode);
		// 아래쪽으로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
	// 한칸씩 위로 가며 왼쪽 아래가 막혀있고 왼쪽이 뚫려있을 경우나
	// 오른쪽 아래가 막혀있고 오른쪽이 뚫려있으면 해당 노드는 코너가 된다.
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
			// 왼쪽 아래가 벽이고 왼쪽 일반일 경우
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
			// 오른쪽 아래가 벽이고 오른쪽이 일반일 경우
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// 대각선 코너가 아니라면 위로 진행이 되므로 오른쪽 위, 그리고 왼쪽 위로
		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록 한다.
		NavNode* find = GetCornerIsometricRT(cornerNode, endNode);
		// 오른쪽 위로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
	// 한칸씩 아래로 가며 왼쪽 위가 막혀있고 왼쪽이 뚫려있을 경우나
	// 오른쪽 위가 막혀있고 오른쪽이 뚫려있으면 해당 노드는 코너가 된다.
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
			// 왼쪽 위가 벽이고 왼쪽 일반일 경우
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
			// 오른쪽 위가 벽이고 오른쪽이 일반일 경우
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[indexY * m_CountX + (indexX + 1)]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// 대각선 코너가 아니라면 아래로 진행이 되므로 오른쪽 아래, 그리고 왼쪽 아래로
		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록 한다.
		NavNode* find = GetCornerIsometricRB(cornerNode, endNode);
		// 오른쪽 아래로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
	// 한칸씩 왼쪽으로 가며 오른쪽 위가 막혀있고 위가 뚫려있을 경우나
	// 오른쪽 아래가 막혀있고 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 오른쪽 위가 벽이고 위 일반일 경우
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
			// 오른쪽 아래가 벽이고 아래가 일반일 경우
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY - 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// 대각선 코너가 아니라면 왼쪽으로 진행이 되므로 왼쪽 위, 그리고 왼쪽 아래로
		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록 한다.
		NavNode* find = GetCornerIsometricLT(cornerNode, endNode);
		// 왼쪽 위로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
	// 한칸씩 오른쪽으로 가며 왼쪽 위가 막혀있고 위가 뚫려있을 경우나
	// 왼쪽 아래가 막혀있고 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 왼쪽 위가 벽이고 위 일반일 경우
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
			// 왼쪽 아래가 벽이고 아래가 일반일 경우
			if (m_vecNode[cornerY * m_CountX + cornerX]->tileOption == ETileOption::Wall &&
				m_vecNode[(indexY - 2) * m_CountX + indexX]->tileOption == ETileOption::None)
			{
				return cornerNode;
			}
		}
		// 대각선 코너가 아니라면 오른쪽으로 진행이 되므로 오른쪽 위, 그리고 오른쪽 아래로
		// 코너가 있는지를 판단하여 코너가 있다면 현재 노드를 코너드로 판단하도록 한다.
		NavNode* find = GetCornerIsometricRT(cornerNode, endNode);
		// 오른쪽 위로 검사하던 중 코너가 있다면 이 노드를 코너로 인식한다.
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
	// 한칸씩 왼쪽 위로 올라가며 오른쪽 위가 막혀있고 위가 뚫려있을 경우나
	// 왼쪽 아래가 막혀있고 왼쪽이 뚫려있으면 해당 노드는 코너가 된다.
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
			// 오른쪽 위가 벽이고 위가 일반일 경우
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
			// 왼쪽 아래가 벽이고 왼쪽이 일반일 경우
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
	// 한칸씩 오른쪽 위로 올라가며 왼쪽 위가 막혀있고 위가 뚫려있을 경우나
	// 오른쪽 아래가 막혀있고 오른쪽이 뚫려있으면 해당 노드는 코너가 된다.
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
			// 왼쪽 위가 벽이고 위가 일반일 경우
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
			// 오른쪽 아래가 벽이고 오른쪽이 일반일 경우
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
	// 한칸씩 왼쪽 아래로 내려가며 왼쪽 위가 막혀있고 왼쪽이 뚫려있을 경우나
	// 오른쪽 아래가 막혀있고 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 왼쪽 위가 벽이고 왼쪽이 일반일 경우
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
			// 오른쪽 아래가 벽이고 아래가 일반일 경우
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
	// 한칸씩 오른쪽 아래로 내려가며 오른쪽 위가 막혀있고 오른쪽이 뚫려있을 경우나
	// 왼쪽 아래가 막혀있고 아래가 뚫려있으면 해당 노드는 코너가 된다.
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
			// 오른쪽 위가 벽이고 오른쪽이 일반일 경우
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
			// 왼쪽 아래가 벽이고 아래가 일반일 경우
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
		NavNode* node = m_vecOpen.back();		// 열린 목록에서 노드를 가져온다.
		m_vecOpen.pop_back();
		node->nodeType = ENavNodeType::Close;
		// 현재 노드가 검사해야할 방향의 타일을 검사하여 코너를 열린목록에 넣어준다.
		if (FindNode(node, endNode, end, pathList))
		{
			break;
		}
		// 열린 목록에 있는 노드를 비용이 큰 노드에서 작은 노드 순서로 정렬해준다.
		if (m_vecOpen.size() >= 2)
		{
			std::sort(m_vecOpen.begin(), m_vecOpen.end(), CNavigation::SortNode);
		}
	}
	m_vecOpen.clear();
	return !pathList.empty();
}
