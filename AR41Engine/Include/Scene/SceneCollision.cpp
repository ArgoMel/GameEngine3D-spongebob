#include "SceneCollision.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneViewport.h"
#include "CollisionSection2D.h"
#include "CollisionSection3D.h"
#include "../CollisionManager.h"
#include "../Device.h"
#include "../PathManager.h"
#include "../Input.h"
#include "../Component/Collider2D.h"
#include "../Component/Collider3D.h"
#include "../Component/CameraComponent.h"

CSceneCollision::CSceneCollision()
	: m_Owner(nullptr)
	, m_CollisionWidget(false)
{
}

CSceneCollision::~CSceneCollision()
{
	size_t	count = m_Section2D.vecSection.size();
	for (size_t i = 0; i < count; ++i)
	{
		SAFE_DELETE(m_Section2D.vecSection[i]);
	}
	count = m_Section3D.vecSection.size();
	for (size_t i = 0; i < count; ++i)
	{
		SAFE_DELETE(m_Section3D.vecSection[i]);
	}
	auto	iter = m_mapPixelCollision.begin();
	auto	iterEnd = m_mapPixelCollision.end();
	for (; iter != iterEnd; ++iter)
	{
		--iter->second->refCount;
		if (iter->second->refCount == 0)
		{
			SAFE_RELEASE(iter->second->SRV);
			SAFE_DELETE_ARRAY(iter->second->pixel);
			SAFE_DELETE(iter->second);
		}
	}
	m_mapPixelCollision.clear();
}

bool CSceneCollision::SortPickginSection(int src, int dest)
{
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
	Vector3	cameraPos = camera->GetWorldPos();
	CSceneCollision* collision = CSceneManager::GetInst()->GetScene()->GetCollisionManager();
	Vector3	srcCenter = collision->m_Section3D.vecSection[src]->m_Center;
	Vector3	destCenter = collision->m_Section3D.vecSection[dest]->m_Center;
	float	srcDist = cameraPos.Distance(srcCenter);
	float	destDist = cameraPos.Distance(destCenter);
	return srcDist < destDist;
}

bool CSceneCollision::SortColliderList(CSharedPtr<class CCollider3D> src, CSharedPtr<class CCollider3D> dest)
{
	Vector3	srcCenter = src->GetCenter();
	Vector3	destCenter = dest->GetCenter();
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
	float	srcDist = camera->GetWorldPos().Distance(srcCenter) - src->GetRadius();
	float	destDist = camera->GetWorldPos().Distance(destCenter) - dest->GetRadius();
	return srcDist < destDist;
}

void CSceneCollision::AddCollider(CCollider* collider)
{
	m_ColliderList.push_back(collider);
}

bool CSceneCollision::CollisionWidget()
{
	return m_CollisionWidget = m_Owner->GetViewport()->CollisionMouse();
}

bool CSceneCollision::Init()
{
	float width = (float)CDevice::GetInst()->GetResolution().width;
	float height = (float)CDevice::GetInst()->GetResolution().height;
	CreateSection2D(10, 10, Vector2(), Vector2(width, height));
	CreateSection3D(5, 5, 5, Vector3(), Vector3(1000.f, 1000.f, 1000.f));
	return true;
}

void CSceneCollision::Update(float deltaTime)
{
	for (int i = 0; i < m_Section2D.count; ++i)
	{
		m_Section2D.vecSection[i]->m_ColliderList.clear();
	}
	for (int i = 0; i < m_Section3D.count; ++i)
	{
		m_Section3D.vecSection[i]->m_ColliderList.clear();
	}
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			(*iter)->SendPrevCollisionEnd();
			++iter;
			continue;
		}
		CheckSection(*iter);
		++iter;
	}
	// 현재 충돌영역이 겹치는지 판단한다. 이전프레임에 충돌되고 있던 물체와 충돌영역이 겹치는게 없다면
	// 현재 프레임에서는 충돌체크 자체를 아예 안하고 충돌 가능성도 0% 이기 때문에 충돌하던 물체가 떨어졌다는
	// 처리를 진행해주어야 한다.
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
		{
			continue;
		}
		(*iter)->CheckPrevCollisionColliderSection();
	}
	// 마우스와 먼저 충돌처리를 진행해야 한다.
	CollisionMouse(deltaTime);
	for (int i = 0; i < m_Section2D.count; ++i)
	{
		m_Section2D.vecSection[i]->Collision(deltaTime);
	}
	for (int i = 0; i < m_Section3D.count; ++i)
	{
		m_Section3D.vecSection[i]->Collision(deltaTime);
	}
	iter = m_ColliderList.begin();
	iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
		{
			continue;
		}
		(*iter)->ClearFrame();
	}
}

