#include "TileMapComponent.h"
#include "CameraComponent.h"
#include "../Device.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Resource/Shader/TileMapConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Thread/ThreadManager.h"

CTileMapComponent::CTileMapComponent()
	: m_CountX(0)
	, m_CountY(0)
	, m_Count(0)
	, m_RenderCount(0)
	, m_TileMapCBuffer(nullptr)
	, m_TileInfoBuffer(nullptr)
	, m_EditorMouseOnTile(nullptr)
	, m_TileInfoCount(0)
{
	SetTypeID<CTileMapComponent>();
	m_ComponentTypeName = "TileMapComponent";
	m_TileTypeColor[(int)ETileOption::None] = Vector4::white;
	m_TileTypeColor[(int)ETileOption::Wall] = Vector4::red;
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& component)
	: CPrimitiveComponent(component)
{
	m_TileMesh = component.m_TileMesh;
	m_TileInfoCount = component.m_TileInfoCount;
	if (component.m_TileMaterial)
	{
		m_TileMaterial = component.m_TileMaterial->Clone();
	}
	if (component.m_TileMapCBuffer)
	{
		m_TileMapCBuffer = component.m_TileMapCBuffer->Clone();
	}
	if (component.m_TileInfoBuffer)
	{
		m_TileInfoBuffer = component.m_TileInfoBuffer->Clone();
	}
	m_Shape = component.m_Shape;
	m_CountX = component.m_CountX;
	m_CountY = component.m_CountY;
	m_Count = component.m_Count;
	m_RenderCount = component.m_RenderCount;
	m_TileSize = component.m_TileSize;
	m_TileTypeColor[(int)ETileOption::None] = component.m_TileTypeColor[(int)ETileOption::None];
	m_TileTypeColor[(int)ETileOption::Wall] = component.m_TileTypeColor[(int)ETileOption::Wall];
	m_vecTile.clear();
	size_t	size = component.m_vecTile.size();
	for (size_t i = 0; i < size; ++i)
	{
		CTile* tile = component.m_vecTile[i]->Clone();
		tile->m_Owner = this;
		m_vecTile.push_back(tile);
	}
	m_vecTileInfo.resize(m_vecTile.size());
}

CTileMapComponent::~CTileMapComponent()
{
	CThreadManager::GetInst()->DeleteNavigationThread(this);
	SAFE_DELETE(m_TileMapCBuffer);
	SAFE_DELETE(m_TileInfoBuffer);
	auto	iter = m_vecTile.begin();
	auto	iterEnd = m_vecTile.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	m_vecTile.clear();
}

void CTileMapComponent::SetEditorMouseOnTile(int index)
{
	if (index < 0 || index >= m_Count)
	{
		m_EditorMouseOnTile = nullptr;
	}
	else
	{
		m_EditorMouseOnTile = m_vecTile[index];
	}
}

void CTileMapComponent::SetTileMaterial(const std::string& name)
{
	if (m_Scene)
	{
		m_TileMaterial = m_Scene->GetResource()->FindMaterial(name);
	}
	else
	{
		m_TileMaterial = CResourceManager::GetInst()->FindMaterial(name);
	}
	m_TileMapCBuffer->SetImageSize(Vector2((float)m_TileMaterial->GetTexture(0)->GetWidth(), 
										   (float)m_TileMaterial->GetTexture(0)->GetHeight()));
}

void CTileMapComponent::SetTileMaterial(CMaterial* material)
{
	m_TileMaterial = material;
}

void CTileMapComponent::SetTileTexture(CTexture* texture)
{
	m_TileMaterial->SetTexture(0, 0, (int)EShaderBufferType::Pixel, texture->GetName(), texture);
}

void CTileMapComponent::SetTileTexture(const std::string& name, const TCHAR* fileName, 
	const std::string& pathName)
{
	m_TileMaterial->SetTexture(0, 0, (int)EShaderBufferType::Pixel, name, fileName, pathName);
}

void CTileMapComponent::SetTileTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	m_TileMaterial->SetTextureFullPath(0, 0, (int)EShaderBufferType::Pixel, name, fullPath);
}

void CTileMapComponent::SetTileTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	m_TileMaterial->SetTextureArray(0, 0, (int)EShaderBufferType::Pixel, name, vecFileName, pathName);
}

