#include "OceanComponent.h"
#include "../PathManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
//#include "../Scene/NavigationManager3D.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/TerrainConstantBuffer.h"
//#include "../Thread/ThreadManager.h"

COceanComponent::COceanComponent()
	: m_CountX(0)
	, m_CountY(0)
	, m_HeightMapX(0)
	, m_HeightMapY(0)
	, m_Grid(true)
	, m_Height(0.5f)
	, m_Time(0.f)
	, m_MaxTime(1.f)
	, m_Dir(1.f)
{
    SetTypeID<COceanComponent>();
    m_ComponentTypeName = "OceanComponent";
    m_CBuffer = new CTerrainConstantBuffer;
    m_CBuffer->Init();
}

COceanComponent::COceanComponent(const COceanComponent& component)
    : CPrimitiveComponent(component)
	, m_Time(0.f)
	, m_Dir(1.f)
	, m_MaxTime(component.m_MaxTime)
{
	m_CountX = component.m_CountX;
	m_CountY = component.m_CountY;
	m_Size = component.m_Size;
	m_CellSize = component.m_CellSize;
	if (component.m_CBuffer)
	{
		m_CBuffer = component.m_CBuffer->Clone();
	}
	m_Grid = component.m_Grid;
	m_Height = component.m_Height;
}

COceanComponent::~COceanComponent()
{
    SAFE_DELETE(m_CBuffer);
}

float COceanComponent::GetDetailLevel() const
{
	return m_CBuffer->GetDetailLevel();
}

int COceanComponent::GetSplatCount() const
{
	return m_CBuffer->GetSplatCount();
}

float COceanComponent::GetTime() const
{
	return m_CBuffer->GetTime();
}

float COceanComponent::GetWaveSpeed() const
{
	return m_CBuffer->GetWaveSpeed();
}

void COceanComponent::SetDetailLevel(float level)
{
	m_CBuffer->SetDetailLevel(level);
	m_CBuffer->UpdateBuffer();
}

void COceanComponent::SetSplatCount(int count)
{
	m_CBuffer->SetSplatCount(count);
	m_CBuffer->UpdateBuffer();
}

void COceanComponent::SetTime(float time)
{
	m_CBuffer->SetTime(time);
	m_CBuffer->UpdateBuffer();
}

void COceanComponent::SetWaveSpeed(float speed)
{
	m_CBuffer->SetWaveSpeed(speed);
	m_CBuffer->UpdateBuffer();
}

bool COceanComponent::SetMesh(const std::string& name)
{
    return CPrimitiveComponent::SetMesh(name);
}

bool COceanComponent::SetMesh(CMesh* mesh)
{
    return CPrimitiveComponent::SetMesh(mesh);
}

bool COceanComponent::SetMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (m_Scene)
	{
		m_Scene->GetResource()->LoadMesh(MeshType::Static, name, fileName, pathName);
		m_Mesh = m_Scene->GetResource()->FindMesh(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadMesh(m_Scene, MeshType::Static, name, fileName, pathName);
		m_Mesh = CResourceManager::GetInst()->FindMesh(name);
	}
	if (m_Mesh)
	{
		SetMin(m_Mesh->GetMin());
		SetMax(m_Mesh->GetMax());
		SetMeshSize(m_Mesh->GetMeshSize());
	}
	m_vecMaterial.clear();
	int slotCount = m_Mesh->GetSlotCount();
	for (int i = 0; i < slotCount; ++i)
	{
		CMaterial* material = m_Mesh->GetMaterial(i);
		m_vecMaterial.push_back(material->Clone());
	}
	return true;
}

bool COceanComponent::SetMeshFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (m_Scene)
	{
		m_Scene->GetResource()->LoadMeshFullPath(MeshType::Static, name, fullPath);
		m_Mesh = m_Scene->GetResource()->FindMesh(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadMeshFullPath(m_Scene, MeshType::Static, name, fullPath);
		m_Mesh = CResourceManager::GetInst()->FindMesh(name);
	}
	if (m_Mesh)
	{
		SetMin(m_Mesh->GetMin());
		SetMax(m_Mesh->GetMax());
		SetMeshSize(m_Mesh->GetMeshSize());
	}
	m_vecMaterial.clear();
	int slotCount = m_Mesh->GetSlotCount();
	for (int i = 0; i < slotCount; i++)
	{
		CMaterial* material = m_Mesh->GetMaterial(i);
		m_vecMaterial.push_back(material->Clone());
	}
	return true;
}

