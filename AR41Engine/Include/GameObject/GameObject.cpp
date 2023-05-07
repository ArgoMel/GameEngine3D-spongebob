#include "GameObject.h"
#include "../Input.h"
#include "../CollisionManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../Component/CameraComponent.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/Animation/SkeletonSocket.h"
std::unordered_map<std::string, CGameObject*> CGameObject::m_mapObjectCDO;

CGameObject::CGameObject()
    : m_Parent(nullptr)
    , m_Scene(nullptr)
    , m_TimeScale(1.f)
    , m_LifeTime(-1.f)
	, m_ComponentSerialNumber(0)
	, m_Start(false)
	, m_FrustumCull(false)
	, m_Radius(0.f)
{
	SetTypeID<CGameObject>();
	m_ObjectTypeName = "GameObject";
}

CGameObject::CGameObject(const CGameObject& obj)
    : CRef(obj)
	, m_Parent(nullptr)
	, m_Scene(nullptr)
{
	m_ObjectTypeName = obj.m_ObjectTypeName;
	m_TimeScale = obj.m_TimeScale;
	m_LifeTime = obj.m_LifeTime;
	m_ComponentSerialNumber = obj.m_ComponentSerialNumber;
	m_Start = false;
	if (obj.m_RootComponent)
	{
		m_RootComponent = obj.m_RootComponent->Clone();
		m_RootComponent->SetOwner(this);
		m_RootComponent->AddOwner();
	}
	{
		m_vecObjectComponent.clear();
		auto	iter = obj.m_vecObjectComponent.begin();
		auto	iterEnd = obj.m_vecObjectComponent.end();
		for (; iter != iterEnd; ++iter)
		{
			CObjectComponent* component = (*iter)->Clone();
			component->SetOwner(this);
			m_vecObjectComponent.push_back(component);
		}
	}
}

CGameObject::~CGameObject()
{
	if (m_RootComponent)
	{
		m_RootComponent->Destroy();
	}
	size_t	size = m_vecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObjectComponent[i]->Destroy();
	}
}

bool CGameObject::SortComponent(CSceneComponent* src, CSceneComponent* dest)
{
	Vector3	srcCenter = src->GetCenter();
	Vector3	destCenter = dest->GetCenter();
	CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
	float	srcDist = camera->GetWorldPos().Distance(srcCenter) - src->GetRadius();
	float	destDist = camera->GetWorldPos().Distance(destCenter) - dest->GetRadius();
	return srcDist < destDist;
}

void CGameObject::SetScene(CScene* scene)
{
	m_Scene = scene;
	if (m_RootComponent)
	{
		m_RootComponent->SetScene(scene);
	}
	size_t	size = m_vecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObjectComponent[i]->SetScene(scene);
	}
}

void CGameObject::AddChildToSocket(const std::string& socketName, CGameObject* obj)
{
	CAnimationMeshComponent* mesh = nullptr;	// 애니메이션메쉬 컴포넌트가 있는지 판단한다.
	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->CheckTypeID<CAnimationMeshComponent>())
		{
			mesh = (CAnimationMeshComponent*)*iter;
			break;
		}
	}
	if (!mesh)
	{
		return;
	}
	CSkeletonSocket* socket = mesh->GetSkeleton()->GetSocket(socketName);
	if (!socket)
	{
		return;
	}
	obj->GetRootComponent()->SetSocket(socket);
}

void CGameObject::Destroy()
{
	CRef::Destroy();
	if (m_RootComponent)
	{
		m_RootComponent->Destroy();
	}
	size_t	size = m_vecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObjectComponent[i]->Destroy();
	}
}

void CGameObject::GetAllComponentHierarchyName(std::vector<HierarchyName>& vecName)
{
	if (m_RootComponent)
	{
		HierarchyName	names;
		names.name = m_RootComponent->GetName();
		names.className = m_RootComponent->GetComponentTypeName();
		names.component = m_RootComponent;
		names.parent = nullptr;
		vecName.push_back(names);
		m_RootComponent->GetAllComponentHierarchyName(vecName);
	}
}