void CTileMapComponent::SetTileTextureArrayFullPath(const std::string& name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	m_TileMaterial->SetTextureArrayFullPath(0, 0, (int)EShaderBufferType::Pixel, name, vecFullPath);
}

void CTileMapComponent::SetTileBackTexture(CTexture* texture)
{
	m_TileBackTexture = texture;
	if (m_TileBackTexture)
	{
		if (m_TileBackTexture->GetImageType() != EImageType::Array)
		{
			m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel, texture->GetName(), texture);
		}
		else
		{
			m_vecMaterial[0]->SetTexture(10, 0, (int)EShaderBufferType::Pixel, texture->GetName(), texture);
		}
	}
}

void CTileMapComponent::SetTileBackTexture(const std::string& name, const TCHAR* fileName, 
	const std::string& pathName)
{
	CTexture* texture = nullptr;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(name, fileName, pathName))
		{
			return;
		}
		texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		{
			return;
		}
		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_TileBackTexture = texture;
	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel, texture->GetName(), texture);
	}
}

void CTileMapComponent::SetTileBackTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CTexture* texture = nullptr;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(name, fullPath))
		{
			return;
		}
		texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
		{
			return;
		}
		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_TileBackTexture = texture;
	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetTexture(0, 0, (int)EShaderBufferType::Pixel, texture->GetName(), texture);
	}
}

void CTileMapComponent::SetTileBackTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	CTexture* texture = nullptr;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArray(name, vecFileName, pathName))
		{
			return;
		}
		texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArray(name, vecFileName, pathName))
		{
			return;
		}
		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_TileBackTexture = texture;
	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetTexture(0, 10, (int)EShaderBufferType::Pixel, texture->GetName(), texture);
	}
}

void CTileMapComponent::SetTileBackTextureArrayFullPath(const std::string& name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* texture = nullptr;
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArrayFullPath(name, vecFullPath))
		{
			return;
		}
		texture = m_Scene->GetResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(name, vecFullPath))
		{
			return;
		}
		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	m_TileBackTexture = texture;
	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetTexture(0, 10, (int)EShaderBufferType::Pixel, texture->GetName(), texture);
	}
}

void CTileMapComponent::SetTileBaseColor(const Vector4& color)
{
	m_TileMaterial->SetBaseColor(color);
}

void CTileMapComponent::CreateTile(ETileShape shape, int countX, int countY, const Vector2& tileSize)
{
	m_Shape = shape;
	m_CountX = countX;
	m_CountY = countY;
	m_Count = m_CountX * m_CountY;
	m_TileSize = tileSize;
	auto	iter = m_vecTile.begin();
	auto	iterEnd = m_vecTile.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	m_vecTile.clear();
	m_vecTile.resize((size_t)(m_CountX * m_CountY));
	for (int i = 0; i < m_CountY; ++i)
	{
		for (int j = 0; j < m_CountX; ++j)
		{
			CTile* tile = new CTile;
			tile->m_Owner = this;
			int	index = i * m_CountX + j;
			tile->m_IndexX = j;
			tile->m_IndexY = i;
			tile->m_Index = index;
			tile->m_Size.x = m_TileSize.x;
			tile->m_Size.y = m_TileSize.y;
			tile->m_TileStart = m_TileStartFrame;
			tile->m_TileEnd = m_TileEndFrame;
			tile->m_Frame = 3;
			m_vecTile[index] = tile;
		}
	}
	int	renderCountX = 0;
	int renderCountY = 0;
	switch (m_Shape)
	{
	case ETileShape::Rect:
		renderCountX = (int)(CDevice::GetInst()->GetResolution().width / m_TileSize.x) + 2;
		renderCountY = (int)(CDevice::GetInst()->GetResolution().height / m_TileSize.y) + 2;
		for (int i = 0; i < m_CountY; ++i)
		{
			for (int j = 0; j < m_CountX; ++j)
			{
				int	index = i * m_CountX + j;
				m_vecTile[index]->m_Pos.x = j * m_TileSize.x;
				m_vecTile[index]->m_Pos.y = i * m_TileSize.y;
			}
		}
		break;
	case ETileShape::Isometric:
	{
		renderCountX = (int)(CDevice::GetInst()->GetResolution().width / m_TileSize.x) + 3;
		renderCountY = (int)(CDevice::GetInst()->GetResolution().height / m_TileSize.y) * 2 + 6;
		float	startX = 0.f;
		for (int i = 0; i < m_CountY; ++i)
		{
			if (i % 2 == 0)
			{
				startX = 0.f;
			}
			else
			{
				startX = m_TileSize.x * 0.5f;
			}
			for (int j = 0; j < m_CountX; ++j)
			{
				int	index = i * m_CountX + j;
				m_vecTile[index]->m_Pos.x = startX + j * m_TileSize.x;
				m_vecTile[index]->m_Pos.y = i * m_TileSize.y * 0.5f;
			}
		}
	}
		break;
	}
	m_TileMapCBuffer->SetTileSize(m_TileSize);
	SAFE_DELETE(m_TileInfoBuffer);
	m_TileInfoCount = renderCountX * renderCountY;
	m_TileInfoBuffer = new CStructuredBuffer;
	m_TileInfoBuffer->Init("TileInfo", sizeof(TileInfo), m_TileInfoCount, 40, true, (int)EShaderBufferType::Vertex);
	m_vecTileInfo.resize((size_t)m_Count);
	for (int i = 0; i < m_Count; ++i)
	{
		m_vecTileInfo[i].typeColor = Vector4(1.f, 1.f, 1.f, 1.f);
		m_vecTileInfo[i].opacity = 1.f;
	}
	CThreadManager::GetInst()->CreateNavigationThread(this);
}

