#include "Mesh.h"
#include "FBXLoader.h"
#include "../../Device.h"
#include "../Material/Material.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CMesh::CMesh()
	: m_Scene(nullptr)
	, m_Min(FLT_MAX, FLT_MAX, FLT_MAX)
	, m_Max(FLT_MIN, FLT_MIN, FLT_MIN)
	, m_MeshType(MeshType::Sprite)
	, m_RenderCount(0)
{
}

CMesh::~CMesh()
{
	size_t	size = m_vecContainer.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
	size = m_vecMeshSlot.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecMeshSlot[i]);
	}
}

void CMesh::AddRenderCount(bool add)
{
	if (add)
	{
		++m_RenderCount;
	}
	else
	{
		--m_RenderCount;
	}
}

void CMesh::SetMaterial(int container, int subset, const std::string& name)
{
	CMaterial* material = nullptr;
	if (m_Scene)
	{
		material = m_Scene->GetResource()->FindMaterial(name);
	}
	else
	{
		material = CResourceManager::GetInst()->FindMaterial(name);
	}
	m_vecContainer[container]->vecSubset[subset].material = material;
	m_vecContainer[container]->vecSubset[subset].slot->material = material;
}

void CMesh::SetMaterial(int container, int subset, CMaterial* material)
{
	m_vecContainer[container]->vecSubset[subset].material = material;
	m_vecContainer[container]->vecSubset[subset].slot->material = material;
}

bool CMesh::CreateMesh(void* vtxData, int size, int count, D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive,
	void* idxData, int idxSize, int idxCount, D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	MeshContainer* container = new MeshContainer;
	container->VB.size = size;
	container->VB.count = count;
	container->primitive = primitive;
	int dataSize = size * count;
	container->VB.data = new char[dataSize];
	memcpy(container->VB.data, vtxData, dataSize);
	if (!CreateBuffer(BufferType::Vertex, vtxData, size, count, vtxUsage, &container->VB.buffer))
	{
		return false;
	}
	m_vecContainer.push_back(container);
	MeshSlot* slot = new MeshSlot;
	m_vecMeshSlot.push_back(slot);
	slot->VB = &container->VB;
	slot->primitive = container->primitive;
	if (idxData != nullptr)
	{
		MeshSubset	subset;
		container->vecSubset.push_back(subset);
		int index = (int)container->vecSubset.size() - 1;
		container->vecSubset[index].slot = slot;
		slot->IB = &container->vecSubset[index].IB;
		slot->IB->size = idxSize;
		slot->IB->count = idxCount;
		slot->IB->fmt = fmt;
		dataSize = idxSize * idxCount;
		slot->IB->data = new char[dataSize];
		memcpy(slot->IB->data, idxData, dataSize);
		if (!CreateBuffer(BufferType::Index, idxData, idxSize, idxCount, idxUsage, &slot->IB->buffer))
		{
			return false;
		}
	}
	return true;
}

bool CMesh::LoadMesh(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	return false;
}

bool CMesh::LoadMeshFullPath(const std::string& name, const TCHAR* fullPath)
{
	return false;
}

bool CMesh::LoadMeshMultibyte(const std::string& name, const char* fileName, const std::string& pathName)
{
	return false;
}

bool CMesh::LoadMeshMultibyteFullPath(const std::string& name, const char* fullPath)
{
	return false;
}

void CMesh::Render()
{
	size_t	size = m_vecMeshSlot.size();
	for (size_t i = 0; i < size; ++i)
	{
		unsigned int	stride = (unsigned int)m_vecMeshSlot[i]->VB->size;
		unsigned int	offset = 0;
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[i]->primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,&m_vecMeshSlot[i]->VB->buffer, &stride, &offset);
		if (m_vecMeshSlot[i]->IB)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecMeshSlot[i]->IB->buffer, m_vecMeshSlot[i]->IB->fmt,0);
			CDevice::GetInst()->GetContext()->DrawIndexed(m_vecMeshSlot[i]->IB->count, 0, 0);
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->Draw(m_vecMeshSlot[i]->VB->count, 0);
		}
	}
}