CComponent* CGameObject::FindComponent(const std::string& name)
{
	auto    iter = m_SceneComponentList.begin();
	auto    iterEnd = m_SceneComponentList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	auto    iter1 = m_vecObjectComponent.begin();
	auto    iter1End = m_vecObjectComponent.end();
	for (; iter1 != iter1End; ++iter1)
	{
		if ((*iter1)->GetName() == name)
		{
			return *iter1;
		}
	}
	return nullptr;
}

void CGameObject::Start()
{
	m_Start = true;
	if (m_RootComponent)
	{
		m_PrevPos = GetWorldPos();
		m_RootComponent->Start();
	}
	size_t	size = m_vecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObjectComponent[i]->Start();
	}
}

bool CGameObject::Init()
{
    return true;
}

void CGameObject::Update(float deltaTime)
{
	if (m_LifeTime > 0.f)
	{
		m_LifeTime -= deltaTime;
		if (m_LifeTime <= 0.f)
		{
			Destroy();
			return;
		}
	}
	size_t	size = m_vecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObjectComponent[i]->Update(deltaTime);
	}
	if (m_RootComponent)
	{
		m_RootComponent->Update(deltaTime);
	}
}

void CGameObject::PostUpdate(float deltaTime)
{
	size_t	size = m_vecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecObjectComponent[i]->PostUpdate(deltaTime);
	}
	if (m_RootComponent)
	{
		m_Move = GetWorldPos() - m_PrevPos;
		m_PrevPos = GetWorldPos();
		m_RootComponent->PostUpdate(deltaTime);
	}
	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();
	m_Min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_Max = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
	for (; iter != iterEnd; ++iter)
	{
		Vector3	min = (*iter)->GetMeshSize() * (*iter)->GetWorldScale();
		if (m_Min.x > (*iter)->GetMin().x)
		{
			m_Min.x = (*iter)->GetMin().x;
		}
		if (m_Min.y > (*iter)->GetMin().y)
		{
			m_Min.y = (*iter)->GetMin().y;
		}
		if (m_Min.z > (*iter)->GetMin().z)
		{
			m_Min.z = (*iter)->GetMin().z;
		}
		if (m_Max.x < (*iter)->GetMax().x)
		{
			m_Max.x = (*iter)->GetMax().x;
		}
		if (m_Max.y < (*iter)->GetMax().y)
		{
			m_Max.y = (*iter)->GetMax().y;
		}
		if (m_Max.z < (*iter)->GetMax().z)
		{
			m_Max.z = (*iter)->GetMax().z;
		}
	}
	m_Center = (m_Min + m_Max) * 0.5f;
	m_Radius = (m_Max - m_Min).Length() * 0.5f;
}

CGameObject* CGameObject::Clone() const
{
    return new CGameObject(*this);
}

void CGameObject::Save(FILE* file)
{
	CRef::Save(file);
	fwrite(&m_LifeTime, 4, 1, file);
	int	length = (int)m_RootComponent->GetComponentTypeName().length();
	fwrite(&length, 4, 1, file);
	fwrite(m_RootComponent->GetComponentTypeName().c_str(), 1, length, file);
	m_RootComponent->Save(file);
	{
		int	count = (int)m_vecObjectComponent.size();
		fwrite(&count, 4, 1, file);
		auto	iter = m_vecObjectComponent.begin();
		auto	iterEnd = m_vecObjectComponent.end();
		for (; iter != iterEnd; ++iter)
		{
			length = (int)(*iter)->GetComponentTypeName().length();
			fwrite(&length, 4, 1, file);
			fwrite((*iter)->GetComponentTypeName().c_str(), 1, length, file);
			(*iter)->Save(file);
		}
	}
}

