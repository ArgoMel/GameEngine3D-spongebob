#include "MeshManager.h"
#include "SpriteMesh.h"
#include "StaticMesh.h"
#include "AnimationMesh.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::Init()
{
	// 기본 메쉬 생성 센터 중심, 좌하단 중심
	VertexColor	centerColorMesh[4] =
	{
		VertexColor(Vector3(-0.5f, 0.5f, 0.f), Vector4::red),
		VertexColor(Vector3(0.5f, 0.5f, 0.f), Vector4::green),
		VertexColor(Vector3(-0.5f, -0.5f, 0.f), Vector4::blue),
		VertexColor(Vector3(0.5f, -0.5f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f))
	};
	unsigned int	rectIndex[6] = { 0, 1, 3, 0, 3, 2 };
	CreateMesh(nullptr, MeshType::Sprite, "CenterColorRect", centerColorMesh, sizeof(VertexColor), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
	CMesh* mesh = FindMesh("CenterColorRect");
	mesh->SetMaterial(0, 0, "DefaultColor");

	VertexColor	LBColorMesh[4] =
	{
		VertexColor(Vector3(0.f, 1.f, 0.f), Vector4::red),
		VertexColor(Vector3(1.f, 1.f, 0.f), Vector4::green),
		VertexColor(Vector3(0.f, 0.f, 0.f), Vector4::blue),
		VertexColor(Vector3(1.f, 0.f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f))
	};
	CreateMesh(nullptr, MeshType::Sprite, "LBColorRect", LBColorMesh, sizeof(VertexColor), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
	mesh = FindMesh("LBColorRect");
	mesh->SetMaterial(0, 0, "DefaultColor");

	VertexUV	CenterUVMesh[4] =
	{
		VertexUV(Vector3(-0.5f, 0.5f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(0.5f, 0.5f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(-0.5f, -0.5f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(0.5f, -0.5f, 0.f), Vector2(1.f, 1.f))
	};
	CreateMesh(nullptr, MeshType::Sprite, "CenterUVRect", CenterUVMesh, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
	mesh = FindMesh("CenterUVRect");
	mesh->SetMaterial(0, 0, "DefaultTexture");

	VertexUV	LBUVMesh[4] =
	{
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};
	CreateMesh(nullptr, MeshType::Sprite, "LBUVRect", LBUVMesh, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
	mesh = FindMesh("LBUVRect");
	mesh->SetMaterial(0, 0, "DefaultTexture");

	Vector3	box2DLineMesh[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};
	CreateMesh(nullptr, MeshType::Sprite, "Box2DLineMesh", box2DLineMesh, sizeof(Vector3), 5, 
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Vector3	sphere2DLineMesh[361];
	sphere2DLineMesh[0].x = 1.f;
	sphere2DLineMesh[0].y = 0.f;
	for (int i = 0; i < 360; ++i)
	{
		float radian = (i + 1) * PI / 180.f;
		sphere2DLineMesh[i + 1].x = cosf(radian);
		sphere2DLineMesh[i + 1].y = sinf(radian);
	}
	sphere2DLineMesh[360].x = 1.f;
	sphere2DLineMesh[360].y = 0.f;
	CreateMesh(nullptr, MeshType::Sprite, "Sphere2DLineMesh", sphere2DLineMesh, sizeof(Vector3), 361, 
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	std::vector<Vertex3D>	vecSphere;
	std::vector<Vector3>	vecSpherePos;
	std::vector<int>		vecSphereIndex;
	CreateSphere(vecSphere, vecSphereIndex, 1.f, 10);
	size_t	sphereSize = vecSphere.size();
	vecSpherePos.resize(sphereSize);
	for (size_t i = 0; i < sphereSize; ++i)
	{
		vecSpherePos[i] = vecSphere[i].pos;
	}
	CreateMesh(nullptr, MeshType::Static, "SpherePos", &vecSpherePos[0], sizeof(Vector3), (int)sphereSize,
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecSphereIndex[0], sizeof(int), 
		(int)vecSphereIndex.size(), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	CreateMesh(nullptr, MeshType::Static, "GizMo",
		&vecSpherePos[0], sizeof(Vector3), (int)sphereSize,
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		&vecSphereIndex[0], sizeof(int), (int)vecSphereIndex.size(),
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
	mesh = FindMesh("GizMo");
	mesh->SetMaterial(0, 0, "DefaultTexture");

	Vector3		point;
	CreateMesh(nullptr, MeshType::Static, "ParticlePoint", &point, sizeof(Vector3), 1, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	Vector3	CubePos[8] =
	{
		Vector3(-0.5f, 0.5f, -0.5f),
		Vector3(0.5f, 0.5f, -0.5f),
		Vector3(-0.5f, -0.5f, -0.5f),
		Vector3(0.5f, -0.5f, -0.5f),

		Vector3(-0.5f, 0.5f, 0.5f),
		Vector3(0.5f, 0.5f, 0.5f),
		Vector3(-0.5f, -0.5f, 0.5f),
		Vector3(0.5f, -0.5f, 0.5f)
	};
	int	CubeLineIndex[24] =
	{
		0, 1, 1, 3, 2, 3, 0, 2, 4, 0, 5, 1, 6, 2, 7, 3, 4, 5, 5, 7, 7, 6, 6, 4
	};
	CreateMesh(nullptr, MeshType::Static, "CubeLinePos", &CubePos[0], sizeof(Vector3), 8, 
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, &CubeLineIndex[0], sizeof(int), 24, 
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	int	CubeIndex[36] =
	{
		0, 1, 3, 0, 3, 2,	// Near
		1, 5, 7, 1, 7, 3,	// Right
		5, 4, 6, 5, 6, 7,	// Far
		4, 0, 2, 4, 2, 6,	// Left
		4, 5, 1, 4, 1, 0,	// Top
		2, 3, 7, 2, 7, 6	// Bottom
	};
	CreateMesh(nullptr, MeshType::Static, "CubePos", &CubePos[0], sizeof(Vector3), 8, 
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &CubeIndex[0], sizeof(int), 36,
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
	return true;
}

bool CMeshManager::CreateMesh(CScene* scene, MeshType type, const std::string& name, void* vtxData, 
	int size, int count, D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData, 
	int idxSize, int idxCount, D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	if (FindMesh(name))
	{
		return true;
	}
	CMesh* mesh = nullptr;
	switch (type)
	{
	case MeshType::Sprite:
		mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		mesh = new CAnimationMesh;
		break;
	}
	mesh->SetScene(scene);
	mesh->SetName(name);
	if (!mesh->CreateMesh(vtxData, size, count, vtxUsage, primitive,idxData, idxSize, idxCount, idxUsage, fmt))
	{
		SAFE_RELEASE(mesh);
		return false;
	}
	m_mapMesh.insert(std::make_pair(name, mesh));
	return true;
}

bool CMeshManager::LoadMesh(CScene* scene, MeshType type, const std::string& name, const TCHAR* fileName, 
	const std::string& pathName)
{
	if (FindMesh(name))
	{
		return true;
	}
	CMesh* mesh = nullptr;
	switch (type)
	{
	case MeshType::Sprite:
		mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		mesh = new CAnimationMesh;
		break;
	}
	mesh->SetScene(scene);
	mesh->SetName(name);
	if (!mesh->LoadMesh(name, fileName, pathName))
	{
		SAFE_RELEASE(mesh);
		return false;
	}
	m_mapMesh.insert(std::make_pair(name, mesh));
	return true;
}

bool CMeshManager::LoadMeshFullPath(CScene* scene, MeshType type, const std::string& name, 
	const TCHAR* fullPath)
{
	if (FindMesh(name))
	{
		return true;
	}
	CMesh* mesh = nullptr;
	switch (type)
	{
	case MeshType::Sprite:
		mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		mesh = new CAnimationMesh;
		break;
	}
	mesh->SetScene(scene);
	mesh->SetName(name);
	if (!mesh->LoadMeshFullPath(name, fullPath))
	{
		SAFE_RELEASE(mesh);
		return false;
	}
	m_mapMesh.insert(std::make_pair(name, mesh));
	return true;
}

bool CMeshManager::LoadMeshMultibyte(CScene* scene, MeshType type, const std::string& name, 
	const char* fileName, 
	const std::string& pathName)
{
	if (FindMesh(name))
	{
		return true;
	}
	CMesh* mesh = nullptr;
	switch (type)
	{
	case MeshType::Sprite:
		mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		mesh = new CAnimationMesh;
		break;
	}
	mesh->SetScene(scene);
	mesh->SetName(name);
	if (!mesh->LoadMeshMultibyte(name, fileName, pathName))
	{
		SAFE_RELEASE(mesh);
		return false;
	}
	m_mapMesh.insert(std::make_pair(name, mesh));
	return true;
}

bool CMeshManager::LoadMeshMultibyteFullPath(CScene* scene, MeshType type, const std::string& name, 
	const char* fullPath)
{
	if (FindMesh(name))
	{
		return true;
	}
	CMesh* mesh = nullptr;
	switch (type)
	{
	case MeshType::Sprite:
		mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		mesh = new CStaticMesh;
		break;
	case MeshType::Animation:
		mesh = new CAnimationMesh;
		break;
	}
	mesh->SetScene(scene);
	mesh->SetName(name);
	if (!mesh->LoadMeshMultibyteFullPath(name, fullPath))
	{
		SAFE_RELEASE(mesh);
		return false;
	}
	m_mapMesh.insert(std::make_pair(name, mesh));
	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& name)
{
	auto	iter = m_mapMesh.find(name);
	if (iter == m_mapMesh.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CMeshManager::ReleaseMesh(const std::string& name)
{
	auto	iter = m_mapMesh.find(name);
	if (iter != m_mapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapMesh.erase(iter);
		}
	}
}

bool CMeshManager::CreateSphere(std::vector<Vertex3D>& vecVertex, std::vector<int>& vecIndex, float radius, 
	unsigned int subDivision)
{
	subDivision = min(subDivision, 5);	// Put a cap on the number of subdivisions.
	// Approximate a sphere by tessellating an icosahedron.
	const float X = 0.525731f;
	const float Z = 0.850651f;
	Vector3 pos[12] =
	{
		Vector3(-X, 0.0f, Z),  Vector3(X, 0.0f, Z),
		Vector3(-X, 0.0f, -Z), Vector3(X, 0.0f, -Z),
		Vector3(0.0f, Z, X),   Vector3(0.0f, Z, -X),
		Vector3(0.0f, -Z, X),  Vector3(0.0f, -Z, -X),
		Vector3(Z, X, 0.0f),   Vector3(-Z, X, 0.0f),
		Vector3(Z, -X, 0.0f),  Vector3(-Z, -X, 0.0f)
	};
	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};
	vecVertex.resize(12);
	vecIndex.resize(60);
	for (UINT i = 0; i < 12; ++i)
	{
		vecVertex[i].pos = pos[i];
	}
	for (UINT i = 0; i < 60; ++i)
	{
		vecIndex[i] = k[i];
	}
	for (UINT i = 0; i < subDivision; ++i)
	{
		Subdivide(vecVertex, vecIndex);
	}
	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < vecVertex.size(); ++i)
	{
		// Project onto unit sphere.
		Vector3	vN = vecVertex[i].pos;
		vN.Normalize();
		// Project onto sphere.
		Vector3 p = vN * radius;
		vecVertex[i].pos = p;
		// Normal이 있을 경우 따로 저장한다.
		// Derive texture coordinates from spherical coordinates.
		float theta = AngleFromXY(vecVertex[i].pos.x, vecVertex[i].pos.z);
		float phi = acosf(vecVertex[i].pos.y / radius);
		vecVertex[i].UV.x = theta / DirectX::XM_2PI;
		vecVertex[i].UV.y = phi / DirectX::XM_PI;
		// Partial derivative of P with respect to theta
		vecVertex[i].tangent.x = -radius * sinf(phi) * sinf(theta);
		vecVertex[i].tangent.y = 0.0f;
		vecVertex[i].tangent.z = radius * sinf(phi) * cosf(theta);
		vecVertex[i].binormal = vecVertex[i].normal.Cross(vecVertex[i].tangent);
		vecVertex[i].binormal.Normalize();
	}
	return true;
}

void CMeshManager::Subdivide(std::vector<Vertex3D>& vecVertices, std::vector<int>& vecIndices)
{
	// Save a copy of the input geometry.
	std::vector<Vertex3D>	vecCopyVertex = vecVertices;
	std::vector<int>	vecCopyIndex = vecIndices;
	vecVertices.resize(0);
	vecIndices.resize(0);
	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2
	UINT numTris = (UINT)(vecCopyIndex.size() / 3);
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex3D v0 = vecCopyVertex[vecCopyIndex[i * 3 + 0]];
		Vertex3D v1 = vecCopyVertex[vecCopyIndex[i * 3 + 1]];
		Vertex3D v2 = vecCopyVertex[vecCopyIndex[i * 3 + 2]];
		// Generate the midpoints.
		Vertex3D m0, m1, m2;
		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.
		m0.pos = Vector3(
			0.5f * (v0.pos.x + v1.pos.x),
			0.5f * (v0.pos.y + v1.pos.y),
			0.5f * (v0.pos.z + v1.pos.z));
		m1.pos = Vector3(
			0.5f * (v1.pos.x + v2.pos.x),
			0.5f * (v1.pos.y + v2.pos.y),
			0.5f * (v1.pos.z + v2.pos.z));
		m2.pos = Vector3(
			0.5f * (v0.pos.x + v2.pos.x),
			0.5f * (v0.pos.y + v2.pos.y),
			0.5f * (v0.pos.z + v2.pos.z));
		// Add new geometry.
		vecVertices.push_back(v0); // 0
		vecVertices.push_back(v1); // 1
		vecVertices.push_back(v2); // 2
		vecVertices.push_back(m0); // 3
		vecVertices.push_back(m1); // 4
		vecVertices.push_back(m2); // 5

		vecIndices.push_back(i * 6 + 0);
		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 5);

		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 4);
		vecIndices.push_back(i * 6 + 5);

		vecIndices.push_back(i * 6 + 5);
		vecIndices.push_back(i * 6 + 4);
		vecIndices.push_back(i * 6 + 2);

		vecIndices.push_back(i * 6 + 3);
		vecIndices.push_back(i * 6 + 1);
		vecIndices.push_back(i * 6 + 4);
	}
}

float CMeshManager::AngleFromXY(float x, float y)
{
	float theta = 0.0f;
	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]
		if (theta < 0.0f)
		{
			theta += 2.0f * PI; // in [0, 2*pi).
		}
	}
	// Quadrant II or III
	else
	{
		theta = atanf(y / x) + PI; // in [0, 2*pi).
	}
	return theta;
}