void CMesh::Render(int slotNumber)
{
	unsigned int	stride = (unsigned int)m_vecMeshSlot[slotNumber]->VB->size;
	unsigned int	offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[slotNumber]->primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,&m_vecMeshSlot[slotNumber]->VB->buffer, &stride, &offset);
	if (m_vecMeshSlot[slotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->
			IASetIndexBuffer(m_vecMeshSlot[slotNumber]->IB->buffer, m_vecMeshSlot[slotNumber]->IB->fmt,0);
		CDevice::GetInst()->GetContext()->DrawIndexed(m_vecMeshSlot[slotNumber]->IB->count, 0, 0);
	}
	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(m_vecMeshSlot[slotNumber]->VB->count, 0);
	}
}

void CMesh::RenderInstancing(int count)
{
	size_t	size = m_vecMeshSlot.size();
	for (size_t i = 0; i < size; ++i)
	{
		unsigned int	stride = (unsigned int)m_vecMeshSlot[i]->VB->size;
		unsigned int	offset = 0;
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[i]->primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &m_vecMeshSlot[i]->VB->buffer, &stride, &offset);
		if (m_vecMeshSlot[i]->IB)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecMeshSlot[i]->IB->buffer, m_vecMeshSlot[i]->IB->fmt, 0);
			CDevice::GetInst()->GetContext()->DrawIndexedInstanced(m_vecMeshSlot[i]->IB->count, count, 0, 0, 0);
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->DrawInstanced(m_vecMeshSlot[i]->VB->count, count, 0, 0);
		}
	}
}

void CMesh::RenderInstancing(int count, int slotNumber)
{
	unsigned int	stride = (unsigned int)m_vecMeshSlot[slotNumber]->VB->size;
	unsigned int	offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[slotNumber]->primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &m_vecMeshSlot[slotNumber]->VB->buffer, &stride, &offset);
	if (m_vecMeshSlot[slotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			m_vecMeshSlot[slotNumber]->IB->buffer, m_vecMeshSlot[slotNumber]->IB->fmt,0);
		CDevice::GetInst()->GetContext()->DrawIndexedInstanced(m_vecMeshSlot[slotNumber]->IB->count, count, 0, 0, 0);
	}
	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->DrawInstanced(m_vecMeshSlot[slotNumber]->VB->count, count, 0, 0);
	}
}

bool CMesh::CreateBuffer(BufferType type, void* data, int size, int count, D3D11_USAGE usage, ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC	desc = {};
	desc.ByteWidth = size * count;
	desc.Usage = usage;
	if (type == BufferType::Vertex)
	{
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	}
	else
	{
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	}
	if (usage == D3D11_USAGE_DYNAMIC)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (usage == D3D11_USAGE_STAGING)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
	D3D11_SUBRESOURCE_DATA	bufferData = {};
	bufferData.pSysMem = data;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, &bufferData, buffer)))
	{
		return false;
	}
	if (type == BufferType::Vertex)
	{
		char* vertexData = (char*)data;		// 정점은 위치 12바이트가 항상 먼저 들어온다
		for (int i = 0; i < count; ++i)
		{
			Vector3* pos = (Vector3*)vertexData;
			vertexData += size;
			if (m_Min.x > pos->x)
			{
				m_Min.x = pos->x;
			}
			if (m_Min.y > pos->y)
			{
				m_Min.y = pos->y;
			}
			if (m_Min.z > pos->z)
			{
				m_Min.z = pos->z;
			}
			if (m_Max.x < pos->x)
			{
				m_Max.x = pos->x;
			}
			if (m_Max.y < pos->y)
			{
				m_Max.y = pos->y;
			}
			if (m_Max.z < pos->z)
			{
				m_Max.z = pos->z;
			}
		}
	}
	return true;
}