void CGameObject::Load(FILE* file)
{
	CRef::Load(file);
	fread(&m_LifeTime, 4, 1, file);
	size_t	length = 0;
	char	typeName[256] = {};
	fread(&length, 4, 1, file);
	if (length > MAXCHAR)
	{
		throw std::runtime_error("file data unexpected size");
	}
	fread(typeName, 1, length, file);
	if (!m_RootComponent)
	{
		CComponent* CDO = CComponent::FindCDO(typeName);
		m_RootComponent = (CSceneComponent*)CDO->Clone();
		m_RootComponent->SetOwner(this);
		m_RootComponent->SetScene(m_Scene);
		m_RootComponent->Load(file);
		m_RootComponent->AddOwner();
	}
	else
	{
		m_RootComponent->SetOwner(this);
		m_RootComponent->SetScene(m_Scene);
		m_RootComponent->Load(file);
	}
	{
		int	count = 0;
		fread(&count, 4, 1, file);
		bool	empty = m_vecObjectComponent.empty();
		for (int i = 0; i < count; ++i)
		{
			int	length = 0;
			char	typeName[256] = {};
			fread(&length, 4, 1, file);
			if (length > MAXCHAR)
			{
				throw std::runtime_error("file data unexpected size");
			}
			fread(typeName, 1, length, file);
			if (empty)
			{
				CComponent* CDO = CComponent::FindCDO(typeName);
				CComponent* component = CDO->Clone();
				component->SetOwner(this);
				component->SetScene(m_Scene);
				component->Load(file);
				m_vecObjectComponent.push_back((CObjectComponent*)component);
			}
			else
			{
				m_vecObjectComponent[i]->SetOwner(this);
				m_vecObjectComponent[i]->SetScene(m_Scene);
				m_vecObjectComponent[i]->Load(file);
			}
		}
	}
}

int CGameObject::InflictDamage(float damage)
{
	return 0;
}

void CGameObject::Reset()
{
	SetWorldPosition(960.f,540.f);
}

void CGameObject::FrustumCull(CCameraComponent* camera)
{
	bool	cull = false;
	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (camera->FrustumInSphere((*iter)->GetWorldPos(), (*iter)->GetRadius()))
		{
			cull = true;
		}
	}
	m_FrustumCull = cull;
}

bool CGameObject::Picking(PickingResult& result)
{
	Ray	ray = CInput::GetInst()->GetRay();
	// 월드공간으로 변환한다.
	CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();
	Matrix	matView = camera->GetViewMatrix();
	matView.Inverse();
	ray.pos = ray.pos.TransformCoord(matView);
	ray.dir = ray.dir.TransformNormal(matView);
	ray.dir.Normalize();
	std::list<CSceneComponent*> sceneComponentList = m_SceneComponentList;
	sceneComponentList.sort(CGameObject::SortComponent);
	auto	iter = sceneComponentList.begin();
	auto	iterEnd = sceneComponentList.end();
	for (; iter != iterEnd; ++iter)
	{
		Vector3	center = (*iter)->GetCenter();
		float	radius = (*iter)->GetRadius();
		if (CCollisionManager::GetInst()->CollisionRayToSphere( result, ray, center, radius))
		{
			result.pickObject = this;
			result.pickComponent = *iter;
			return true;
		}
	}
	return false;
}

void CGameObject::SetInheritScale(bool inherit)
{
	if (m_RootComponent)
	{
		m_RootComponent->SetInheritScale(inherit);
	}
}

void CGameObject::SetInheritRotX(bool inherit)
{
	if (m_RootComponent)
	{
		m_RootComponent->SetInheritRotX(inherit);
	}
}

void CGameObject::SetInheritRotY(bool inherit)
{
	if (m_RootComponent)
	{
		m_RootComponent->SetInheritRotY(inherit);
	}
}

void CGameObject::SetInheritRotZ(bool inherit)
{
	if (m_RootComponent)
	{
		m_RootComponent->SetInheritRotZ(inherit);
	}
}

