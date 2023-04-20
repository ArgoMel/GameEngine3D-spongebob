#include "NavigationMesh.h"
#include "Scene.h"
#include "SceneManager.h"
#include "NavigationManager3D.h"
#include "../Thread/ThreadManager.h"
#include "../Component/CameraComponent.h"

CNavigationMesh::CNavigationMesh()
	: m_SectionArray(nullptr)
	, m_Terrain(nullptr)
	, m_SectionX(0)
	, m_SectionY(0)
	, m_Min(10000000.f, 10000000.f, 10000000.f)
	, m_Max(-10000000.f, -10000000.f, -10000000.f)
	, m_Grid(false)
	, m_LineRectCountX(0)
	, m_LineRectCountY(0)
	, m_Mgr(nullptr)
{
}

CNavigationMesh::CNavigationMesh(const CNavigationMesh& nav)
{
	m_vecCell.clear();
	size_t	size = nav.m_vecCell.size();
	for (size_t i = 0; i < size; ++i)
	{
		NavMeshInfo* cell = new NavMeshInfo;
		*cell = *nav.m_vecCell[i];
		m_vecCell.push_back(cell);
	}
	m_CellSize = nav.m_CellSize;
	m_Terrain = nav.m_Terrain;
	m_Min = nav.m_Min;
	m_Max = nav.m_Max;
	m_Grid = nav.m_Grid;
	m_LineRectCountX = nav.m_LineRectCountX;
	m_LineRectCountY = nav.m_LineRectCountY;
	m_SectionX = nav.m_SectionX;
	m_SectionY = nav.m_SectionY;
	m_SectionSize = nav.m_SectionSize;
	if (nav.m_SectionArray)
	{
		m_SectionArray = new NavigationSection[m_SectionX * m_SectionY];
		for (int i = 0; i < m_SectionX * m_SectionY; ++i)
		{
			/*m_SectionArray[i] = nav.m_SectionArray[i];
			m_SectionArray[i].vecCell = m_vecCell;*/
			*(m_SectionArray + i) = nav.m_SectionArray[i];
			(*(m_SectionArray + i)).vecCell = m_vecCell;
		}
	}
}

CNavigationMesh::~CNavigationMesh()
{
	size_t	size = m_vecCell.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecCell[i]);
	}
	SAFE_DELETE_ARRAY(m_SectionArray);
	CThreadManager::GetInst()->DeleteNavigationThread(this);
}

bool CNavigationMesh::SortCamera(NavMeshInfo* src, NavMeshInfo* dest)
{
	Vector3	camPos = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetWorldPos();
	float	srcDist = camPos.Distance(src->center);
	float	destDist = camPos.Distance(dest->center);
	return srcDist < destDist;
}

void CNavigationMesh::CreateAdjGrid()
{
	size_t	size = m_vecCell.size();
	for (size_t i = 0; i < size; i += 2)
	{
		NavAdjInfo	info = {};
		// 우상단 삼각형 인접정보 구성.
		// 위쪽 사각형의 좌 하단 삼각형, 오른쪽 사각형의 좌 하단 삼각형,
		// 자기 자신 사각형의 좌 하단 삼각형이다.

		// 위쪽 좌 하단 삼각형 인덱스
		int	index = (int)i - m_LineRectCountX * 2 + 1;
		if (index >= 0 && index < (int)size)
		{
			info.index = index;
			info.edgeIndex = 0;
			m_vecCell[i]->vecAdj.push_back(info);
		}

		// 자기자신 사각형의 왼쪽 하단 삼각형이다.
		index = (int)i + 1;
		info.index = index;
		info.edgeIndex = 2;
		m_vecCell[i]->vecAdj.push_back(info);

		// 오른쪽 사각형의 왼쪽 하단 삼각형이다.
		index = (int)i + 3;

		// 오른쪽 끝 사각형인지 판단한다.
		if (i / 2 % m_LineRectCountX != m_LineRectCountX - 1)
		{
			info.index = index;
			info.edgeIndex = 1;
			m_vecCell[i]->vecAdj.push_back(info);
		}

		// 좌하단 삼각형 인접정보 구성
		// 왼쪽 사각형의 우상단, 자기자신 사각형의 우상단,
		// 아래쪽 사각형의 우상단 삼각형이다.

		// 자기자신 사각형의 우상단
		index = (int)i;
		info.index = index;
		info.edgeIndex = 0;
		m_vecCell[i + 1]->vecAdj.push_back(info);

		// 왼쪽 사각형의 우상단
		if ((int)i / 2 % m_LineRectCountX != 0)
		{
			// i는 우상단 삼각형의 인덱스이므로 2를 빼준다.
			index = (int)i - 2;
			info.index = index;
			info.edgeIndex = 2;
			m_vecCell[i + 1]->vecAdj.push_back(info);
		}

		// 아래쪽 사각형의 우상단.
		index = (int)i + m_LineRectCountX * 2;
		if (index < (int)size)
		{
			info.index = index;
			info.edgeIndex = 1;
			m_vecCell[i + 1]->vecAdj.push_back(info);
		}
	}
}

