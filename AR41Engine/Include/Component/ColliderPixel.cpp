#include "ColliderPixel.h"
#include "ColliderBox2D.h"
#include "ColliderOBB2D.h"
#include "ColliderSphere2D.h"
#include "CameraComponent.h"
#include "../Device.h"
#include "../Engine.h"
#include "../CollisionManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Scene/SceneCollision.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CColliderPixel::CColliderPixel()
	: m_Info(nullptr)
{
	SetTypeID<CColliderPixel>();
	m_ComponentTypeName = "ColliderPixel";
	m_Collider2DType = ECollider2D_Type::Pixel;
}

CColliderPixel::CColliderPixel(const CColliderPixel& component)
	: CCollider2D(component)
{
	m_Info = component.m_Info;
	if (m_Info)
	{
		++m_Info->refCount;
	}
	m_PixelMesh = component.m_PixelMesh;
	m_PixelShader = component.m_PixelShader;
}

CColliderPixel::~CColliderPixel()
{
	if (m_Info)
	{
		--m_Info->refCount;
		if (m_Info->refCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->pixel);
			SAFE_DELETE(m_Info);
		}
	}
}

void CColliderPixel::Start()
{
	CCollider2D::Start();
}

bool CColliderPixel::Init()
{
	if (!CCollider2D::Init())
	{
		return false;
	}
	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box2DLineMesh");
		m_PixelMesh = CResourceManager::GetInst()->FindMesh("CenterUVRect");
		m_PixelShader = CResourceManager::GetInst()->FindShader("ColliderImageShader");
	}
	return true;
}

void CColliderPixel::Update(float deltaTime)
{
	CCollider2D::Update(deltaTime);
}

void CColliderPixel::PostUpdate(float deltaTime)
{
	CCollider2D::PostUpdate(deltaTime);
	Vector2	size = Vector2((float)m_Info->width, (float)m_Info->height);
	size.x *= GetWorldScale().x;
	size.y *= GetWorldScale().y;
	Vector3	min, max;
	min.x = GetWorldPos().x - size.x * 0.5f;
	min.y = GetWorldPos().y - size.y * 0.5f;
	max.x = min.x + size.x;
	max.y = min.y + size.y;
	m_Info->box2D.left = min.x;
	m_Info->box2D.bottom = min.y;
	m_Info->box2D.right = max.x;
	m_Info->box2D.top = max.y;
	min.x -= GetWorldPos().x;
	min.y -= GetWorldPos().y;
	max.x -= GetWorldPos().x;
	max.y -= GetWorldPos().y;
	m_Transform->SetMin(min);
	m_Transform->SetMax(max);
}

void CColliderPixel::Render()
{
	CCollider2D::Render();
	Matrix	matScale, matTranslate, matWorld;
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();
	Vector3	scale = Vector3((float)m_Info->width, (float)m_Info->height, 1.f);
	scale.x *= GetWorldScale().x;
	scale.y *= GetWorldScale().y;
	scale.z = GetWorldScale().z;
	matScale.Scaling(scale);
	matTranslate.Translation(GetWorldPos());
	matWorld = matScale * matTranslate;
	CColliderConstantBuffer* buffer = CResourceManager::GetInst()->GetColliderCBuffer();
	buffer->SetColor(m_Color);
	buffer->SetWVP(matWorld * matView * matProj);
	buffer->UpdateBuffer();
	CDevice::GetInst()->GetContext()->PSSetShaderResources(0, 1, &m_Info->SRV);
	m_PixelShader->SetShader();
	m_PixelMesh->Render();
	m_Shader->SetShader();
	m_Mesh->Render();
}

CColliderPixel* CColliderPixel::Clone() const
{
	return new CColliderPixel(*this);
}

void CColliderPixel::Save(FILE* file)
{
	CCollider2D::Save(file);
	bool	pixelInfoEnable = m_Info ? true : false;
	fwrite(&pixelInfoEnable, sizeof(bool), 1, file);
	if (pixelInfoEnable)
	{
		int	length = (int)m_Info->name.length();
		fwrite(&length, sizeof(int), 1, file);
		fwrite(m_Info->name.c_str(), 1, length, file);
		fwrite(m_Info->fileName, sizeof(TCHAR), MAX_PATH, file);
		fwrite(m_Info->pathName, sizeof(char), MAX_PATH, file);
		fwrite(&m_Info->box2D, sizeof(Box2DInfo), 1, file);
		fwrite(&m_Info->pixelColorCollisionType, sizeof(EPixelCollision_Type), 1, file);
		fwrite(&m_Info->pixelAlphaCollisionType, sizeof(EPixelCollision_Type), 1, file);
		fwrite(m_Info->typeColor, sizeof(unsigned char), 4, file);
	}
}