bool CMesh::ConvertFBX(CFBXLoader* loader, const char* fullPath)
{
	const std::vector<std::vector<FbxMaterial*>>* vecMaterials = loader->GetMaterials();
	const std::vector<FbxMeshContainer*>* vecContainers = loader->GetContainer();
	auto    iter = vecContainers->begin();
	auto    iterEnd = vecContainers->end();
	std::vector<std::vector<bool>>  vecEmptyIndex;
	int containerIndex = 0;
	bool    bumpEnable = false;
	bool    animationEnable = false;
	for (; iter != iterEnd; ++iter, ++containerIndex)
	{
		MeshContainer* container = new MeshContainer;
		m_vecContainer.push_back(container);
		std::vector<bool>    vecEmpty;
		vecEmptyIndex.push_back(vecEmpty);
		if ((*iter)->Bump)
		{
			bumpEnable = true;
		}
		if ((*iter)->Animation)
		{
			animationEnable = true;
		}
		std::vector<Vertex3D>   vecVtx;
		size_t  vtxCount = (*iter)->vecPos.size();
		vecVtx.resize(vtxCount);
		for (size_t i = 0; i < vtxCount; ++i)
		{
			Vertex3D    vtx = {};
			vtx.pos = (*iter)->vecPos[i];
			vtx.normal = (*iter)->vecNormal[i];
			vtx.UV = (*iter)->vecUV[i];
			//vtx.color = (*iter)->vecColor[i];
			if (bumpEnable)
			{
				if (!(*iter)->vecTangent.empty())
				{
					vtx.tangent = (*iter)->vecTangent[i];
				}
				if (!(*iter)->vecBinormal.empty())
				{
					vtx.binormal = (*iter)->vecBinormal[i];
				}
			}
			if (!(*iter)->vecBlendWeight.empty() && animationEnable)
			{
				vtx.blendWeight = (*iter)->vecBlendWeight[i];
				vtx.blendIndex = (*iter)->vecBlendIndex[i];
			}
			vecVtx[i] = vtx;
		}
		// Mesh의 Vertex 생성
		if (!CreateBuffer(BufferType::Vertex, &vecVtx[0], sizeof(Vertex3D), (int)vecVtx.size(),
			D3D11_USAGE_DEFAULT, &container->VB.buffer))
		{
			return false;
		}
		container->VB.size = sizeof(Vertex3D);
		container->VB.count = (int)vecVtx.size();
		container->VB.data = new char[container->VB.size * container->VB.count];
		memcpy(container->VB.data, &vecVtx[0], container->VB.size * container->VB.count);
		container->primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		size_t  idxCount = (*iter)->vecIndex.size();
		container->vecSubset.reserve(idxCount);
		for (size_t i = 0; i < idxCount; ++i)
		{
			// 인덱스 버퍼를 채워줄 데이터가 없다면 해당 서브셋은 폐기한다.
			if ((*iter)->vecIndex[i].empty())
			{
				vecEmptyIndex[containerIndex].push_back(false);
				continue;
			}
			vecEmptyIndex[containerIndex].push_back(true);
			MeshSlot* slot = new MeshSlot;
			m_vecMeshSlot.push_back(slot);
			slot->VB = &container->VB;
			slot->primitive = container->primitive;
			MeshSubset	subset = {};
			container->vecSubset.emplace_back(subset);
			if (!CreateBuffer(BufferType::Index, &(*iter)->vecIndex[i][0], sizeof(UINT), (int)(*iter)->vecIndex[i].size(),
				D3D11_USAGE_DEFAULT, &container->vecSubset[i].IB.buffer))
			{
				return false;
			}
			container->vecSubset[i].IB.size = sizeof(UINT);
			container->vecSubset[i].IB.count = (int)(*iter)->vecIndex[i].size();
			container->vecSubset[i].IB.fmt = DXGI_FORMAT_R32_UINT;
			container->vecSubset[i].IB.data = new char[container->vecSubset[i].IB.size * container->vecSubset[i].IB.count];
			memcpy(container->vecSubset[i].IB.data, &(*iter)->vecIndex[i][0], 
				container->vecSubset[i].IB.size * container->vecSubset[i].IB.count);
			slot->IB = &container->vecSubset[i].IB;
			container->vecSubset[i].slot = slot;
		}
	}
	// 재질 정보를 읽어온다.
	auto    iterM = vecMaterials->begin();
	auto    iterMEnd = vecMaterials->end();
	containerIndex = 0;
	for (; iterM != iterMEnd; ++iterM, ++containerIndex)
	{
		// 서브셋 수만큼 반복한다.
		size_t  size = (*iterM).size();
		for (size_t i = 0; i < size; ++i)
		{
			if (!vecEmptyIndex[containerIndex][i])
			{
				continue;
			}
			FbxMaterial* mtrl = (*iterM)[i];
			CMaterial* material = new CMaterial;
			material->SetName(mtrl->Name);
			material->SetBaseColor(mtrl->BaseColor);
			material->SetAmbientColor(mtrl->AmbientColor);
			material->SetSpecularColor(mtrl->SpecularColor);
			material->SetEmissiveColor(mtrl->EmissiveColor);
			material->SetSpecularPower(mtrl->Shininess);
			material->SetShader("MeshShader");
			SetMaterial(containerIndex, (int)i, material);
			// Texture
			char    fileName[MAX_PATH] = {};
			char    ext[MAX_PATH] = {};
			_splitpath_s(mtrl->DiffuseTexture.c_str(), 0, 0, 0, 0, fileName, MAX_PATH, ext, MAX_PATH);
			TCHAR   fullPath[MAX_PATH] = {};
#ifdef UNICODE
			int pathLength = MultiByteToWideChar(CP_ACP, 0, mtrl->DiffuseTexture.c_str(), -1, 0, 0);
			MultiByteToWideChar(CP_ACP, 0, mtrl->DiffuseTexture.c_str(), -1, fullPath, pathLength);
#else
			strcpy_s(fullPath, mtrl->DiffuseTexture.c_str());
#endif // UNICODE
			if (ext[0] == '\0')
			{
				material->AddTexture(0, (int)EShaderBufferType::Pixel, "none", TEXT("none.png"));
			}
			else
			{
				material->AddTextureFullPath(0, (int)EShaderBufferType::Pixel, fileName, fullPath);
			}
			if (!mtrl->BumpTexture.empty())
			{
				material->EnableBump();
				memset(fileName, 0, MAX_PATH);
				_splitpath_s(mtrl->BumpTexture.c_str(), 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);
				memset(fullPath, 0, sizeof(TCHAR) * MAX_PATH);
#ifdef UNICODE
				int pathLength = MultiByteToWideChar(CP_ACP, 0, mtrl->BumpTexture.c_str(), -1, 0, 0);
				MultiByteToWideChar(CP_ACP, 0, mtrl->BumpTexture.c_str(), -1, fullPath, pathLength);
#else
				strcpy_s(fullPath, mtrl->BumpTexture.c_str());
#endif // UNICODE
				material->AddTextureFullPath(1, (int)EShaderBufferType::Pixel, fileName, fullPath);
			}
			if (!mtrl->SpecularTexture.empty())
			{
				memset(fileName, 0, MAX_PATH);
				_splitpath_s(mtrl->SpecularTexture.c_str(), 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);
				memset(fullPath, 0, sizeof(TCHAR) * MAX_PATH);
#ifdef UNICODE
				int pathLength = MultiByteToWideChar(CP_ACP, 0, mtrl->SpecularTexture.c_str(), -1, 0, 0);
				MultiByteToWideChar(CP_ACP, 0, mtrl->SpecularTexture.c_str(), -1, fullPath, pathLength);
#else
				strcpy_s(fullPath, mtrl->SpecularTexture.c_str());
#endif // UNICODE
				material->AddTextureFullPath(2, (int)EShaderBufferType::Pixel, fileName, fullPath);
				material->EnableSpecular();
			}
			if (animationEnable)
			{
				material->EnableAnimation3D();
			}
		}
	}
	// 자체포맷으로 저장해준다.
	char    meshFullPath[MAX_PATH] = {};
	strcpy_s(meshFullPath, fullPath);
	int pathLength = (int)strlen(fullPath);
	memcpy(&meshFullPath[pathLength - 3], "msh", 3);
	SaveMeshFile(meshFullPath);
	return true;
}