void CNavigationMesh::CreateAdj()
{
	size_t	size = m_vecCell.size();
	FILE* file = NULL;
	fopen_s(&file, "NaviAdj.txt", "wt");
	if (!file)
	{
		return;
	}
	Vector3	origin[2][3];
	Vector3	edgeDir[2][3];
	float	length[2][3];
	for (size_t i = 0; i < size - 1; ++i)
	{
		Vector3	src[3];
		Vector3	srcOrigin[3];
		float	srcLength[3] = {};
		for (int k = 0; k < 3; ++k)
		{
			edgeDir[0][k] = m_vecCell[i]->edge[k];
			length[0][k] = edgeDir[0][k].Length();
			edgeDir[0][k].Normalize();
		}
		origin[0][0] = m_vecCell[i]->pos[0];
		origin[0][1] = m_vecCell[i]->pos[1];
		origin[0][2] = m_vecCell[i]->pos[2];
		for (size_t j = i + 1; j < size; ++j)
		{
			// 삼각형의 3개의 엣지들을 서로 내적하여 평행한지 비교한다.
			Vector3	dest[3];
			Vector3	destOrigin[3];
			int	posIdx[3][2] = { 1, 0, 2, 1, 2, 0 };
			bool	insert = false;
			origin[1][0] = m_vecCell[j]->pos[0];
			origin[1][1] = m_vecCell[j]->pos[1];
			origin[1][2] = m_vecCell[j]->pos[2];
			for (int k = 0; k < 3; ++k)
			{
				edgeDir[1][k] = m_vecCell[j]->edge[k];
				length[1][k] = edgeDir[1][k].Length();
				edgeDir[1][k].Normalize();
			}
			for (int k = 0; k < 3; ++k)
			{
				if (insert)
				{
					break;
				}
				for (int l = 0; l < 3; ++l)
				{
					// A삼각형과 B삼각형의 엣지를 구성하는 두 정점이 같을 경우
					// 이 삼각형은 무조건 붙어있는 삼각형이다.
					if ((origin[0][posIdx[k][0]] == origin[1][posIdx[l][0]] || 
						origin[0][posIdx[k][0]] == origin[1][posIdx[l][1]]) &&
						(origin[0][posIdx[k][1]] == origin[1][posIdx[l][0]] ||
							origin[0][posIdx[k][1]] == origin[1][posIdx[l][1]]))
					{
						NavAdjInfo	info = {};
						info.index = (int)j;
						info.edgeIndex = (int)k;
						m_vecCell[i]->vecAdj.push_back(info);

						info.index = (int)i;
						info.edgeIndex = (int)l;
						m_vecCell[j]->vecAdj.push_back(info);
						insert = true;
						break;
					}
					// 두 엣지에 대해서 상대 엣지를 구성하는 2개의 점이 대상 엣지 위에 둘다 존재하는지를 판단한다. 
					// 둘다 존재한다면 무조건 붙어있는 것이다.
					// (B1 - A1, B2 - A1), (B1 - A2, B2 - A2)
					// 내적을 통해서 빼준 벡터의 크기가 그대로 나온다면 직선상에 존재하는 것이다. 
					// 단, 둘다 크기가 나올 경우는 무조건 존재하는 것이고 둘중 하나만 나온다면 크기가 0이라면
					// 하나의 점만 직선위에 존재하고 두 점 위치가 같다는 것이므로 해당 삼각형은 인접 삼각형이 아니다.
					// B1 - A1 처리
					if (CheckOnEdge((int)i, (int)j, origin[1][posIdx[l][0]],
						origin[0][posIdx[k][0]], edgeDir[0][k], length[0][k], k, l))
					{
						insert = true;
						break;
					}
					// B2 - A1 처리
					else if (CheckOnEdge((int)i, (int)j, origin[1][posIdx[l][1]],
						origin[0][posIdx[k][0]], edgeDir[0][k], length[0][k], k, l))
					{
						insert = true;
						break;
					}
					// B1 - A2 처리
					else if (CheckOnEdge((int)i, (int)j, origin[1][posIdx[l][0]],
						origin[0][posIdx[k][1]], edgeDir[0][k] * -1.f, length[0][k], k, l))
					{
						insert = true;
						break;
					}
					// B2 - A2 처리
					else if (CheckOnEdge((int)i, (int)j, origin[1][posIdx[l][1]],
						origin[0][posIdx[k][1]], edgeDir[0][k] * -1.f, length[0][k], k, l))
					{
						insert = true;
						break;
					}
				}
			}
		}
	}
	for (size_t i = 0; i < size; ++i)
	{
		char	text[256] = {};
		sprintf_s(text, "%d : ", (int)i);
		for (size_t j = 0; j < m_vecCell[i]->vecAdj.size(); ++j)
		{
			char	addText[256] = {};
			sprintf_s(addText, "%d ", m_vecCell[i]->vecAdj[j].index);
			strcat_s(text, addText);
		}
		strcat_s(text, "\n");
		fputs(text, file);
	}
	fclose(file);
}