void CSceneCollision::CreateSection2D(int countX, int countY, const Vector2& worldStart, const Vector2& sectionSize)
{
	size_t	count = m_Section2D.vecSection.size();
	for (size_t i = 0; i < count; ++i)
	{
		SAFE_DELETE(m_Section2D.vecSection[i]);
	}
	m_Section2D.countX = countX;
	m_Section2D.countY = countY;
	m_Section2D.count = countX * countY;
	m_Section2D.worldStart = worldStart;
	m_Section2D.sectionSize = sectionSize;
	m_Section2D.worldSize = sectionSize * Vector2((float)countX, (float)countY);
	for (int i = 0; i < countY; ++i)
	{
		for (int j = 0; j < countX; ++j)
		{
			CCollisionSection2D* section = new CCollisionSection2D;
			section->m_Min = worldStart + sectionSize * Vector2((float)j, (float)i);
			section->m_Max = section->m_Min + sectionSize;
			section->m_IndexX = j;
			section->m_IndexY = i;
			section->m_Index = i * countX + j;
			m_Section2D.vecSection.push_back(section);
		}
	}
}

void CSceneCollision::CreateSection3D(int countX, int countY, int countZ, const Vector3& worldStart
	, const Vector3& sectionSize)
{
	size_t	count = m_Section3D.vecSection.size();
	for (size_t i = 0; i < count; ++i)
	{
		SAFE_DELETE(m_Section3D.vecSection[i]);
	}
	m_Section3D.countX = countX;
	m_Section3D.countY = countY;
	m_Section3D.countZ = countZ;
	m_Section3D.count = countX * countY * countZ;
	m_Section3D.worldStart = worldStart;
	m_Section3D.sectionSize = sectionSize;
	m_Section3D.worldSize = sectionSize * Vector3((float)countX, (float)countY, (float)countZ);
	for (int i = 0; i < countZ; ++i)
	{
		for (int j = 0; j < countY; ++j)
		{
			for (int k = 0; k < countX; ++k)
			{
				CCollisionSection3D* section = new CCollisionSection3D;
				section->m_Min = worldStart + sectionSize * Vector3((float)k, (float)j, (float)i);
				section->m_Max = section->m_Min + sectionSize;
				section->m_Center = (section->m_Min + section->m_Max) * 0.5f;
				section->m_Radius = (section->m_Max - section->m_Min).Length() * 0.5f;
				section->m_IndexX = k;
				section->m_IndexY = j;
				section->m_IndexZ = i;
				section->m_Index = i * countX * countY + j * countX + k;
				m_Section3D.vecSection.push_back(section);
			}
		}
	}
}

void CSceneCollision::Save(FILE* file)
{
	/*
	Section2D	m_Section2D;
	Section3D	m_Section3D;
	이거는 저장 필요할 수도 있음
	*/
}

void CSceneCollision::Load(FILE* file)
{
}

void CSceneCollision::CollisionMouse(float deltaTime)
{
	if (m_MouseCollision)
	{
		if (!m_MouseCollision->GetActive())
		{
			m_MouseCollision->CallCollisionMouseCallback(ECollision_Result::Release);
			m_MouseCollision = nullptr;
		}
	}
	// UI와 마우스가 충돌이 없을 경우 월드물체와 충돌을 시작한다.
	bool mouseWorldCollision = false;
	if (!m_CollisionWidget)
	{
		Vector2	mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
		// 마우스가 2D Section에서 어느 영역에 있는지를 판단한다.
		Vector2	mouseIndex = mouseWorldPos;
		mouseIndex -= m_Section2D.worldStart;
		int	indexX =(int)(mouseIndex.x / m_Section2D.sectionSize.x);
		int	indexY =(int)(mouseIndex.y / m_Section2D.sectionSize.y);
		indexX = indexX < 0 ? -1 : indexX;
		indexY = indexY < 0 ? -1 : indexY;
		indexX = indexX >= m_Section2D.countX ? -1 : indexX;
		indexY = indexY >= m_Section2D.countY ? -1 : indexY;
		if (indexX != -1 && indexY != -1)
		{
			int index = indexY * m_Section2D.countX + indexX;
			CCollider* colliderMouse = m_Section2D.vecSection[index]-> CollisionMouse(mouseWorldPos, deltaTime);
			if (colliderMouse)
			{
				if (colliderMouse != m_MouseCollision)
				{
					colliderMouse->CallCollisionMouseCallback(ECollision_Result::Collision);
				}
				if (m_MouseCollision && m_MouseCollision != colliderMouse)
				{
					m_MouseCollision->CallCollisionMouseCallback(ECollision_Result::Release);
				}
				m_MouseCollision = colliderMouse;
				mouseWorldCollision = true;
			}
		}
	}
	// 마우스와 UI가 충돌되었다면 기존에 충돌되고 있던 월드물체가 있을 경우 충돌 해제한다.
	else if (m_MouseCollision)
	{
		m_MouseCollision->CallCollisionMouseCallback(ECollision_Result::Release);
		m_MouseCollision = nullptr;
	}
	if (!mouseWorldCollision)
	{
		if (m_MouseCollision)
		{
			m_MouseCollision->CallCollisionMouseCallback(ECollision_Result::Release);
			m_MouseCollision = nullptr;
		}
	}
}

