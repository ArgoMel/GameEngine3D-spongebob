#pragma once
#include "../Component/TerrainComponent.h"

//	�ڳ�ã�°� ���� jps���� ���̽�Ÿ ���

struct NavigationCell
{
	NavigationCell* parent;
	ENavNodeType	nodeType;
	bool			enable;		//�����ִ��� ������ üũ
	int		index;
	int		parentIndex;
	float			cost;
	float			dist;
	float			total;
	NavigationCell()
	{
		parent = nullptr;
		nodeType = ENavNodeType::None;
		parentIndex = -1;
		index = -1;
		cost = -1.f;
		dist = -1.f;
		total = -1.f;
		enable = true;
	}
	void Clear()
	{
		nodeType = ENavNodeType::None;
		parentIndex = -1;
		cost = -1.f;
		dist = -1.f;
		total = -1.f;
		parent = nullptr;
	}
};

class CNavigation3D
{
private:
	friend class CNavigation3DThread;
	friend class CNavigationManager3D;
	class CNavigationMesh* m_NavMesh;
	bool		m_Grid;				// ������������ �ƴ���
	int			m_LineRectCountX;
	int			m_LineRectCountY;
	std::vector<NavigationCell*>	m_vecCell;
	std::vector<NavigationCell*>	m_vecOpen;
	std::vector<NavigationCell*>	m_vecUseNode;
	CNavigation3D();
	CNavigation3D(const CNavigation3D& nav);
	~CNavigation3D();
	static bool SortNode(NavigationCell* src, NavigationCell* dest);
	bool FindNode(NavigationCell* node, NavigationCell* startNode, NavigationCell* endNode, const Vector3& end,
		std::list<Vector3>& pathList);
public:
	void CreateNavigation(class CNavigationMesh* navMesh);
	bool FindPath(const Vector3& start, const Vector3& end, std::list<Vector3>& pathList);
	CNavigation3D* Clone();
};