bool CNavigationMesh::CheckOnEdge(int src, int dest, const Vector3& origin1, const Vector3& origin2, 
	const Vector3& edge, float edgeLength, int edge1, int edge2)
{
	//너무작은 삼각형은 예외처리
	if ((int)(origin2.x * 100) == (int)(origin1.x * 100) && (int)(origin2.y * 100) == (int)(origin1.y * 100) &&
		(int)(origin2.z * 100) == (int)(origin1.z * 100))
	{
		return false;
	}
	Vector3	result = origin1 - origin2;
	float	fDist = result.Length();
	// 위에서 구해준 거리를 이용해서 원점으로부터 엣지 방향으로 지정된 거리만큼 이동된 위치에 대상 점이 있는지를 판단한다.
	result = origin2 + edge * fDist;
	if ((int)(result.x * 100) == (int)(origin1.x * 100) && (int)(result.y * 100) == (int)(origin1.y * 100) &&
		(int)(result.z * 100) == (int)(origin1.z * 100))
	{
		result -= origin2;
		if (result.Length() < edgeLength)
		{
			NavAdjInfo	info = {};
			info.index = dest;
			info.edgeIndex = edge1;
			m_vecCell[src]->vecAdj.push_back(info);

			info.index = src;
			info.edgeIndex = edge2;
			m_vecCell[dest]->vecAdj.push_back(info);
			return true;
		}
	}
	return false;
}

float CNavigationMesh::ccw(const Vector2& v1, const Vector2& v2)
{
	return 0.0f;
}

float CNavigationMesh::ccw(const Vector2& v1, const Vector2& v2, const Vector2& v3)
{
	return 0.0f;
}