void CTileMapComponent::ChangeTileFrame(const Vector2& pos, int frame)
{
	CTile* tile = GetTile(pos);
	if (!tile)
	{
		return;
	}
	tile->m_TileStart = m_vecTileFrame[frame].start;
	tile->m_TileEnd = m_vecTileFrame[frame].end;
	tile->m_Frame = frame;
}

void CTileMapComponent::ChangeTileOption(const Vector2& pos, ETileOption option)
{
	CTile* tile = GetTile(pos);
	if (!tile)
	{
		return;
	}
	tile->m_TileOption = option;
}

void CTileMapComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CTileMapComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}
	m_Transform->Set2D(true);
	SetMesh("LBUVRect");	// 배경용 메쉬 등록(타일 안쓸때는 배경용)
	m_TileMesh = m_Mesh;
	m_Shape = ETileShape::Rect;
	m_TileMapCBuffer = new CTileMapConstantBuffer;
	m_TileMapCBuffer->Init();
	m_TileMapCBuffer->SetStart(Vector2(0.f, 0.f));
	m_TileMapCBuffer->SetEnd(Vector2(160.f, 80.f));
	m_TileMapCBuffer->SetFrame(0);
	SetTileMaterial("TileMap");
	for (int i = 0; i <= 379; ++i)
	{
		Animation2DFrameData	data;
		data.start = Vector2(0.f, 0.f);
		data.end = Vector2(160.f, 80.f);
		m_vecTileFrame.push_back(data);
	}
	m_TileStartFrame = Vector2(0.f, 0.f);
	m_TileEndFrame = Vector2(160.f, 80.f);
	m_vecMaterial.clear();
	AddMaterial("DefaultTileMapBack");
	return true;
}

void CTileMapComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
	size_t	size = m_vecTile.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecTile[i]->Update(deltaTime);
	}
}

void CTileMapComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
	if (!m_vecTile.empty())
	{
		CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();
		Resolution	RS = CDevice::GetInst()->GetResolution();
		Vector3	LB = camera->GetWorldPos();
		Vector3	RT = LB + Vector3((float)RS.width, (float)RS.height, 0.f);
		int startX = GetTileRenderIndexX(LB);
		int startY = GetTileRenderIndexY(LB);
		int endX = GetTileRenderIndexX(RT);
		int endY = GetTileRenderIndexY(RT);
		if (m_Shape == ETileShape::Isometric)
		{
			--startX;
			startY -= 2;
			++endX;
			endY+=2;
			startX = startX < 0 ? 0 : startX;
			startY = startY < 0 ? 0 : startY;
			endX = endX >= m_CountX ? m_CountX - 1 : endX;
			endY = endY >= m_CountY ? m_CountY - 1 : endY;
		}
		Matrix matView = camera->GetViewMatrix();
		Matrix matProj = camera->GetProjMatrix();
		m_RenderCount = 0;
		for (int i = startY; i <= endY; ++i)
		{
			for (int j = startX; j <= endX; ++j)
			{
				int	index = i * m_CountX + j;
				m_vecTile[index]->UpdateTransform(deltaTime);
				m_vecTileInfo[m_RenderCount].matWVP = m_vecTile[index]->m_matWorld* matView * matProj;
				m_vecTileInfo[m_RenderCount].matWVP.Transpose();
				if (!m_vecTile[index]->FrameEmpty())
				{
					m_vecTileInfo[m_RenderCount].start = m_vecTile[index]->GetFrameData().start;
					m_vecTileInfo[m_RenderCount].end = m_vecTile[index]->GetFrameData().end;
				}
				else
				{
					m_vecTileInfo[m_RenderCount].start = m_vecTile[index]->m_TileStart;
					m_vecTileInfo[m_RenderCount].end = m_vecTile[index]->m_TileEnd;
				}
				m_vecTileInfo[m_RenderCount].opacity = m_vecTile[index]->m_Opacity;
				m_vecTileInfo[m_RenderCount].animationType = (int)m_vecTile[index]->m_Anim2DType;
				m_vecTileInfo[m_RenderCount].frame = m_vecTile[index]->m_Frame;
				m_vecTileInfo[m_RenderCount].typeColor = m_TileTypeColor[(int)m_vecTile[index]->m_TileOption];
				if (m_EditorMouseOnTile == m_vecTile[index])
				{
					m_vecTileInfo[m_RenderCount].typeColor = Vector4(0.f, 1.f, 0.f, 1.f);
				}
				++m_RenderCount;
			}
		}
		m_TileInfoBuffer->UpdateBuffer(&m_vecTileInfo[0],m_RenderCount);
	}
}

void CTileMapComponent::Render()
{
	CSceneComponent::Render();
	if (m_TileBackTexture)
	{
		m_vecMaterial[0]->SetMaterial();
		m_Mesh->Render();
		m_vecMaterial[0]->ResetMaterial();
	}
	if (m_RenderCount > 0 && m_TileMaterial)
	{
		m_TileInfoBuffer->SetShader();		// 구조화버퍼의 내용을 Shader로 보내준다.
		m_TileMapCBuffer->UpdateBuffer();		// TileMap 상수버퍼를 Shader로 보내준다.
		m_TileMaterial->SetMaterial();		// 재질 지정
		m_TileMesh->RenderInstancing(m_RenderCount);
		m_TileMaterial->ResetMaterial();
		m_TileInfoBuffer->ResetShader();
	}
}

CTileMapComponent* CTileMapComponent::Clone() const
{
	return new CTileMapComponent(*this);
}

void CTileMapComponent::Save(FILE* file)
{
	CPrimitiveComponent::Save(file);
	int	count = (int)m_vecTile.size();
	fwrite(&count, sizeof(int), 1, file);
	for (int i = 0; i < count; ++i)
	{
		m_vecTile[i]->Save(file);
	}
	bool	tileMaterial = false;
	if (m_TileMaterial)
	{
		tileMaterial = true;
	}
	fwrite(&tileMaterial, sizeof(bool), 1, file);
	if (tileMaterial)
	{
		m_TileMaterial->Save(file);
	}
	bool backTexture = false;
	if (m_TileBackTexture)
	{
		backTexture = true;
	}
	fwrite(&backTexture, sizeof(bool), 1, file);
	if (backTexture)
	{
		int	length = (int)m_TileBackTexture->GetName().length();
		fwrite(&length, sizeof(int), 1, file);
		fwrite(m_TileBackTexture->GetName().c_str(), sizeof(char), length, file);
	}
	fwrite(&m_Shape, sizeof(ETileShape), 1, file);
	fwrite(&m_CountX, sizeof(int), 1, file);
	fwrite(&m_CountY, sizeof(int), 1, file);
	fwrite(&m_Count, sizeof(int), 1, file);
	fwrite(&m_TileInfoCount, sizeof(int), 1, file);
	fwrite(&m_TileSize, sizeof(Vector2), 1, file);
	fwrite(&m_TileStartFrame, sizeof(Vector2), 1, file);
	fwrite(&m_TileEndFrame, sizeof(Vector2), 1, file);
	int	frameCount = (int)m_vecTileFrame.size();
	fwrite(&frameCount, sizeof(int), 1, file);
	for (int i = 0; i < frameCount; ++i)
	{
		fwrite(&m_vecTileFrame[i], sizeof(Animation2DFrameData), 1, file);
	}
}

