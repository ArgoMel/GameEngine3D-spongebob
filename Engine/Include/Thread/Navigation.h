#pragma once
#include "../Component/TileMapComponent.h"

enum class ENodeDir
{
	T,
	RT,
	R,
	RB,
	B,
	LB,
	L,
	LT,
	End
};

struct NavNode
{
	NavNode* parent;
	ENavNodeType	nodeType;
	ETileOption		tileOption;
	Vector2			pos;
	Vector2			size;
	Vector2			center;
	int				indexX;
	int				indexY;
	int				index;
	float			cost;	//비용 작은거 우선해서 넣는다
	float			dist;	//최소 거리
	float			total;	//전체비용
	std::list<ENodeDir>	searchDirList;
	NavNode() 
		: parent(nullptr)
		, nodeType(ENavNodeType::None)
		, tileOption(ETileOption::None)
		, indexX(-1)
		, indexY(-1)
		, index(-1)
		, cost(FLT_MAX)
		, dist(FLT_MAX)
		, total(FLT_MAX)
	{
	}
};

class CNavigation
{
private:
	friend class CNavigation2DThread;
	class CTileMapComponent* m_TileMap;
	ETileShape	m_Shape;
	Vector2	m_TileSize;
	int		m_CountX;
	int		m_CountY;
	std::vector<NavNode*>			m_vecNode;
	std::vector<NavNode*>			m_vecOpen;	//열린목록 노드
	std::vector<NavNode*>			m_vecUseNode; //사용한 노드들
	CNavigation();
	~CNavigation();
	static bool SortNode(NavNode* src, NavNode* dest);
	bool FindNode(NavNode* node, NavNode* endNode, const Vector2& end, std::list<Vector2>& pathList);
	NavNode* GetCorner(ENodeDir dir, NavNode* node, NavNode* endNode, const Vector2& end);

	NavNode* GetCornerRectTop(NavNode* node, NavNode* endNode);
	NavNode* GetCornerRectBottom(NavNode* node, NavNode* endNode);
	NavNode* GetCornerRectLeft(NavNode* node, NavNode* endNode);
	NavNode* GetCornerRectRight(NavNode* node, NavNode* endNode);
	NavNode* GetCornerRectLT(NavNode* node, NavNode* endNode);
	NavNode* GetCornerRectRT(NavNode* node, NavNode* endNode);
	NavNode* GetCornerRectLB(NavNode* node, NavNode* endNode);
	NavNode* GetCornerRectRB(NavNode* node, NavNode* endNode);

	NavNode* GetCornerIsometricTop(NavNode* node, NavNode* endNode);
	NavNode* GetCornerIsometricBottom(NavNode* node, NavNode* endNode);
	NavNode* GetCornerIsometricLeft(NavNode* node, NavNode* endNode);
	NavNode* GetCornerIsometricRight(NavNode* node, NavNode* endNode);
	NavNode* GetCornerIsometricLT(NavNode* node, NavNode* endNode);
	NavNode* GetCornerIsometricRT(NavNode* node, NavNode* endNode);
	NavNode* GetCornerIsometricLB(NavNode* node, NavNode* endNode);
	NavNode* GetCornerIsometricRB(NavNode* node, NavNode* endNode);
	void AddDir(ENodeDir dir, NavNode* node);
public:
	void CreateNavigation(class CTileMapComponent* tileMap);
	bool FindPath(const Vector2& start, const Vector2& end, std::list<Vector2>& pathList);
};

