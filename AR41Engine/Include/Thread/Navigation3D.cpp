#include "Navigation3D.h"
#include "../Component/TerrainComponent.h"
#include "../Scene/NavigationMesh.h"

CNavigation3D::CNavigation3D()
    : m_Grid(true)
    , m_LineRectCountX(0)
    , m_LineRectCountY(0)
    , m_NavMesh(nullptr)
{
}

CNavigation3D::CNavigation3D(const CNavigation3D& nav)
	: m_NavMesh(nav.m_NavMesh)
{
    m_vecCell.clear();
    size_t	size = nav.m_vecCell.size();
    for (size_t i = 0; i < size; ++i)
    {
        NavigationCell* cell = new NavigationCell;
        *cell = *nav.m_vecCell[i];
        m_vecCell.push_back(cell);
    }
    m_Grid = nav.m_Grid;
    m_LineRectCountX = nav.m_LineRectCountX;
    m_LineRectCountY = nav.m_LineRectCountY;
}

CNavigation3D::~CNavigation3D()
{
    size_t	size = m_vecCell.size();
    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(m_vecCell[i]);
    }
}

bool CNavigation3D::SortNode(NavigationCell* src, NavigationCell* dest)
{
	return src->total > dest->total;
}

bool CNavigation3D::FindNode(NavigationCell* node, NavigationCell* startNode, NavigationCell* endNode, 
    const Vector3& end, std::list<Vector3>& pathList)
{
	NavMeshInfo* nodeInfo = m_NavMesh->GetNavMeshInfo(node->index);
	size_t	adjSize = nodeInfo->vecAdj.size();
	for (size_t i = 0; i < adjSize; ++i)
	{
		if (nodeInfo->vecAdj[i].index == -1)
		{
			continue;
		}
		int	adjIndex = nodeInfo->vecAdj[i].index;
		NavMeshInfo* adjNode = m_NavMesh->GetNavMeshInfo(adjIndex);
		if (!adjNode->enable)
		{
			continue;
		}
		else if (m_vecCell[adjIndex]->nodeType == ENavNodeType::Close)
		{
			continue;
		}
		// 도착할 셀을 찾았을 경우
		if (endNode->index == adjIndex)
		{
			m_vecUseNode.push_back(m_vecCell[adjIndex]);
			pathList.push_back(end);
			std::stack<int>		pathStack;
			NavigationCell* pathNode = endNode;
			while (pathNode)
			{
				pathStack.push(pathNode->index);
				pathNode = pathNode->parent;
			}
			std::vector<int>	vecPathIndex;
			while (!pathStack.empty())
			{
				int	index = pathStack.top();
				vecPathIndex.push_back(index);
				pathStack.pop();
			}
			size_t	pathIndexCount = vecPathIndex.size();
			for (size_t j = 0; j < pathIndexCount - 1; ++j)
			{
				NavMeshInfo* info = m_NavMesh->GetNavMeshInfo(vecPathIndex[j]);
				int	edgeIndex = -1;
				size_t	adjSize1 = info->vecAdj.size();
				for (size_t k = 0; k < adjSize1; ++k)
				{
					if (info->vecAdj[k].index == vecPathIndex[j + 1])
					{
						edgeIndex = info->vecAdj[k].edgeIndex;
						break;
					}
				}
				if (edgeIndex != -1)
				{
					pathList.push_front(info->edgeCenter[edgeIndex]);
				}
			}
			return true;
		}
		Vector3 line = adjNode->center - nodeInfo->center;
		float	cost = line.x * line.x + line.z * line.z;
		line = adjNode->center - end;
		float	dist = line.x * line.x + line.z * line.z;
		if (cost < 0.f || dist < 0.f)
		{
			//필요하면 예외처리
		}
		cost = sqrtf(cost);
		dist = sqrtf(dist);
		if (m_vecCell[adjIndex]->nodeType == ENavNodeType::None)
		{
			m_vecUseNode.push_back(m_vecCell[adjIndex]);
			m_vecCell[adjIndex]->cost = cost + node->cost;
			m_vecCell[adjIndex]->dist = dist;
			m_vecCell[adjIndex]->total = m_vecCell[adjIndex]->cost + m_vecCell[adjIndex]->dist;
			m_vecCell[adjIndex]->parent = node;
			m_vecCell[adjIndex]->parentIndex = node->index;
			m_vecCell[adjIndex]->nodeType = ENavNodeType::Open;
			m_vecOpen.push_back(m_vecCell[adjIndex]);
		}
		else if (m_vecCell[adjIndex]->cost > cost + node->cost)
		{
			m_vecCell[adjIndex]->cost = cost + node->cost;
			m_vecCell[adjIndex]->dist = dist;
			m_vecCell[adjIndex]->total = m_vecCell[adjIndex]->cost + m_vecCell[adjIndex]->dist;
			m_vecCell[adjIndex]->parent = node;
			m_vecCell[adjIndex]->parentIndex = node->index;
		}
	}
	return false;
}

void CNavigation3D::CreateNavigation(CNavigationMesh* navMesh)
{
    m_NavMesh = navMesh;
    m_Grid = m_NavMesh->m_Grid;
    m_LineRectCountX = m_NavMesh->m_LineRectCountX;
    m_LineRectCountY = m_NavMesh->m_LineRectCountY;
    size_t	size = m_NavMesh->m_vecCell.size();
    for (size_t i = 0; i < size; ++i)
    {
        NavigationCell* cell = new NavigationCell;
        cell->enable = m_NavMesh->m_vecCell[i]->enable;
        cell->index = m_NavMesh->m_vecCell[i]->index;
        m_vecCell.push_back(cell);
    }
}

bool CNavigation3D::FindPath(const Vector3& start, const Vector3& end, std::list<Vector3>& pathList)
{
	pathList.clear();
	NavMeshInfo* startCell = m_NavMesh->FindNavMeshInfo(start);
	if (!startCell)
	{
		return false;
	}
	NavMeshInfo* endCell = m_NavMesh->FindNavMeshInfo(end);
	if (!endCell)
	{
		return false;
	}
	if (!startCell->enable || !endCell->enable)
	{
		return false;
	}
	else if (startCell == endCell)
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
	}
	m_vecUseNode.clear();
	NavigationCell* startNode = m_vecCell[startCell->index];
	NavigationCell* endNode = m_vecCell[endCell->index];
	startNode->nodeType = ENavNodeType::Open;
	startNode->cost = 0.f;
	startNode->dist = startCell->center.Distance(end);
	startNode->total = startNode->dist;
	m_vecOpen.push_back(startNode);
	m_vecUseNode.push_back(startNode);
	while (!m_vecOpen.empty())
	{
		// 열린 목록에서 노드를 가져온다.
		NavigationCell* node = m_vecOpen.back();
		m_vecOpen.pop_back();
		node->nodeType = ENavNodeType::Close;
		// 현재 노드가 검사해야할 방향의 타일을 검사하여 코너를 열린목록에 넣어준다.
		if (FindNode(node, startNode, endNode, end, pathList))
		{
			break;
		}
		// 열린 목록에 있는 노드를 비용이 큰 노드에서 작은 노드 순서로 정렬해준다.
		if (m_vecOpen.size() >= 2)
		{
			std::sort(m_vecOpen.begin(), m_vecOpen.end(), CNavigation3D::SortNode);
		}
	}
	m_vecOpen.clear();
	return !pathList.empty();
}

CNavigation3D* CNavigation3D::Clone()
{
	return new CNavigation3D(*this);
}