void CNavigationMesh::CreateSection()
{
	m_SectionX = 5;
	m_SectionY = 5;
	Vector3	length = m_Max - m_Min;
	m_SectionSize = length / Vector3((float)m_SectionX, 1.f, (float)m_SectionY);
	SAFE_DELETE_ARRAY(m_SectionArray);
	m_SectionArray = new NavigationSection[m_SectionX * m_SectionY];
	for (int i = 0; i < m_SectionY; ++i)
	{
		for (int j = 0; j < m_SectionX; ++j)
		{
			int	index = i * m_SectionX + j;
			m_SectionArray[index].size = m_SectionSize;
			m_SectionArray[index].min = m_Min + m_SectionSize * Vector3((float)j, 0.f, (float)i);
			m_SectionArray[index].max = m_Min + m_SectionSize * Vector3((float)(j + 1), 0.f, (float)(i + 1));
		}
	}
	size_t	size = m_vecCell.size();
	// 최대 4개의 영역에 포함될 수 있다.
	for (size_t i = 0; i < size; ++i)
	{
		int	minX = 1000000;
		int minY = 1000000;
		int maxX = -1000000;
		int maxY = -1000000;
		for (int j = 0; j < 3; ++j)
		{
			Vector3	pos = m_vecCell[i]->pos[j] - m_Min;
			pos /= m_SectionSize;
			int	x = (int)pos.x;
			int	y = (int)pos.z;
			if (minX > x)
			{
				minX = x;
			}
			if (maxX > x)
			{
				maxX = x;
			}
			if (minY < y)
			{
				minY = y;
			}
			if (maxY > y)
			{
				maxY = y;
			}
		}
		minX = minX < 0 ? 0 : minX;
		minY = minY < 0 ? 0 : minY;
		maxX = maxX >= m_SectionX ? m_SectionX - 1 : maxX;
		maxY = maxY >= m_SectionY ? m_SectionY - 1 : maxY;
		for (int j = minY; j <= maxY; ++j)
		{
			for (int k = minX; k <= maxX; ++k)
			{
				int	index = j * m_SectionX + k;
				m_SectionArray[index].Add(m_vecCell[i]);
			}
		}
	}
}

bool CNavigationMesh::RayIntersectTriangle(Vector3& impactPoint, float& dist, const Vector3& pos, const Vector3& dir, 
	const Vector3& cellPos0, const Vector3& cellPos1, const Vector3& cellPos2)
{
	Vector3 e1, e2, h, s, q;
	float a, f, u, v;

	e1 = cellPos1 - cellPos0;
	e2 = cellPos2 - cellPos0;
	h = dir.Cross(e2);
	a = e1.Dot(h);
	if (a > -0.00001 && a < 0.00001)
	{
		return false;
	}
	f = 1.f / a;
	s = pos - cellPos0;
	u = f * s.Dot(h);
	if (u < 0.f || u > 1.f)
	{
		return false;
	}
	q = s.Cross(e1);
	v = f * dir.Dot(q);
	if (v < 0.f || u + v > 1.f)
	{
		return false;
	}
	dist = f * e2.Dot(q);
	if (dist > 0.00001)
	{
		impactPoint = pos + dir * dist;
		return true;
	}
	return false;
}

CScene* CNavigationMesh::GetScene() const
{
	return m_Mgr->GetScene();
}

std::string CNavigationMesh::GetSceneName() const
{
	return m_Mgr->GetScene()->GetName();
}

std::string CNavigationMesh::GetComponentName() const
{
	return m_Terrain->GetName();
}