void CGameObject::SetInheritParentRotationPosX(bool inherit)
{
	if (m_RootComponent)
	{
		m_RootComponent->SetInheritParentRotationPosX(inherit);
	}
}

void CGameObject::SetInheritParentRotationPosY(bool inherit)
{
	if (m_RootComponent)
	{
		m_RootComponent->SetInheritParentRotationPosY(inherit);
	}
}

void CGameObject::SetInheritParentRotationPosZ(bool inherit)
{
	if (m_RootComponent)
	{
		m_RootComponent->SetInheritParentRotationPosZ(inherit);
	}
}

void CGameObject::InheritRotation(bool current)
{
	if (m_RootComponent)
	{
		m_RootComponent->InheritRotation(current);
	}
}

void CGameObject::InheritParentRotationPos()
{
	if (m_RootComponent)
	{
		m_RootComponent->InheritParentRotationPos();
	}
}

void CGameObject::InheritWorldRotation(bool current)
{
	m_RootComponent->InheritWorldRotation(current);
}

void CGameObject::InheritWorldParentRotationPos()
{
	m_RootComponent->InheritWorldParentRotationPos();
}

Vector3 CGameObject::GetRelativeScale() const
{
	return m_RootComponent->GetRelativeScale();
}

Vector3 CGameObject::GetRelativeRot() const
{
	return m_RootComponent->GetRelativeRot();
}

Vector3 CGameObject::GetRelativePos() const
{
	return m_RootComponent->GetRelativePos();
}

Vector3 CGameObject::GetRelativeAxis(AXIS axis) const
{
	return m_RootComponent->GetRelativeAxis(axis);
}

void CGameObject::SetRelativeScale(const Vector3& scale)
{
	m_RootComponent->SetRelativeScale(scale);
}

void CGameObject::SetRelativeScale(const Vector2& scale)
{
	m_RootComponent->SetRelativeScale(scale);
}

void CGameObject::SetRelativeScale(float x, float y)
{
	m_RootComponent->SetRelativeScale(x, y);
}

void CGameObject::SetRelativeScale(float x, float y, float z)
{
	m_RootComponent->SetRelativeScale(x, y, z);
}

void CGameObject::SetRelativeScaleX(float x)
{
	m_RootComponent->SetRelativeScaleX(x);
}

void CGameObject::SetRelativeScaleY(float y)
{
	m_RootComponent->SetRelativeScaleY(y);
}

void CGameObject::SetRelativeScaleZ(float z)
{
	m_RootComponent->SetRelativeScaleZ(z);
}

void CGameObject::SetRelativeRotation(const Vector3& rot)
{
	m_RootComponent->SetRelativeRotation(rot);
}

void CGameObject::SetRelativeRotation(const Vector2& rot)
{
	m_RootComponent->SetRelativeRotation(rot);
}

void CGameObject::SetRelativeRotation(float x, float y)
{
	m_RootComponent->SetRelativeRotation(x, y);
}

void CGameObject::SetRelativeRotation(float x, float y, float z)
{
	m_RootComponent->SetRelativeRotation(x, y, z);
}

void CGameObject::SetRelativeRotationX(float x)
{
	m_RootComponent->SetRelativeRotationX(x);
}

void CGameObject::SetRelativeRotationY(float y)
{
	m_RootComponent->SetRelativeRotationY(y);
}

void CGameObject::SetRelativeRotationZ(float z)
{
	m_RootComponent->SetRelativeRotationZ(z);
}

void CGameObject::SetRelativePosition(const Vector3& pos)
{
	m_RootComponent->SetRelativePosition(pos);
}

void CGameObject::SetRelativePosition(const Vector2& pos)
{
	m_RootComponent->SetRelativePosition(pos);
}

void CGameObject::SetRelativePosition(float x, float y)
{
	m_RootComponent->SetRelativePosition(x, y);
}