void CSceneCollision::CheckSection(CCollider* collider)
{
	Vector3	min = collider->GetMin();
	Vector3	max = collider->GetMax();
	if (collider->GetColliderType() == ECollider_Type::Collider2D)
	{
		min -= Vector3(m_Section2D.worldStart.x, m_Section2D.worldStart.y, 0.f);
		max -= Vector3(m_Section2D.worldStart.x, m_Section2D.worldStart.y, 0.f);
		int	indexMinX, indexMinY, indexMaxX, indexMaxY;
		indexMinX = (int)(min.x / m_Section2D.sectionSize.x);
		indexMinY = (int)(min.y / m_Section2D.sectionSize.y);
		indexMaxX = (int)(max.x / m_Section2D.sectionSize.x);
		indexMaxY = (int)(max.y / m_Section2D.sectionSize.y);
		indexMinX = indexMinX < 0 ? 0 : indexMinX;
		indexMinY = indexMinY < 0 ? 0 : indexMinY;
		indexMaxX = indexMaxX >= m_Section2D.countX ? m_Section2D.countX - 1 : indexMaxX;
		indexMaxY = indexMaxY >= m_Section2D.countY ? m_Section2D.countY - 1 : indexMaxY;
		for (int i = indexMinY; i <= indexMaxY; ++i)
		{
			for (int j = indexMinX; j <= indexMaxX; ++j)
			{
				int	index = i * m_Section2D.countX + j;
				m_Section2D.vecSection[index]->AddCollider((CCollider2D*)collider);
			}
		}
	}
	else
	{
		min -= m_Section3D.worldStart;
		max -= m_Section3D.worldStart;
		int	indexMinX, indexMinY, indexMinZ, indexMaxX, indexMaxY, indexMaxZ;
		indexMinX = (int)(min.x / m_Section3D.sectionSize.x);
		indexMinY = (int)(min.y / m_Section3D.sectionSize.y);
		indexMinZ = (int)(min.z / m_Section3D.sectionSize.z);
		indexMaxX = (int)(max.x / m_Section3D.sectionSize.x);
		indexMaxY = (int)(max.y / m_Section3D.sectionSize.y);
		indexMaxZ = (int)(max.z / m_Section3D.sectionSize.z);

		indexMaxX = indexMaxX >= m_Section3D.countX ? m_Section3D.countX - 1 : indexMaxX;
		indexMaxY = indexMaxY >= m_Section3D.countY ? m_Section3D.countY - 1 : indexMaxY;
		indexMaxZ = indexMaxZ >= m_Section3D.countZ ? m_Section3D.countZ - 1 : indexMaxZ;
		indexMinX = indexMinX > indexMaxX ? indexMaxX : indexMinX;
		indexMinY = indexMinY > indexMaxY ? indexMaxY : indexMinY;
		indexMinZ = indexMinZ > indexMaxZ ? indexMaxZ : indexMinZ;
		indexMinX = indexMinX < 0 ? 0 : indexMinX;
		indexMinY = indexMinY < 0 ? 0 : indexMinY;
		indexMinZ = indexMinZ < 0 ? 0 : indexMinZ;

		for (int i = indexMinZ; i <= indexMaxZ; ++i)
		{
			for (int j = indexMinY; j <= indexMaxY; ++j)
			{
				for (int k = indexMinX; k <= indexMaxX; ++k)
				{
					int	index = i * m_Section3D.countX * m_Section3D.countY +j * m_Section3D.countZ + k;
					m_Section3D.vecSection[index]->AddCollider((CCollider3D*)collider);
				}
			}
		}
	}
}

bool CSceneCollision::CreatePixelCollision(const std::string& name, const TCHAR* fileName
	, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	TCHAR	fullPath[MAX_PATH] = {};
	if (path)
	{
		lstrcpy(fullPath, path->path);
	}
	lstrcat(fullPath, fileName);
	return CreatePixelCollisionFullPath(name, fullPath);
}

bool CSceneCollision::CreatePixelCollisionFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (FindPixelCollision(name))
	{
		return true;
	}
	TCHAR	_FileExt[_MAX_EXT] = {};
	_wsplitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, _FileExt, _MAX_EXT);
	char	fileExt[_MAX_EXT] = {};
#ifdef UNICODE
	int	convertLength = WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, fileExt, convertLength, nullptr, nullptr);
#else
	strcpy_s(fileExt, _FileExt);