void CTileMapComponent::Load(FILE* file)
{
	CPrimitiveComponent::Load(file);
	int	count = 0;
	fread(&count, sizeof(int), 1, file);
	size_t	size = m_vecTile.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecTile[i]);
	}
	m_vecTile.clear();
	for (int i = 0; i < count; ++i)
	{
		CTile* tile = new CTile;
		tile->m_Owner = this;
		tile->Load(file);
		m_vecTile.push_back(tile);
	}
	bool	tileMaterial = false;
	fread(&tileMaterial, sizeof(bool), 1, file);
	if (tileMaterial)
	{
		CMaterial* material = m_Mesh->GetMaterial(0);
		m_TileMaterial = material->Clone();
		m_TileMaterial->SetScene(m_Scene);
		m_TileMaterial->Load(file);
	}
	bool backTexture = false;
	fread(&backTexture, sizeof(bool), 1, file);
	if (backTexture)
	{
		int	length = 0;
		char	texName[256] = {};
		fread(&length, sizeof(int), 1, file);
		assert(length >= 0);
		fread(texName, sizeof(char), length, file);
		if (m_Scene)
		{
			m_TileBackTexture = m_Scene->GetResource()->FindTexture(texName);
		}
		else
		{
			m_TileBackTexture = CResourceManager::GetInst()->FindTexture(texName);
		}
	}
	fread(&m_Shape, sizeof(ETileShape), 1, file);
	fread(&m_CountX, sizeof(int), 1, file);
	fread(&m_CountY, sizeof(int), 1, file);
	fread(&m_Count, sizeof(int), 1, file);
	fread(&m_TileInfoCount, sizeof(int), 1, file);
	fread(&m_TileSize, sizeof(Vector2), 1, file);
	fread(&m_TileStartFrame, sizeof(Vector2), 1, file);
	fread(&m_TileEndFrame, sizeof(Vector2), 1, file);
	int	frameCount = 0;
	fread(&frameCount, sizeof(int), 1, file);
	if (frameCount > 0)
	{
		m_vecTileFrame.resize((size_t)frameCount);
	}
	for (int i = 0; i < frameCount; ++i)
	{
		fread(&m_vecTileFrame[i], sizeof(Animation2DFrameData), 1, file);
	}
	m_TileMapCBuffer->SetStart(m_TileStartFrame);
	m_TileMapCBuffer->SetEnd(m_TileEndFrame);
	m_TileMapCBuffer->SetFrame(0);
	SAFE_DELETE(m_TileInfoBuffer);
	m_TileInfoBuffer = new CStructuredBuffer;
	m_TileInfoBuffer->Init("TileInfo", sizeof(TileInfo), m_TileInfoCount, 40, true,	(int)EShaderBufferType::Vertex);
	m_vecTileInfo.resize((size_t)m_Count);
	for (int i = 0; i < m_Count; ++i)
	{
		m_vecTileInfo[i].typeColor = Vector4(1.f, 1.f, 1.f, 1.f);
		m_vecTileInfo[i].opacity = 1.f;
	}
	if(m_Scene)
	{
		m_SceneName = m_Scene->GetName();
		CThreadManager::GetInst()->CreateNavigationThread(this);
	}
}

int CTileMapComponent::GetTileIndexX(const Vector2& pos)
{
	if (m_Shape == ETileShape::Rect)
	{
		float convertX = pos.x - GetWorldPos().x;
		int	indexX = (int)(convertX / m_TileSize.x);
		if (indexX < 0 || indexX >= m_CountX)
		{
			return -1;
		}
		return indexX;
	}
	//마름모
	int	indexY = GetTileIndexY(pos);
	if (indexY < 0 || indexY >= m_CountY)
	{
		return -1;
	}
	int	indexX = -1;
	float	convertX = pos.x - GetWorldPos().x;
	if (indexY % 2 == 0)
	{
		indexX = (int)(convertX / m_TileSize.x);
	}
	else
	{
		indexX = (int)((convertX - m_TileSize.x * 0.5f) / m_TileSize.x);
	}
	if (indexX < 0 || indexX >= m_CountX)
	{
		return -1;
	}
	return indexX;
}

int CTileMapComponent::GetTileIndexX(const Vector3& pos)
{
	return GetTileIndexX(Vector2(pos.x, pos.y));
}