void CGameObject::SetRelativePosition(float x, float y, float z)
{
	m_RootComponent->SetRelativePosition(x, y, z);
}

void CGameObject::SetRelativePositionX(float x)
{
	m_RootComponent->SetRelativePositionX(x);
}

void CGameObject::SetRelativePositionY(float y)
{
	m_RootComponent->SetRelativePositionY(y);
}

void CGameObject::SetRelativePositionZ(float z)
{
	m_RootComponent->SetRelativePositionZ(z);
}

void CGameObject::AddRelativeScale(const Vector3& scale)
{
	m_RootComponent->AddRelativeScale(scale);
}

void CGameObject::AddRelativeScale(const Vector2& scale)
{
	m_RootComponent->AddRelativeScale(scale);
}

void CGameObject::AddRelativeScale(float x, float y)
{
	m_RootComponent->AddRelativeScale(x, y);
}

void CGameObject::AddRelativeScale(float x, float y, float z)
{
	m_RootComponent->AddRelativeScale(x, y, z);
}

void CGameObject::AddRelativeScaleX(float x)
{
	m_RootComponent->AddRelativeScaleX(x);
}

void CGameObject::AddRelativeScaleY(float y)
{
	m_RootComponent->AddRelativeScaleY(y);
}

void CGameObject::AddRelativeScaleZ(float z)
{
	m_RootComponent->AddRelativeScaleZ(z);
}

void CGameObject::AddRelativeRotation(const Vector3& rot)
{
	m_RootComponent->AddRelativeRotation(rot);
}

void CGameObject::AddRelativeRotation(const Vector2& rot)
{
	m_RootComponent->AddRelativeRotation(rot);
}

void CGameObject::AddRelativeRotation(float x, float y)
{
	m_RootComponent->AddRelativeRotation(x, y);
}

void CGameObject::AddRelativeRotation(float x, float y, float z)
{
	m_RootComponent->AddRelativeRotation(x, y, z);
}

void CGameObject::AddRelativeRotationX(float x)
{
	m_RootComponent->AddRelativeRotationX(x);
}

void CGameObject::AddRelativeRotationY(float y)
{
	m_RootComponent->AddRelativeRotationY(y);
}

void CGameObject::AddRelativeRotationZ(float z)
{
	m_RootComponent->AddRelativeRotationZ(z);
}

void CGameObject::AddRelativePosition(const Vector3& pos)
{
	m_RootComponent->AddRelativePosition(pos);
}

void CGameObject::AddRelativePosition(const Vector2& pos)
{
	m_RootComponent->AddRelativePosition(pos);
}

void CGameObject::AddRelativePosition(float x, float y)
{
	m_RootComponent->AddRelativePosition(x, y);
}

void CGameObject::AddRelativePosition(float x, float y, float z)
{
	m_RootComponent->AddRelativePosition(x, y, z);
}

void CGameObject::AddRelativePositionX(float x)
{
	m_RootComponent->AddRelativePositionX(x);
}

void CGameObject::AddRelativePositionY(float y)
{
	m_RootComponent->AddRelativePositionY(y);
}

void CGameObject::AddRelativePositionZ(float z)
{
	m_RootComponent->AddRelativePositionZ(z);
}

float CGameObject::GetRadius() const
{
	return m_RootComponent->GetRadius();
}

const Vector3& CGameObject::GetWorldScale() const
{
	return m_RootComponent->GetWorldScale();
}

const Vector3& CGameObject::GetWorldRot() const
{
	return m_RootComponent->GetWorldRot();
}

const Vector3& CGameObject::GetWorldPos() const
{
	return m_RootComponent->GetWorldPos();
}

const Vector3& CGameObject::GetWorldAxis(AXIS axis) const
{
	return m_RootComponent->GetWorldAxis(axis);
}

const Vector3& CGameObject::GetPivot() const
{
	return m_RootComponent->GetPivot();
}