bool CMesh::SaveMeshFile(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return false;
	}
	SaveMesh(file);
	fclose(file);
	return true;
}

bool CMesh::LoadMeshFile(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	LoadMesh(file);
	fclose(file);
	return true;
}

bool CMesh::SaveMesh(FILE* file)
{
	CRef::Save(file);
	fwrite(&m_MeshType, sizeof(MeshType), 1, file);
	fwrite(&m_Min, sizeof(Vector3), 1, file);
	fwrite(&m_Max, sizeof(Vector3), 1, file);
	int	containerCount = (int)m_vecContainer.size();
	fwrite(&containerCount, sizeof(int), 1, file);
	for (int i = 0; i < containerCount; ++i)
	{
		MeshContainer* container = m_vecContainer[i];
		fwrite(&container->primitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, file);
		fwrite(&container->VB.size, sizeof(int), 1, file);
		fwrite(&container->VB.count, sizeof(int), 1, file);
		fwrite(container->VB.data, container->VB.size, container->VB.count, file);
		int	subsetCount = (int)container->vecSubset.size();
		fwrite(&subsetCount, sizeof(int), 1, file);
		for (int j = 0; j < subsetCount; ++j)
		{
			fwrite(&container->vecSubset[j].IB.size, sizeof(int), 1, file);
			fwrite(&container->vecSubset[j].IB.count, sizeof(int), 1, file);
			fwrite(&container->vecSubset[j].IB.fmt, sizeof(DXGI_FORMAT), 1, file);
			fwrite(container->vecSubset[j].IB.data, container->vecSubset[j].IB.size, container->vecSubset[j].IB.count, file);
			container->vecSubset[j].material->Save(file);
		}
	}
	return true;
}