void COceanComponent::Start()
{
	CPrimitiveComponent::Start();
	SetDetailLevel(5.f);
}

bool COceanComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}
	//if (m_Scene)
	//{
	//	m_ShadowMapShader = (CGraphicShader*)m_Scene->GetResource()->FindShader("ShadowMapStaticShader");
	//}
	//else
	//{
	//	m_ShadowMapShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("ShadowMapStaticShader");
	//}
	return true;
}

void COceanComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
	if (deltaTime > m_MaxTime)
	{
		deltaTime = 0.f;
	}
	m_Time += deltaTime* m_Dir;

	if (m_Dir==1.f&&m_Time > m_MaxTime)
	{
		m_Dir*=-1.f;
	}
	else if(m_Dir == -1.f && m_Time < -m_MaxTime)
	{
		m_Dir *= -1.f;
	}
	SetTime(m_Time);
}

void COceanComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
}

void COceanComponent::RenderShadowMap()
{
	CPrimitiveComponent::RenderShadowMap();
}

void COceanComponent::Render()
{
	CPrimitiveComponent::Render();
}

COceanComponent* COceanComponent::Clone() const
{
	return new COceanComponent(*this);
}

void COceanComponent::Save(FILE* file)
{
	fwrite(&m_CountX, sizeof(int), 1, file);
	fwrite(&m_CountY, sizeof(int), 1, file);
	fwrite(&m_CellSize, sizeof(Vector2), 1, file);
	fwrite(&m_Grid, sizeof(bool), 1, file);
	fwrite(&m_Height, sizeof(float), 1, file);
	fwrite(&m_MaxTime, sizeof(float), 1, file);
	int Length = (int)m_FileName.length();
	fwrite(&Length, 4, 1, file);
	fwrite(m_FileName.c_str(), 1, Length, file);
	CSceneComponent::Save(file);
}

void COceanComponent::Load(FILE* file)
{
	fread(&m_CountX, sizeof(int), 1, file);
	fread(&m_CountY, sizeof(int), 1, file);
	fread(&m_CellSize, sizeof(Vector2), 1, file);
	fread(&m_Grid, sizeof(bool), 1, file);
	fread(&m_Height, sizeof(float), 1, file);
	fread(&m_MaxTime, sizeof(float), 1, file);
	size_t length = 0;
	char	Name[256] = {};
	fread(&length, sizeof(size_t), 1, file);
	if (length > MAXCHAR)
	{
		throw std::runtime_error("file data unexpected size");
	}
	//assert(Length < MAXCHAR);
	fread(Name, 1, length, file);
	m_FileName = Name;
	TCHAR* t_filename = new TCHAR[m_FileName.size() + 1];
	t_filename[m_FileName.size()] = 0;
	std::copy(m_FileName.begin(), m_FileName.end(), t_filename);
	CreateOcean(m_CountX, m_CountY, m_CellSize.x, m_CellSize.y, t_filename);
	SAFE_DELETE_ARRAY(t_filename);
	CSceneComponent::Load(file);
}