const Vector3& CGameObject::GetMeshSize() const
{
	return m_RootComponent->GetMeshSize();
}

const Vector3& CGameObject::GetOffset() const
{
	return m_RootComponent->GetOffset();
}

const Matrix& CGameObject::GetWorldMatrix() const
{
	return m_RootComponent->GetWorldMatrix();
}

void CGameObject::SetPivot(const Vector3& pivot)
{
	m_RootComponent->SetPivot(pivot);
}

void CGameObject::SetPivot(const Vector2& pivot)
{
	m_RootComponent->SetPivot(pivot);
}

void CGameObject::SetPivot(float x, float y, float z)
{
	m_RootComponent->SetPivot(x, y, z);
}

void CGameObject::SetPivot(float x, float y)
{
	m_RootComponent->SetPivot(x, y);
}

void CGameObject::SetMeshSize(const Vector3& meshSize)
{
	m_RootComponent->SetMeshSize(meshSize);
}

void CGameObject::SetMeshSize(const Vector2& meshSize)
{
	m_RootComponent->SetMeshSize(meshSize);
}

void CGameObject::SetMeshSize(float x, float y, float z)
{
	m_RootComponent->SetMeshSize(x, y, z);
}

void CGameObject::SetMeshSize(float x, float y)
{
	m_RootComponent->SetMeshSize(x, y);
}

void CGameObject::SetWorldScale(const Vector3& scale)
{
	m_RootComponent->SetWorldScale(scale);
}

void CGameObject::SetWorldScale(const Vector2& scale)
{
	m_RootComponent->SetWorldScale(scale);
}

void CGameObject::SetWorldScale(float x, float y)
{
	m_RootComponent->SetWorldScale(x, y);
}

void CGameObject::SetWorldScale(float x, float y, float z)
{
	m_RootComponent->SetWorldScale(x, y, z);
}

void CGameObject::SetWorldScaleX(float x)
{
	m_RootComponent->SetWorldScaleX(x);
}

void CGameObject::SetWorldScaleY(float y)
{
	m_RootComponent->SetWorldScaleY(y);
}

void CGameObject::SetWorldScaleZ(float z)
{
	m_RootComponent->SetWorldScaleZ(z);
}

void CGameObject::SetWorldRotation(const Vector3& rot)
{
	m_RootComponent->SetWorldRotation(rot);
}

void CGameObject::SetWorldRotation(const Vector2& rot)
{
	m_RootComponent->SetWorldRotation(rot);
}

void CGameObject::SetWorldRotation(float x, float y)
{
	m_RootComponent->SetWorldRotation(x, y);
}

void CGameObject::SetWorldRotation(float x, float y, float z)
{
	m_RootComponent->SetWorldRotation(x, y, z);
}

void CGameObject::SetWorldRotationX(float x)
{
	m_RootComponent->SetWorldRotationX(x);
}

void CGameObject::SetWorldRotationY(float y)
{
	m_RootComponent->SetWorldRotationY(y);
}

void CGameObject::SetWorldRotationZ(float z)
{
	m_RootComponent->SetWorldRotationZ(z);
}

void CGameObject::SetWorldPosition(const Vector3& pos)
{
	m_RootComponent->SetWorldPosition(pos);
}

void CGameObject::SetWorldPosition(const Vector2& pos)
{
	m_RootComponent->SetWorldPosition(pos);
}

void CGameObject::SetWorldPosition(float x, float y)
{
	m_RootComponent->SetWorldPosition(x, y);
}

void CGameObject::SetWorldPosition(float x, float y, float z)
{
	m_RootComponent->SetWorldPosition(x, y, z);
}

void CGameObject::SetWorldPositionX(float x)
{
	m_RootComponent->SetWorldPositionX(x);
}

void CGameObject::SetWorldPositionY(float y)
{
	m_RootComponent->SetWorldPositionY(y);
}

