#pragma once
#include "../Component/TerrainComponent.h"

struct NavAdjInfo
{
	int	index;
	int	edgeIndex;
};

struct NavMeshInfo
{
	// 셀을 구성하는 위치 정보들은 1개만 있으면 되기 때문에
	// 공유할 수 있는 클래스에 1개만 만들어두고 모든 네비게이션이 공유한다.
	Vector3	pos[3];
	Vector3	min;
	Vector3	max;
	Vector3	edge[3];			//모서리
	Vector3	edgeCenter[3];		//모서리 중앙
	Vector3	center;				//삼각형 중점
	float	angle;				//y축기준으로 가장낮은 y와 가장높은 y와의 각도를 구하고 각도가 낮으면 막힘 높으면 통과
	int		index;
	bool			enable;
	std::vector<NavAdjInfo>	vecAdj;
	NavMeshInfo()
	{
		enable = true;
		angle = 0.f;
		index = 0;
		min = Vector3(10000000.f, 10000000.f, 10000000.f);
		max = Vector3(-10000000.f, -10000000.f, -10000000.f);
	}
};

struct NavigationSection
{
	Vector3		size;
	Vector3		min;
	Vector3		max;
	std::vector<NavMeshInfo*>	vecCell;
	void Add(NavMeshInfo* cell)
	{
		vecCell.push_back(cell);
	}
};

class CNavigationMesh
{
private:
	friend class CNavigation3DThread;
	friend class CNavigationManager3D;
	friend class CNavigation3D;
	class CNavigationManager3D* m_Mgr;
	class CTerrainComponent* m_Terrain;
	NavigationSection* m_SectionArray;
	Vector3		m_Min;
	Vector3		m_Max;
	Vector3		m_SectionSize;
	Vector2		m_CellSize;
	bool		m_Grid;
	int			m_LineRectCountX;
	int			m_LineRectCountY;
	int			m_SectionX;
	int			m_SectionY;
	std::vector<NavMeshInfo*>	m_vecCell;
	std::vector<NavMeshInfo*>	m_vecCellFind;
	CNavigationMesh();
	CNavigationMesh(const CNavigationMesh& nav);
	~CNavigationMesh();
	static bool SortCamera(NavMeshInfo* src, NavMeshInfo* dest);
	void CreateAdjGrid();
	void CreateAdj();
	bool CheckOnEdge(int src, int dest, const Vector3& origin1, const Vector3& origin2,
		const Vector3& edge, float edgeLength, int edge1, int edge2);
	float ccw(const Vector2& v1, const Vector2& v2);
	float ccw(const Vector2& v1, const Vector2& v2, const Vector2& v3);
	void CreateSection();
	bool RayIntersectTriangle(Vector3& impactPoint, float& dist, const Vector3& pos, const Vector3& dir, 
		const Vector3& cellPos0, const Vector3& cellPos1, const Vector3& cellPos2);
public:
	class CScene* GetScene()	const;
	std::string GetSceneName()	const;
	std::string GetComponentName()	const;
	void CreateNavigation(class CTerrainComponent* terrain);
	float GetHeight(const Vector3& pos);
	NavMeshInfo* FindNavMeshInfo(const Vector3& pos);
	int FindNavMeshInfoIndex(const Vector3& pos);
	NavMeshInfo* GetNavMeshInfo(int index);
	bool GetPickingPos(Vector3& result, const Ray& ray);
};