#endif // UNICODE
	_strupr_s(fileExt);
	DirectX::ScratchImage* image = new DirectX::ScratchImage;
	if (strcmp(fileExt, ".DDS") == 0)
	{
		if (FAILED(DirectX::LoadFromDDSFile(fullPath, DirectX::DDS_FLAGS_NONE, nullptr, *image)))
		{
			SAFE_DELETE(image);
			return false;
		}
	}
	else if (strcmp(fileExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(fullPath, nullptr, *image)))
		{
			SAFE_DELETE(image);
			return false;
		}
	}
	else
	{
		if (FAILED(DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_NONE, nullptr, *image)))
		{
			SAFE_DELETE(image);
			return false;
		}
	}
	PixelInfo* info = new PixelInfo;
	info->refCount = 1;
	info->width = (unsigned int)image->GetImages()[0].width;
	info->height = (unsigned int)image->GetImages()[0].height;
	info->pixel = new unsigned char[image->GetPixelsSize()];
	memcpy(info->pixel, image->GetPixels(), image->GetPixelsSize());
	if (strcmp(fileExt, ".BMP") == 0)
	{
		unsigned int lineSize = info->width * 4;
		unsigned char* line = new unsigned char[lineSize];
		int	halfHeight = info->height / 2;
		for (int i = 0; i < halfHeight; ++i)
		{
			memcpy(line, &info->pixel[i * lineSize], lineSize);
			memcpy(&info->pixel[i * lineSize], &info->pixel[(info->height - 1 - i) * lineSize], lineSize);
			memcpy(&info->pixel[(info->height - 1 - i) * lineSize], line, lineSize);
		}
		SAFE_DELETE_ARRAY(line);
	}
	if (FAILED(DirectX::CreateShaderResourceView(CDevice::GetInst()->GetDevice(),
		image->GetImages(), image->GetImageCount(), image->GetMetadata(), &info->SRV)))
	{
		return false;
	}
	m_mapPixelCollision.insert(std::make_pair(name, info));
	SAFE_DELETE(image);
	return true;
}

bool CSceneCollision::CreatePixelCollisionMultibyte(const std::string& name, const char* fileName
	, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	char	fullPath[MAX_PATH] = {};
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	return CreatePixelCollisionMultibyteFullPath(name, fullPath);
}

bool CSceneCollision::CreatePixelCollisionMultibyteFullPath(const std::string& name, const char* fullPath)
{
	TCHAR	wideCharFullPath[MAX_PATH] = {};
	int	length = MultiByteToWideChar(CP_ACP, 0, fullPath, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, fullPath, -1, wideCharFullPath, length);
	return CreatePixelCollisionFullPath(name, wideCharFullPath);
}

PixelInfo* CSceneCollision::FindPixelCollision(const std::string& name)
{
	auto	iter = m_mapPixelCollision.find(name);
	if (iter == m_mapPixelCollision.end())
	{
		return nullptr;
	}
	return iter->second;
}

bool CSceneCollision::Picking(PickingResult& result)
{
	Ray	ray = CInput::GetInst()->GetRay();
	Matrix	matView = m_Owner->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	matView.Inverse();
	ray.ConvertSpace(matView);
	// 광선이 관통하는 구를 찾는다.
	size_t	sectionCount = m_Section3D.vecSection.size();
	std::list<int>	sectionList;
	for (size_t i = 0; i < sectionCount; ++i)
	{
		Vector3 center = m_Section3D.vecSection[i]->m_Center;
		float radius = m_Section3D.vecSection[i]->m_Radius;
		PickingResult	sectionResult;
		if (CCollisionManager::GetInst()->CollisionRayToSphere(sectionResult, ray, center, radius))
		{
			sectionList.push_back((int)i);
		}
	}
	sectionList.sort(CSceneCollision::SortPickginSection);
	auto	iter = sectionList.begin();
	auto	iterEnd = sectionList.end();
	for (; iter != iterEnd; ++iter)
	{
		int	idx = *iter;
		std::list<CSharedPtr<CCollider3D>>	list = m_Section3D.vecSection[idx]->m_ColliderList;
		if (list.empty())
		{
			continue;
		}
		list.sort(CSceneCollision::SortColliderList);
		auto	iter1 =list.begin();
		auto	iter1End = list.end();
		for (; iter1 != iter1End; ++iter1)
		{
			if (!(*iter1)->GetActive() || !(*iter1)->GetEnable() || (*iter1)->GetFrustumCull())
			{
				continue;
			}
			if (CCollisionManager::GetInst()->CollisionRayToSphere(result, ray, (*iter1)->GetCenter(), 
				(*iter1)->GetRadius()))
			{
				result.pickObject = (*iter1)->GetOwner();
				result.pickComponent = *iter1;
				return true;
			}
		}
	}
	return false;
}