void CNavigationMesh::CreateNavigation(CTerrainComponent* terrain)
{
	m_Terrain = terrain;
	m_Grid = m_Terrain->GetGrid();
	m_LineRectCountX = m_Terrain->GetCountX() - 1;
	m_LineRectCountY = m_Terrain->GetCountY() - 1;
	m_CellSize = m_Terrain->GetCellSize();
	int	faceCount = m_Terrain->GetFaceCount();
	for (int i = 0; i < faceCount; ++i)
	{
		unsigned int	faceIndex[3] = {};
		m_Terrain->GetFaceIndex(faceIndex, i);
		NavMeshInfo* cell = new NavMeshInfo;
		for (int j = 0; j < 3; ++j)
		{
			cell->pos[j] = m_Terrain->GetVertexPos(faceIndex[j]);
		}
		cell->edge[0] = cell->pos[1] - cell->pos[0];
		cell->edgeCenter[0] = (cell->pos[1] + cell->pos[0]) / 2.f;
		cell->edge[1] = cell->pos[2] - cell->pos[1];
		cell->edgeCenter[1] = (cell->pos[2] + cell->pos[1]) / 2.f;
		cell->edge[2] = cell->pos[2] - cell->pos[0];
		cell->edgeCenter[2] = (cell->pos[2] + cell->pos[0]) / 2.f;
		cell->center = (cell->pos[0] + cell->pos[1] + cell->pos[2]) / 3.f;
		cell->index = (int)m_vecCell.size();
		for (int j = 0; j < 3; ++j)
		{
			if (m_Min.x > cell->pos[j].x)
			{
				m_Min.x = cell->pos[j].x;
			}
			if (m_Min.y > cell->pos[j].y)
			{
				m_Min.y = cell->pos[j].y;
			}
			if (m_Min.z > cell->pos[j].z)
			{
				m_Min.z = cell->pos[j].z;
			}
			if (m_Max.x < cell->pos[j].x)
			{
				m_Max.x = cell->pos[j].x;
			}
			if (m_Max.y < cell->pos[j].y)
			{
				m_Max.y = cell->pos[j].y;
			}
			if (m_Max.z < cell->pos[j].z)
			{
				m_Max.z = cell->pos[j].z;
			}
			if (cell->min.x > cell->pos[j].x)
			{
				cell->min.x = cell->pos[j].x;
			}
			if (cell->min.y > cell->pos[j].y)
			{
				cell->min.y = cell->pos[j].y;
			}
			if (cell->min.z > cell->pos[j].z)
			{
				cell->min.z = cell->pos[j].z;
			}
			if (cell->max.x < cell->pos[j].x)
			{
				cell->max.x = cell->pos[j].x;
			}
			if (cell->max.y < cell->pos[j].y)
			{
				cell->max.y = cell->pos[j].y;
			}
			if (cell->max.z < cell->pos[j].z)
			{
				cell->max.z = cell->pos[j].z;
			}
		}
		Vector3	dir1 = cell->edge[0];
		Vector3	dir2 = cell->edge[2];
		dir1.Normalize();
		dir2.Normalize();
		Vector3	faceNormal = dir1.Cross(dir2);
		faceNormal.Normalize();
		float angle = Vector3::axis[(int)AXIS::AXIS_Y].Angle(faceNormal);
		cell->angle = angle;
		if (angle >= 50.f)
		{
			float	minY = cell->pos[0].y;
			float	maxY = cell->pos[0].y;
			minY = minY > cell->pos[1].y ? cell->pos[1].y : minY;
			minY = minY > cell->pos[2].y ? cell->pos[2].y : minY;
			maxY = maxY < cell->pos[1].y ? cell->pos[1].y : maxY;
			maxY = maxY < cell->pos[2].y ? cell->pos[2].y : maxY;
			float	height = maxY - minY;
			if (height >= 30.f)
			{
				cell->enable = false;
			}
		}
		m_vecCell.push_back(cell);
		m_vecCellFind.push_back(cell);
	}
	if (m_Grid)
	{
		CreateAdjGrid();
	}
	else
	{
		CreateAdj();
	}
	CreateSection();
}