void COceanComponent::CreateOcean(int countX, int countY, float sizeX, float sizeY, const TCHAR* heightMapName, 
	const std::string& heightMapPath)
{
	m_CountX = countX;
	m_CountY = countY;
	m_CellSize.x = sizeX;
	m_CellSize.y = sizeY;
	m_Size = m_CellSize * Vector2((float)countX, (float)countY);

	std::vector<float>	vecY;
	float maxY = 0.f;
	char	meshName[256] = {};
	// HeightMap�� ���� ��� ���� ������ ���´�.
	if (heightMapName)
	{
		TCHAR fullPath[MAX_PATH] = {};
		const PathInfo* path = CPathManager::GetInst()->FindPath(heightMapPath);
		if (path)
		{
			lstrcpy(fullPath, path->path);
		}
		lstrcat(fullPath, heightMapName);
		char ext[_MAX_EXT] = {};
		char fullPathMultibyte[MAX_PATH] = {};
#ifdef UNICODE
		int	length = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultibyte, length, nullptr, nullptr);
		length = WideCharToMultiByte(CP_ACP, 0, heightMapName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, heightMapName, -1, meshName, length, nullptr, nullptr);
#else
		strcpy_s(fileNameMultibyte, fullPath);
		strcpy_s(meshName, heightMapName);
#endif // UNICODE
		m_FileName = meshName;
		_splitpath_s(fullPathMultibyte, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
		_strupr_s(ext);
		DirectX::ScratchImage img;
		if (strcmp(ext, ".DDS") == 0)
		{
			if (FAILED(DirectX::LoadFromDDSFile(fullPath, DirectX::DDS_FLAGS_NONE, nullptr, img)))
			{
				return;
			}
		}
		else if (strcmp(ext, ".TGA") == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(fullPath, nullptr, img)))
			{
				return;
			}
		}
		else
		{
			if (FAILED(DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_NONE, nullptr, img)))
			{
				return;
			}
		}
		const DirectX::Image* pixelInfo = img.GetImages();
		m_HeightMapX = (int)pixelInfo->width;
		m_HeightMapY = (int)pixelInfo->height;
		for (size_t i = 0; i < pixelInfo->height; i++)
		{
			for (size_t j = 0; j < pixelInfo->width; j++)
			{
				int	pixelIndex = (int)i * (int)pixelInfo->width * 4 + (int)j * 4;
				float Y = pixelInfo->pixels[pixelIndex];
				if (Y > maxY)
				{
					maxY = Y;
				}
				vecY.push_back(Y * m_Height);
			}
		}
	}
	else
	{
		vecY.resize(m_CountX * m_CountY);
	}
	Vector3	Min(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3	Max(FLT_MIN, FLT_MIN, FLT_MIN);
	// ���������� �ε��� ������ �����Ѵ�.
	for (int i = 0; i < m_CountY; i++)
	{
		for (int j = 0; j < m_CountX; j++)
		{
			Vertex3DStatic	vtx;
			vtx.pos = Vector3((float)j * m_CellSize.x, vecY[i * m_HeightMapX + j],
				(m_Size.y - m_CellSize.y) - (float)i * m_CellSize.y);
			vtx.UV = Vector2((float)j / (float)(m_CountX - 1), (float)i / (float)(m_CountY - 1));
			if (Min.x > vtx.pos.x)
			{
				Min.x = vtx.pos.x;
			}
			if (Min.y > vtx.pos.y)
			{
				Min.y = vtx.pos.y;
			}
			if (Min.z > vtx.pos.z)
			{
				Min.z = vtx.pos.z;
			}
			if (Max.x < vtx.pos.x)
			{
				Max.x = vtx.pos.x;
			}
			if (Max.y < vtx.pos.y)
			{
				Max.y = vtx.pos.y;
			}
			if (Max.z < vtx.pos.z)
			{
				Max.z = vtx.pos.z;
			}
			m_vecVtx.push_back(vtx);
		}
	}
	SetMin(Min);
	SetMax(Max);
	SetMeshSize(Max - Min);
	m_vecFaceNormal.resize((m_CountX - 1) * (m_CountY - 1) * 2);
	int	triIndex = 0;
	for (int i = 0; i < m_CountY - 1; i++)
	{
		for (int j = 0; j < m_CountX - 1; j++)
		{
			int index = i * m_CountX + j;
			m_vecIndex.push_back(index);
			m_vecIndex.push_back(index + 1);
			m_vecIndex.push_back(index + m_CountX + 1);
			Vector3 trianglePos1 = m_vecVtx[index].pos;
			Vector3 trianglePos2 = m_vecVtx[index + 1].pos;
			Vector3 trianglePos3 = m_vecVtx[index + m_CountX + 1].pos;
			Vector3 edge1 = trianglePos2 - trianglePos1;
			Vector3 edge2 = trianglePos3 - trianglePos1;
			edge1.Normalize();
			edge2.Normalize();
			m_vecFaceNormal[triIndex] = edge1.Cross(edge2);
			m_vecFaceNormal[triIndex].Normalize();
			triIndex++;
			m_vecIndex.push_back(index);
			m_vecIndex.push_back(index + m_CountX + 1);
			m_vecIndex.push_back(index + m_CountX);
			trianglePos1 = m_vecVtx[index].pos;
			trianglePos2 = m_vecVtx[index + m_CountX + 1].pos;
			trianglePos3 = m_vecVtx[index + m_CountX].pos;
			edge1 = trianglePos2 - trianglePos1;
			edge2 = trianglePos3 - trianglePos1;
			edge1.Normalize();
			edge2.Normalize();
			m_vecFaceNormal[triIndex] = edge1.Cross(edge2);
			m_vecFaceNormal[triIndex].Normalize();
			++triIndex;
		}
	}
	ComputeNormal();
	ComputeTangent();
	// ������ ������ �޽��� �����.
	m_Scene->GetResource()->CreateMesh(MeshType::Static, meshName, &m_vecVtx[0], sizeof(Vertex3DStatic),
		(int)m_vecVtx.size(), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vecIndex[0], 4,
		(int)m_vecIndex.size(), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
	m_Mesh = m_Scene->GetResource()->FindMesh(meshName);
	AddMaterial("Sea");
}

void COceanComponent::DeleteOcean()
{
	CResourceManager::GetInst()->ReleaseMesh(m_FileName);
	m_vecVtx.clear();
	m_vecIndex.clear();
	m_vecFaceNormal.clear();
	m_vecMaterial.clear();
}

void COceanComponent::ComputeNormal()
{
	size_t	size = m_vecFaceNormal.size();
	for (size_t i = 0; i < size; i++)
	{
		unsigned int Idx0 = m_vecIndex[i * 3];
		unsigned int Idx1 = m_vecIndex[i * 3 + 1];
		unsigned int Idx2 = m_vecIndex[i * 3 + 2];
		m_vecVtx[Idx0].normal += m_vecFaceNormal[i];
		m_vecVtx[Idx1].normal += m_vecFaceNormal[i];
		m_vecVtx[Idx2].normal += m_vecFaceNormal[i];
	}
	size = m_vecVtx.size();
	for (size_t i = 0; i < size; i++)
	{
		m_vecVtx[i].normal.Normalize();
	}
}

void COceanComponent::ComputeTangent()
{
	size_t size = m_vecFaceNormal.size();
	for (size_t i = 0; i < size; ++i)
	{
		unsigned int idx0 = m_vecIndex[i * 3];
		unsigned int idx1 = m_vecIndex[i * 3 + 1];
		unsigned int idx2 = m_vecIndex[i * 3 + 2];
		float	fVtx1[3], fVtx2[3];
		fVtx1[0] = m_vecVtx[idx1].pos.x - m_vecVtx[idx0].pos.x;
		fVtx1[1] = m_vecVtx[idx1].pos.y - m_vecVtx[idx0].pos.y;
		fVtx1[2] = m_vecVtx[idx1].pos.z - m_vecVtx[idx0].pos.z;
		fVtx2[0] = m_vecVtx[idx2].pos.x - m_vecVtx[idx0].pos.x;
		fVtx2[1] = m_vecVtx[idx2].pos.y - m_vecVtx[idx0].pos.y;
		fVtx2[2] = m_vecVtx[idx2].pos.z - m_vecVtx[idx0].pos.z;
		float	ftu[2], ftv[2];
		ftu[0] = m_vecVtx[idx1].UV.x - m_vecVtx[idx0].UV.x;
		ftv[0] = m_vecVtx[idx1].UV.y - m_vecVtx[idx0].UV.y;
		ftu[1] = m_vecVtx[idx2].UV.x - m_vecVtx[idx0].UV.x;
		ftv[1] = m_vecVtx[idx2].UV.y - m_vecVtx[idx0].UV.y;
		float	fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);
		Vector3	tangent;
		tangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
		tangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
		tangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;
		tangent.Normalize();
		m_vecVtx[idx0].tangent = tangent;
		m_vecVtx[idx1].tangent = tangent;
		m_vecVtx[idx2].tangent = tangent;
		m_vecVtx[idx0].binormal = m_vecVtx[idx0].normal.Cross(tangent);
		m_vecVtx[idx1].binormal = m_vecVtx[idx1].normal.Cross(tangent);
		m_vecVtx[idx2].binormal = m_vecVtx[idx2].normal.Cross(tangent);
		m_vecVtx[idx0].binormal.Normalize();
		m_vecVtx[idx1].binormal.Normalize();
		m_vecVtx[idx2].binormal.Normalize();
	}
}