int CTileMapComponent::GetTileIndexY(const Vector2& pos)
{
	if (m_Shape == ETileShape::Rect)
	{
		float convertY = pos.y - GetWorldPos().y;
		int	indexY = (int)(convertY / m_TileSize.y);
		if (indexY < 0 || indexY >= m_CountY)
		{
			return -1;
		}
		return indexY;
	}
	Vector2	tileMapPos(GetWorldPos().x, GetWorldPos().y);
	Vector2	convertPos = pos - tileMapPos;
	float ratioX = convertPos.x / m_TileSize.x;
	float ratioY = convertPos.y / m_TileSize.y;
	int	indexX = (int)ratioX;
	int	indexY = (int)ratioY;
	if (indexX < 0 || indexX > m_CountX)
	{
		return -1;
	}
	// 정수 부분을 제거하여 소수점 부분만 남겨준다.
	// 소수점 부분을 이용하여 사각형의 어느 지점에 있는지를 판단하기 위해서이다.
	ratioX -= (int)ratioX;
	ratioY -= (int)ratioY;
	// 사각형의 아래쪽 부분일 경우
	if (ratioY < 0.5f)
	{
		// 좌 하단 사각형의 좌 하단 삼각형일 경우가 된다.
		// RatioX가 0.5보다 크면 오른쪽 하단 사각형이 되는데
		// 이경우 0.5에서 빼면 음수가 나오므로 RatioY는 절대 이 값보다 작아질 수 없다.
		// 이 경우 우측 상단의 삼각형 안에 존재하는 점은 RatioY가 절대 이 값보다 작을 수 없다.
		// 좌측 하단의 삼각형을 구하는 식이다.
		if (ratioY < 0.5f - ratioX)
		{
			// Y인덱스가 0이면 가장 아래쪽 줄이라서 좌 하단 삼각형은 없다.
			if (indexY == 0)
			{
				return -1;
			}
			else if (indexX == 0)
			{
				return -1;
			}
			else
			{
				return indexY * 2 - 1;
			}
		}
		// 오른쪽 사각형 안에서 우 하단 삼각형일 경우
		else if (ratioY < ratioX - 0.5f)
		{
			if (indexY == 0)
			{
				return -1;
			}
			else
			{
				return indexY * 2 - 1;
			}
		}
		else
		{
			return indexY * 2;
		}
	}
	// 사각형의 위쪽 부분일 경우
	else if (ratioY > 0.5f)
	{
		// 좌 상단 사각형의 좌 상단 삼각형일 경우
		if (ratioY - 0.5f > ratioX)
		{
			if (indexX == 0)
			{
				return -1;
			}
			else if (indexY * 2 + 1 >= m_CountY)
			{
				return -1;
			}
			else
			{
				return indexY * 2 + 1;
			}
		}
		// 우 상단 사각형의 우 상단 삼각형일 경우
		else if (ratioY - 0.5f > 1.f - ratioX)
		{
			if (indexX >= m_CountX)
			{
				return -1;
			}
			else if (indexY * 2 + 1 >= m_CountY)
			{
				return -1;
			}
			else
			{
				return indexY * 2 + 1;
			}
		}
		else
		{
			return indexY * 2;
		}
	}
	// 가운데
	else
	{
		return indexY * 2;
	}
	return -1;
}

int CTileMapComponent::GetTileIndexY(const Vector3& pos)
{
	return GetTileIndexY(Vector2(pos.x, pos.y));
}

int CTileMapComponent::GetTileIndex(const Vector2& pos)
{
	int	indexX = GetTileIndexX(pos);
	int	indexY = GetTileIndexY(pos);
	if (indexX == -1 || indexY == -1)
	{
		return -1;
	}
	return indexY * m_CountX + indexX;
}

int CTileMapComponent::GetTileIndex(const Vector3& pos)
{
	return GetTileIndex(Vector2(pos.x, pos.y));
}

CTile* CTileMapComponent::GetTile(const Vector2& pos)
{
	int	index = GetTileIndex(pos);
	if (index == -1)
	{
		return nullptr;
	}
	return m_vecTile[index];
}

CTile* CTileMapComponent::GetTile(const Vector3& pos)
{
	int	index = GetTileIndex(pos);
	if (index == -1)
	{
		return nullptr;
	}
	return m_vecTile[index];
}