void CColliderPixel::Load(FILE* file)
{
	CCollider2D::Load(file);
	bool	pixelInfoEnable = false;
	fread(&pixelInfoEnable, sizeof(bool), 1, file);
	if (pixelInfoEnable)
	{
		if (m_Info)
		{
			--m_Info->refCount;
			if (m_Info->refCount == 0)
			{
				SAFE_RELEASE(m_Info->SRV);
				SAFE_DELETE_ARRAY(m_Info->pixel);
				SAFE_DELETE(m_Info);
			}
		}
		TCHAR	fileName[MAX_PATH] = {};
		char	pathName[MAX_PATH] = {};
		int	length = 0;
		char	name[MAX_PATH] = {};
		fread(&length, sizeof(int), 1, file);
		assert(length >= 0);
		fread(name, 1, length, file);
		fread(fileName, sizeof(TCHAR), MAX_PATH, file);
		fread(pathName, sizeof(char), MAX_PATH, file);
		SetInfo(name, fileName, pathName);
		fread(&m_Info->box2D, sizeof(Box2DInfo), 1, file);
		fread(&m_Info->pixelColorCollisionType, sizeof(EPixelCollision_Type), 1, file);
		fread(&m_Info->pixelAlphaCollisionType, sizeof(EPixelCollision_Type), 1, file);
		fread(m_Info->typeColor, sizeof(unsigned char), 4, file);
	}
}

bool CColliderPixel::Collision(CCollider* dest)
{
	Vector2	hitPoint;
	bool	result = false;
	switch (((CCollider2D*)dest)->GetCollider2DType())
	{
	case ECollider2D_Type::Box2D:
		result = CCollisionManager::GetInst()->CollisionBox2DToPixel(hitPoint, (CColliderBox2D*)dest, this);
		break;
	case ECollider2D_Type::OBB2D:
		result = CCollisionManager::GetInst()->CollisionOBB2DToPixel(hitPoint, (CColliderOBB2D*)dest, this);
		break;
	case ECollider2D_Type::Sphere2D:
		result = CCollisionManager::GetInst()->CollisionSphere2DToPixel(hitPoint, (CColliderSphere2D*)dest, this);
		break;
	case ECollider2D_Type::Pixel:
		result = CCollisionManager::GetInst()->CollisionPixelToPixel(hitPoint, this, (CColliderPixel*)dest);
		break;
	}
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
	m_Result.hitPoint = m_HitPoint;
	dest->SetCollisionResultHitPoint(m_HitPoint);
	return result;
}

bool CColliderPixel::CollisionMouse(const Vector2& mouseWorldPos)
{
	Vector2	hitPoint;
	m_MouseCollision = CCollisionManager::GetInst()->CollisionPointToPixel(hitPoint, mouseWorldPos,*m_Info);
	m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
	return m_MouseCollision;
}

void CColliderPixel::SetInfo(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (!m_Scene->GetCollisionManager()->CreatePixelCollision(name, fileName, pathName))
	{
		return;
	}
	if (m_Info)
	{
		--m_Info->refCount;
		if (m_Info->refCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->pixel);
			SAFE_DELETE(m_Info);
		}
	}
	m_Info = m_Scene->GetCollisionManager()->FindPixelCollision(name);
	++m_Info->refCount;
	m_Info->name = name;
	lstrcpy(m_Info->fileName, fileName);
	strcpy_s(m_Info->pathName, pathName.c_str());
}