float CNavigationMesh::GetHeight(const Vector3& pos)
{
	if (!m_Grid)
	{
		return 0.f;
	}
	Vector3	convertPos = pos - m_Min;
	float	maxHeight = m_Max.z - m_Min.z;
	convertPos.z = maxHeight - convertPos.z;
	convertPos.x /= m_CellSize.x;
	convertPos.z /= m_CellSize.y;
	float	ratioX = convertPos.x - (int)convertPos.x;
	float	ratioY = convertPos.z - (int)convertPos.z;
	int	index = -1;
	int	indexX = (int)convertPos.x;
	int	indexY = (int)convertPos.z;
	// 우상단 삼각형 인덱스를 구한다.
	index = indexY * m_LineRectCountX * 2 + indexX * 2;
	// 좌하단 삼각형에 존재할 경우
	if (ratioX < ratioY)
	{
		++index;
	}
	if (index >= m_vecCell.size())
	{
		return FLT_MAX;
	}
	NavMeshInfo* cell = m_vecCell[index];
	// 좌하단
	if (ratioX < ratioY)
	{
		return cell->pos[0].y + (cell->pos[1].y - cell->pos[2].y) * ratioX + (cell->pos[2].y - cell->pos[0].y) * ratioY;
	}
	// 우상단
	return cell->pos[0].y + (cell->pos[1].y - cell->pos[0].y) * ratioX + (cell->pos[2].y - cell->pos[1].y) * ratioY;
}

NavMeshInfo* CNavigationMesh::FindNavMeshInfo(const Vector3& pos)
{
	int	index = FindNavMeshInfoIndex(pos);
	if (index == -1)
	{
		return nullptr;
	}
	return m_vecCell[index];
}

int CNavigationMesh::FindNavMeshInfoIndex(const Vector3& pos)
{
	if (!m_Grid)
	{
		return -1;
	}
	Vector3	convertPos = pos - m_Min;
	float	maxHeight = m_Max.z - m_Min.z;
	convertPos.z = maxHeight - convertPos.z;
	convertPos.x /= m_CellSize.x;
	convertPos.z /= m_CellSize.y;
	int	indexX = (int)convertPos.x;
	int	indexY = (int)convertPos.z;
	if (indexX < 0 || indexX >= m_LineRectCountX)
	{
		return -1;
	}
	else if (indexY < 0 || indexY >= m_LineRectCountY)
	{
		return -1;
	}
	float	ratioX = convertPos.x - (int)convertPos.x;
	float	ratioY = convertPos.z - (int)convertPos.z;
	if (ratioX >= ratioY)
	{
		return indexY * m_LineRectCountX * 2 + indexX * 2;
	}
	return indexY * m_LineRectCountX * 2 + indexX * 2 + 1;
}

NavMeshInfo* CNavigationMesh::GetNavMeshInfo(int index)
{
	if (index < 0 || index >= (int)m_vecCell.size())
	{
		return nullptr;
	}
	return m_vecCell[index];
}

bool CNavigationMesh::GetPickingPos(Vector3& result, const Ray& ray)
{
	// 1. 광선이 관통하는 섹션 목록을 구한다.
	// 2. 카메라의 가까운 순서대로 섹션 목록을 정렬한다.
	// 3. 섹션 안에 있는 셀과 충돌처리를 진행하여 관통하는 삼각형을 구한다.
	// 이때, 셀은 카메라와 가까운 순서대로 정렬하여 검사한다.
	// 단, DDT 를 적용했을 경우 위는 무시한다.
	std::sort(m_vecCellFind.begin(), m_vecCellFind.end(), CNavigationMesh::SortCamera);
	size_t	size = m_vecCellFind.size();
	for (size_t i = 0; i < size; ++i)
	{
		NavMeshInfo* cell = m_vecCellFind[i];
		float	dist = 0.f;
		if (RayIntersectTriangle(result, dist, ray.pos, ray.dir, cell->pos[0], cell->pos[1], cell->pos[2]))
		{
			return true;
		}
	}
	return false;
}