CTile* CTileMapComponent::GetTile(int x, int y)
{
	if (x < 0 || x >= m_CountX || y < 0 || y >= m_CountY)
	{
		return nullptr;
	}
	return m_vecTile[y * m_CountX + x];
}

CTile* CTileMapComponent::GetTile(int index)
{
	if (index < 0 || index >= m_Count)
	{
		return nullptr;
	}
	return m_vecTile[index];
}

int CTileMapComponent::GetTileRenderIndexX(const Vector3& pos)
{
	if (m_Shape == ETileShape::Rect)
	{
		float convertX = pos.x - GetWorldPos().x;
		int	indexX = (int)(convertX / m_TileSize.x);
		if (indexX < 0)
		{
			return 0;
		}
		else if (indexX >= m_CountX)
		{
			return m_CountX - 1;
		}
		return indexX;
	}
	int	indexY = GetTileRenderIndexY(pos);
	if (indexY < 0)
	{
		indexY = 0;
	}
	else if (indexY >= m_CountY)
	{
		indexY = m_CountY - 1;
	}
	int	indexX = -1;
	float	convertX = pos.x - GetWorldPos().x;
	if (indexY % 2 == 0)
	{
		indexX = (int)(convertX / m_TileSize.x);
	}
	else
	{
		indexX = (int)((convertX - m_TileSize.x * 0.5f) / m_TileSize.x);
	}
	if (indexX < 0)
	{
		return 0;
	}
	else if (indexX >= m_CountX)
	{
		return m_CountX - 1;
	}
	return indexX;
}

int CTileMapComponent::GetTileRenderIndexY(const Vector3& pos)
{
	if (m_Shape == ETileShape::Rect)
	{
		float convertY = pos.y - GetWorldPos().y;
		int	indexY = (int)(convertY / m_TileSize.y);
		if (indexY < 0)
		{
			return 0;
		}
		else if (indexY >= m_CountY)
		{
			return m_CountY - 1;
		}
		return indexY;
	}
	Vector2	tileMapPos(GetWorldPos().x, GetWorldPos().y);
	Vector2	convertPos = Vector2(pos.x, pos.y) - tileMapPos;
	float ratioX = convertPos.x / m_TileSize.x;
	float ratioY = convertPos.y / m_TileSize.y;
	int	indexX = (int)ratioX;
	int	indexY = (int)ratioY;
	if (indexX < 0)
	{
		indexX = 0;
	}
	else if (indexX > m_CountX)
	{
		indexX = m_CountX - 1;
	}
	ratioX -= (int)ratioX;
	ratioY -= (int)ratioY;
	if (ratioY < 0.5f)
	{
		if (ratioY < 0.5f - ratioX)
		{
			if (indexY == 0)
			{
				return 0;
			}
			else if (indexX == 0)
			{
				if (indexY < 0)
				{
					return 0;
				}
				else if (indexY >= m_CountY)
				{
					return m_CountY - 1;
				}
				else
				{
					return indexY * 2 + 1;
				}
			}
			else
			{
				return indexY * 2 - 1;
			}
		}
		else if (ratioY < ratioX - 0.5f)
		{
			if (indexY == 0)
			{
				return 0;
			}
			else
			{
				return indexY * 2 - 1;
			}
		}
		else
		{
			return indexY * 2;
		}
	}
	else if (ratioY > 0.5f)
	{
		if (ratioY - 0.5f > ratioX)
		{
			if (indexX == 0)
			{
				if (indexY < 0)
				{
					return 0;
				}
				else if (indexY >= m_CountY)
				{
					return m_CountY - 1;
				}
			}
			else if (indexY * 2 + 1 >= m_CountY)
			{
				return m_CountY - 1;
			}
			else
			{
				return indexY * 2 + 1;
			}
		}
		else if (ratioY - 0.5f > 1.f - ratioX)
		{
			if (indexX >= m_CountX)
			{
				if (indexY < 0)
				{
					return 0;
				}
				else if (indexY >= m_CountY)
				{
					return m_CountY - 1;
				}
			}
			else if (indexY * 2 + 1 >= m_CountY)
			{
				return  m_CountY - 1;
			}
			else
			{
				return indexY * 2 + 1;
			}
		}
		else
		{
			return indexY * 2;
		}
	}
	else
	{
		return indexY * 2;
	}
	return -1;
}