void CColliderPixel::SetInfoFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (!m_Scene->GetCollisionManager()->CreatePixelCollisionFullPath(name, fullPath))
	{
		return;
	}
	if (m_Info)
	{
		--m_Info->refCount;
		if (m_Info->refCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->pixel);
			SAFE_DELETE(m_Info);
		}
	}
	m_Info = m_Scene->GetCollisionManager()->FindPixelCollision(name);
	++m_Info->refCount;
	m_Info->name = name;
	int	pathLength = lstrlen(fullPath);
	for (int i = pathLength - 1; i >= 0; --i)
	{
		// D:\41th\DirectX\Client\Bin\Texture\Test.png
		if ((fullPath[i] == '\\' || fullPath[i] == '/') && i >= 4)
		{
			if ((fullPath[i - 1] == 'n' || fullPath[i - 1] == 'N') &&
				(fullPath[i - 2] == 'i' || fullPath[i - 2] == 'I') &&
				(fullPath[i - 3] == 'b' || fullPath[i - 3] == 'B') &&
				(fullPath[i - 4] == '\\' || fullPath[i - 4] == '/'))
			{
				lstrcpy(m_Info->fileName, &fullPath[i + 1]);
				break;
			}
		}
	}
	strcpy_s(m_Info->pathName, strlen(ROOT_PATH) + 1, ROOT_PATH);
}

void CColliderPixel::SetInfoMultibyte(const std::string& name, const char* fileName, const std::string& pathName)
{
	if (!m_Scene->GetCollisionManager()->CreatePixelCollisionMultibyte(name, fileName, pathName))
	{
		return;
	}
	if (m_Info)
	{
		--m_Info->refCount;
		if (m_Info->refCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->pixel);
			SAFE_DELETE(m_Info);
		}
	}
	m_Info = m_Scene->GetCollisionManager()->FindPixelCollision(name);
	++m_Info->refCount;
	m_Info->name = name;
	TCHAR	convertFileName[MAX_PATH] = {};
#ifdef UNICODE
	int	convertLength = MultiByteToWideChar(CP_ACP, 0, fileName, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, fileName, -1, convertFileName, convertLength);
#else
	strcpy_s(convertFileName, fileName);
#endif // UNICODE
	lstrcpy(m_Info->fileName, convertFileName);
	strcpy_s(m_Info->pathName, pathName.c_str());
}

void CColliderPixel::SetInfoMultibyteFullPath(const std::string& name, const char* fullPath)
{
	if (!m_Scene->GetCollisionManager()->CreatePixelCollisionMultibyteFullPath(name, fullPath))
	{
		return;
	}
	if (m_Info)
	{
		--m_Info->refCount;
		if (m_Info->refCount == 0)
		{
			SAFE_RELEASE(m_Info->SRV);
			SAFE_DELETE_ARRAY(m_Info->pixel);
			SAFE_DELETE(m_Info);
		}
	}
	m_Info = m_Scene->GetCollisionManager()->FindPixelCollision(name);
	++m_Info->refCount;
	m_Info->name = name;
	TCHAR	convertFullPath[MAX_PATH] = {};
#ifdef UNICODE
	int	convertLength = MultiByteToWideChar(CP_ACP, 0, fullPath, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, fullPath, -1, convertFullPath, convertLength);
#else
	strcpy_s(convertFullPath, fullPath);
#endif // UNICODE
	int	pathLength = lstrlen(convertFullPath);
	for (int i = pathLength - 1; i >= 0; --i)
	{
		if ((fullPath[i] == '\\' || fullPath[i] == '/') && i >= 4)
		{
			if ((fullPath[i - 1] == 'n' || fullPath[i - 1] == 'N') &&
				(fullPath[i - 2] == 'i' || fullPath[i - 2] == 'I') &&
				(fullPath[i - 3] == 'b' || fullPath[i - 3] == 'B') &&
				(fullPath[i - 4] == '\\' || fullPath[i - 4] == '/'))
			{
				lstrcpy(m_Info->fileName, &convertFullPath[i + 1]);
				break;
			}
		}
	}
	strcpy_s(m_Info->pathName, strlen(ROOT_PATH) + 1, ROOT_PATH);
}

void CColliderPixel::SetPixelColorCollisionType(EPixelCollision_Type type)
{
	if (!m_Info)
	{
		return;
	}
	m_Info->pixelColorCollisionType = type;
}

void CColliderPixel::SetPixelAlphaCollisionType(EPixelCollision_Type type)
{
	if (!m_Info)
	{
		return;
	}
	m_Info->pixelAlphaCollisionType = type;
}

void CColliderPixel::SetPixelColor(unsigned char r, unsigned char g, unsigned char b)
{
	if (!m_Info)
	{
		return;
	}
	m_Info->typeColor[0] = r;
	m_Info->typeColor[1] = g;
	m_Info->typeColor[2] = b;
}

void CColliderPixel::SetPixelAlpha(unsigned char a)
{
	if (!m_Info)
	{
		return;
	}
	m_Info->typeColor[3] = a;
}
