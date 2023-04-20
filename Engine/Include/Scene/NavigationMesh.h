#pragma once
#include "../Component/TerrainComponent.h"

struct NavAdjInfo
{
	int	index;
	int	edgeIndex;
};

struct NavMeshInfo
{
	// ���� �����ϴ� ��ġ �������� 1���� ������ �Ǳ� ������
	// ������ �� �ִ� Ŭ������ 1���� �����ΰ� ��� �׺���̼��� �����Ѵ�.
	Vector3	pos[3];
	Vector3	min;
	Vector3	max;
	Vector3	edge[3];			//�𼭸�
	Vector3	edgeCenter[3];		//�𼭸� �߾�
	Vector3	center;				//�ﰢ�� ����
	float	angle;				//y��������� ���峷�� y�� ������� y���� ������ ���ϰ� ������ ������ ���� ������ ���
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