void CGameObject::SetWorldPositionZ(float z)
{
	m_RootComponent->SetWorldPositionZ(z);
}

void CGameObject::SetOffset(const Vector3& offset)
{
	m_RootComponent->SetOffset(offset);
}

void CGameObject::SetOffset(const Vector2& offset)
{
	m_RootComponent->SetOffset(offset);
}

void CGameObject::SetOffsetX(float x)
{
	m_RootComponent->SetOffsetX(x);
}

void CGameObject::SetOffsetY(float y)
{
	m_RootComponent->SetOffsetY(y);
}

void CGameObject::SetOffsetZ(float z)
{
	m_RootComponent->SetOffsetZ(z);
}

void CGameObject::AddWorldScale(const Vector3& scale)
{
	m_RootComponent->AddWorldScale(scale);
}

void CGameObject::AddWorldScale(const Vector2& scale)
{
	m_RootComponent->AddWorldScale(scale);
}

void CGameObject::AddWorldScale(float x, float y)
{
	m_RootComponent->AddWorldScale(x, y);
}

void CGameObject::AddWorldScale(float x, float y, float z)
{
	m_RootComponent->AddWorldScale(x, y, z);
}

void CGameObject::AddWorldScaleX(float x)
{
	m_RootComponent->AddWorldScaleX(x);
}

void CGameObject::AddWorldScaleY(float y)
{
	m_RootComponent->AddWorldScaleY(y);
}

void CGameObject::AddWorldScaleZ(float z)
{
	m_RootComponent->AddWorldScaleZ(z);
}

void CGameObject::AddWorldRotation(const Vector3& rot)
{
	m_RootComponent->AddWorldRotation(rot);
}

void CGameObject::AddWorldRotation(const Vector2& rot)
{
	m_RootComponent->AddWorldRotation(rot);
}

void CGameObject::AddWorldRotation(float x, float y)
{
	m_RootComponent->AddWorldRotation(x, y);
}

void CGameObject::AddWorldRotation(float x, float y, float z)
{
	m_RootComponent->AddWorldRotation(x, y, z);
}

void CGameObject::AddWorldRotationX(float x)
{
	m_RootComponent->AddWorldRotationX(x);
}

void CGameObject::AddWorldRotationY(float y)
{
	m_RootComponent->AddWorldRotationY(y);
}

void CGameObject::AddWorldRotationZ(float z)
{
	m_RootComponent->AddWorldRotationZ(z);
}

void CGameObject::AddWorldPosition(const Vector3& pos)
{
	m_RootComponent->AddWorldPosition(pos);
}

void CGameObject::AddWorldPosition(const Vector2& pos)
{
	m_RootComponent->AddWorldPosition(pos);
}

void CGameObject::AddWorldPosition(float x, float y)
{
	m_RootComponent->AddWorldPosition(x, y);
}

void CGameObject::AddWorldPosition(float x, float y, float z)
{
	m_RootComponent->AddWorldPosition(x, y, z);
}

void CGameObject::AddWorldPositionX(float x)
{
	m_RootComponent->AddWorldPositionX(x);
}

void CGameObject::AddWorldPositionY(float y)
{
	m_RootComponent->AddWorldPositionY(y);
}

void CGameObject::AddWorldPositionZ(float z)
{
	m_RootComponent->AddWorldPositionZ(z);
}

void CGameObject::AddOffset(const Vector3& offset)
{
	m_RootComponent->AddOffset(offset);
}

void CGameObject::AddOffset(const Vector2& offset)
{
	m_RootComponent->AddOffset(offset);
}

void CGameObject::AddOffsetX(float x)
{
	m_RootComponent->AddOffsetX(x);
}

void CGameObject::AddOffsetY(float y)
{
	m_RootComponent->AddOffsetY(y);
}

void CGameObject::AddOffsetZ(float z)
{
	m_RootComponent->AddOffsetZ(z);
}