bool CMesh::LoadMesh(FILE* file)
{
	size_t	size = m_vecContainer.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
	size = m_vecMeshSlot.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecMeshSlot[i]);
	}
	CRef::Load(file);
	fread(&m_MeshType, sizeof(MeshType), 1, file);
	fread(&m_Min, sizeof(Vector3), 1, file);
	fread(&m_Max, sizeof(Vector3), 1, file);
	int	containerCount = 0;
	fread(&containerCount, sizeof(int), 1, file);
	for (int i = 0; i < containerCount; ++i)
	{
		MeshContainer* container = new MeshContainer;
		m_vecContainer.push_back(container);
		fread(&container->primitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, file);
		fread(&container->VB.size, sizeof(int), 1, file);
		fread(&container->VB.count, sizeof(int), 1, file);
		container->VB.data = new char[container->VB.size * container->VB.count];
		assert(container->VB.count >= 0);
		assert(container->VB.count >= 0);
		fread(container->VB.data, container->VB.size, container->VB.count, file);
		CreateBuffer(BufferType::Vertex, container->VB.data, container->VB.size, container->VB.count,
			D3D11_USAGE_DEFAULT, &container->VB.buffer);
		int	subsetCount = 0;
		fread(&subsetCount, sizeof(int), 1, file);
		container->vecSubset.reserve(subsetCount);
		for (int j = 0; j < subsetCount; ++j)
		{
			MeshSlot* slot = new MeshSlot;
			m_vecMeshSlot.push_back(slot);
			slot->VB = &container->VB;
			slot->primitive = container->primitive;
			MeshSubset	subset = {};
			container->vecSubset.emplace_back(subset);
			container->vecSubset[j].slot = slot;
			fread(&container->vecSubset[j].IB.size, sizeof(int), 1, file);
			fread(&container->vecSubset[j].IB.count, sizeof(int), 1, file);
			fread(&container->vecSubset[j].IB.fmt, sizeof(DXGI_FORMAT), 1, file);
			container->vecSubset[j].IB.data = new char[container->vecSubset[j].IB.size * container->vecSubset[j].IB.count];
			fread(container->vecSubset[j].IB.data, container->vecSubset[j].IB.size, container->vecSubset[j].IB.count, file);
			if (!CreateBuffer(BufferType::Index, container->vecSubset[j].IB.data, container->vecSubset[j].IB.size,
				container->vecSubset[j].IB.count, D3D11_USAGE_DEFAULT, &container->vecSubset[j].IB.buffer))
			{
				return false;
			}
			slot->IB = &container->vecSubset[j].IB;
			CMaterial* material = new CMaterial;
			material->Load(file);
			if (material)
			{
				material->SetEmissiveColor(0.1f, 0.1f, 0.1f, 0.f);
				SetMaterial(i, j, material);
			}
		}
	}
	return true;
}
