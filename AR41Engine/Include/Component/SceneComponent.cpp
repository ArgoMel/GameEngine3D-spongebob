#include "SceneComponent.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Animation/SkeletonSocket.h"
#include "../Scene/Scene.h"
#include "../Resource/Shader/GraphicShader.h"

CSceneComponent::CSceneComponent()
	: m_Parent(nullptr)
	, m_Socket(nullptr)
	, m_FrustumCull(false)
	, m_LayerName("Default")
	, m_SceneComponentType(SceneComponentType::Scene)
{
	SetTypeID<CSceneComponent>();
	m_Transform = new CTransform;
	m_Transform->Init();
	m_Transform->m_Owner = this;
	m_ComponentType = ComponentType::Scene;
	m_ComponentTypeName = "SceneComponent";
}

CSceneComponent::CSceneComponent(const CSceneComponent& component)
	: CComponent(component)
{
	m_Socket = nullptr;
	m_SceneComponentType = component.m_SceneComponentType;
	m_LayerName = component.m_LayerName;
	m_Transform = component.m_Transform->Clone();
	m_Transform->m_Parent = nullptr;
	m_Transform->m_vecChild.clear();
	m_Transform->m_Owner = this;
	m_Parent = nullptr;
	m_vecChild.clear();
	size_t	size = component.m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		CSceneComponent* childCom = component.m_vecChild[i]->Clone();
		AddChild(childCom);
	}
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_Transform);
}

void CSceneComponent::AddOwner()
{
	m_Owner->AddSceneComponent(this);
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddOwner();
	}
}

void CSceneComponent::SetSocket(CSkeletonSocket* socket)
{
	m_Socket = socket;
	m_Transform->SetSocket(m_Socket);
}

void CSceneComponent::SetScene(CScene* scene)
{
	CComponent::SetScene(scene);
	m_Transform->m_Scene = scene;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetScene(scene);
	}
}

void CSceneComponent::SetOwner(CGameObject* owner)
{
	CComponent::SetOwner(owner);
	m_Transform->m_Object = owner;
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetOwner(owner);
	}
}

void CSceneComponent::AddChild(CSceneComponent* child, const std::string& socketName)
{
	child->m_Parent = this;
	child->m_ParentName = m_Name;
	m_vecChild.push_back(child);
	m_vecChildName.push_back(child->GetName());
	child->m_Transform->m_Parent = m_Transform;
	m_Transform->m_vecChild.push_back(child->m_Transform);
}

void CSceneComponent::AddChild(CGameObject* child, const std::string& socketName)
{
	CSceneComponent* childComponent = child->GetRootComponent();
	childComponent->m_Parent = this;
	childComponent->m_ParentName = m_Name;
	m_vecChild.push_back(childComponent);
	m_vecChildName.push_back(childComponent->GetName());
	childComponent->m_Transform->m_Parent = m_Transform;
	m_Transform->m_vecChild.push_back(childComponent->m_Transform);
	m_Owner->AddSceneComponent(childComponent);
}

bool CSceneComponent::DeleteChild(CSceneComponent* child)
{
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecChild[i] == child)
		{
			auto	iter = m_vecChild.begin() + i;
			auto	iterName = m_vecChildName.begin() + i;
			(*iter)->m_Parent = nullptr;
			(*iter)->m_ParentName = "";
			m_Owner->DeleteSceneComponent(*iter);
			m_vecChild.erase(iter);
			m_vecChildName.erase(iterName);
			auto	iterTr = m_Transform->m_vecChild.begin() + i;
			(*iterTr)->m_Parent = nullptr;
			m_Transform->m_vecChild.erase(iterTr);
			return true;
		}
	}
	return false;
}

bool CSceneComponent::DeleteChild(const std::string& name)
{
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecChild[i]->GetName() == name)
		{
			auto	iter = m_vecChild.begin() + i;
			auto	iterName = m_vecChildName.begin() + i;
			(*iter)->m_Parent = nullptr;
			(*iter)->m_ParentName = "";
			m_Owner->DeleteSceneComponent(*iter);
			m_vecChild.erase(iter);
			m_vecChildName.erase(iterName);
			auto	iterTr = m_Transform->m_vecChild.begin() + i;
			(*iterTr)->m_Parent = nullptr;
			m_Transform->m_vecChild.erase(iterTr);
			return true;
		}
	}
	return false;
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& name)
{
	if (m_Name == name)
	{
		return this;
	}
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		CSceneComponent* find = m_vecChild[i]->FindComponent(name);
		if (find)
		{
			return find;
		}
	}
	return nullptr;
}

void CSceneComponent::GetAllComponentHierarchyName(std::vector<HierarchyName>& vecName)
{
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		HierarchyName	names;
		CSceneComponent* parent = m_vecChild[i]->GetParent();
		names.name = m_vecChild[i]->GetName();
		names.className = m_vecChild[i]->GetComponentTypeName();
		names.component = m_vecChild[i];
		names.parent = parent;
		if (parent)
		{
			names.parentName = parent->GetName();
			names.parentClassName = parent->GetComponentTypeName();
		}
		vecName.push_back(names);
	}
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->GetAllComponentHierarchyName(vecName);
	}
}

void CSceneComponent::Destroy()
{
	CComponent::Destroy();
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->Destroy();
	}
}

void CSceneComponent::Start()
{
	CComponent::Start();
	m_Transform->Start();
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->Start();
	}
}

bool CSceneComponent::Init()
{
	if (!CComponent::Init())
	{
		return false;
	}
	if (m_Scene)
	{
		m_SceneName = m_Scene->GetName();
	}
	return true;
}

void CSceneComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);
	m_Transform->Update(deltaTime);
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->Update(deltaTime);
	}
}

void CSceneComponent::PostUpdate(float deltaTime)
{
	CComponent::PostUpdate(deltaTime);
	m_Transform->PostUpdate(deltaTime);
	size_t	size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->PostUpdate(deltaTime);
	}
}

void CSceneComponent::Render()
{
	CComponent::Render();
	m_Transform->SetTransform();
}

void CSceneComponent::RenderShadowMap()
{
	m_Transform->SetShadowMapTransform();
	if (m_ShadowMapShader)
	{
		m_ShadowMapShader->SetShader();
	}
}

CSceneComponent* CSceneComponent::Clone() const
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Save(FILE* file)
{
	CComponent::Save(file);
	int	length = (int)m_LayerName.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_LayerName.c_str(), 1, length, file);
	int	childCount = (int)m_vecChild.size();
	fwrite(&childCount, 4, 1, file);
	for (int i = 0; i < childCount; ++i)
	{
		length = (int)m_vecChild[i]->GetComponentTypeName().length();
		fwrite(&length, 4, 1, file);
		fwrite(m_vecChild[i]->GetComponentTypeName().c_str(), 1, length, file);
		m_vecChild[i]->Save(file);
	}
	m_Transform->Save(file);
}

void CSceneComponent::Load(FILE* file)
{
	CComponent::Load(file);
	int	length = 0;
	char	layerName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(layerName, 1, length, file);
	m_LayerName = layerName;
	int	childCount = 0;
	fread(&childCount, 4, 1, file);
	if (m_vecChild.empty())
	{
		for (int i = 0; i < childCount; ++i)
		{
			length = 0;
			char	typeName[256] = {};
			fread(&length, 4, 1, file);
			assert(length >= 0);
			fread(typeName, 1, length, file);
			CComponent* CDO = CComponent::FindCDO(typeName);
			CSceneComponent* component = (CSceneComponent*)CDO->Clone();
			component->SetOwner(m_Owner);
			component->SetScene(m_Scene);
			AddChild(component);
			component->Load(file);
		}
	}
	else
	{
		for (int i = 0; i < childCount; ++i)
		{
			length = 0;
			char	typeName[256] = {};
			fread(&length, 4, 1, file);
			assert(length >= 0);
			fread(typeName, 1, length, file);
			m_vecChild[i]->SetScene(m_Scene);
			m_vecChild[i]->Load(file);
		}
	}
	m_Transform->Load(file);
}

void CSceneComponent::SetInheritScale(bool inherit)
{
	m_Transform->SetInheritScale(inherit);
}

void CSceneComponent::SetInheritRotX(bool inherit)
{
	m_Transform->SetInheritRotX(inherit);
}

void CSceneComponent::SetInheritRotY(bool inherit)
{
	m_Transform->SetInheritRotY(inherit);
}

void CSceneComponent::SetInheritRotZ(bool inherit)
{
	m_Transform->SetInheritRotZ(inherit);
}

void CSceneComponent::SetInheritParentRotationPosX(bool inherit)
{
	m_Transform->SetInheritParentRotationPosX(inherit);
}

void CSceneComponent::SetInheritParentRotationPosY(bool inherit)
{
	m_Transform->SetInheritParentRotationPosY(inherit);
}

void CSceneComponent::SetInheritParentRotationPosZ(bool inherit)
{
	m_Transform->SetInheritParentRotationPosZ(inherit);
}

void CSceneComponent::InheritRotation(bool current)
{
	m_Transform->InheritRotation(current);
}

void CSceneComponent::InheritParentRotationPos()
{
	m_Transform->InheritParentRotationPos();
}

void CSceneComponent::InheritWorldRotation(bool current)
{
	m_Transform->InheritWorldRotation(current);
}

void CSceneComponent::InheritWorldParentRotationPos()
{
	m_Transform->InheritWorldParentRotationPos();
}

Vector3 CSceneComponent::GetRelativeScale() const
{
	return m_Transform->GetRelativeScale();
}

Vector3 CSceneComponent::GetRelativeRot() const
{
	return m_Transform->GetRelativeRot();
}

Vector3 CSceneComponent::GetRelativePos() const
{
	return m_Transform->GetRelativePos();
}

Vector3 CSceneComponent::GetRelativeAxis(AXIS axis) const
{
	return m_Transform->GetRelativeAxis(axis);
}

void CSceneComponent::SetRelativeScale(const Vector3& scale)
{
	m_Transform->SetRelativeScale(scale);
}

void CSceneComponent::SetRelativeScale(const Vector2& scale)
{
	m_Transform->SetRelativeScale(scale);
}

void CSceneComponent::SetRelativeScale(float x, float y)
{
	m_Transform->SetRelativeScale(x, y);
}

void CSceneComponent::SetRelativeScale(float x, float y, float z)
{
	m_Transform->SetRelativeScale(x, y, z);
}

void CSceneComponent::SetRelativeScaleX(float x)
{
	m_Transform->SetRelativeScaleX(x);
}

void CSceneComponent::SetRelativeScaleY(float y)
{
	m_Transform->SetRelativeScaleY(y);
}

void CSceneComponent::SetRelativeScaleZ(float z)
{
	m_Transform->SetRelativeScaleZ(z);
}

void CSceneComponent::SetRelativeRotation(const Vector3& rot)
{
	m_Transform->SetRelativeRotation(rot);
}

void CSceneComponent::SetRelativeRotation(const Vector2& rot)
{
	m_Transform->SetRelativeRotation(rot);
}

void CSceneComponent::SetRelativeRotation(float x, float y)
{
	m_Transform->SetRelativeRotation(x, y);
}

void CSceneComponent::SetRelativeRotation(float x, float y, float z)
{
	m_Transform->SetRelativeRotation(x, y, z);
}

void CSceneComponent::SetRelativeRotationX(float x)
{
	m_Transform->SetRelativeRotationX(x);
}

void CSceneComponent::SetRelativeRotationY(float y)
{
	m_Transform->SetRelativeRotationY(y);
}

void CSceneComponent::SetRelativeRotationZ(float z)
{
	m_Transform->SetRelativeRotationZ(z);
}

void CSceneComponent::SetRelativePosition(const Vector3& pos)
{
	m_Transform->SetRelativePosition(pos);
}

void CSceneComponent::SetRelativePosition(const Vector2& pos)
{
	m_Transform->SetRelativePosition(pos);
}

void CSceneComponent::SetRelativePosition(float x, float y)
{
	m_Transform->SetRelativePosition(x, y);
}

void CSceneComponent::SetRelativePosition(float x, float y, float z)
{
	m_Transform->SetRelativePosition(x, y, z);
}

void CSceneComponent::SetRelativePositionX(float x)
{
	m_Transform->SetRelativePositionX(x);
}

void CSceneComponent::SetRelativePositionY(float y)
{
	m_Transform->SetRelativePositionY(y);
}

void CSceneComponent::SetRelativePositionZ(float z)
{
	m_Transform->SetRelativePositionZ(z);
}

void CSceneComponent::AddRelativeScale(const Vector3& scale)
{
	m_Transform->AddRelativeScale(scale);
}

void CSceneComponent::AddRelativeScale(const Vector2& scale)
{
	m_Transform->AddRelativeScale(scale);
}

void CSceneComponent::AddRelativeScale(float x, float y)
{
	m_Transform->AddRelativeScale(x, y);
}

void CSceneComponent::AddRelativeScale(float x, float y, float z)
{
	m_Transform->AddRelativeScale(x, y, z);
}

void CSceneComponent::AddRelativeScaleX(float x)
{
	m_Transform->AddRelativeScaleX(x);
}

void CSceneComponent::AddRelativeScaleY(float y)
{
	m_Transform->AddRelativeScaleY(y);
}

void CSceneComponent::AddRelativeScaleZ(float z)
{
	m_Transform->AddRelativeScaleZ(z);
}

void CSceneComponent::AddRelativeRotation(const Vector3& rot)
{
	m_Transform->AddRelativeRotation(rot);
}

void CSceneComponent::AddRelativeRotation(const Vector2& rot)
{
	m_Transform->AddRelativeRotation(rot);
}

void CSceneComponent::AddRelativeRotation(float x, float y)
{
	m_Transform->AddRelativeRotation(x, y);
}

void CSceneComponent::AddRelativeRotation(float x, float y, float z)
{
	m_Transform->AddRelativeRotation(x, y, z);
}

void CSceneComponent::AddRelativeRotationX(float x)
{
	m_Transform->AddRelativeRotationX(x);
}

void CSceneComponent::AddRelativeRotationY(float y)
{
	m_Transform->AddRelativeRotationY(y);
}

void CSceneComponent::AddRelativeRotationZ(float z)
{
	m_Transform->AddRelativeRotationZ(z);
}

void CSceneComponent::AddRelativePosition(const Vector3& pos)
{
	m_Transform->AddRelativePosition(pos);
}

void CSceneComponent::AddRelativePosition(const Vector2& pos)
{
	m_Transform->AddRelativePosition(pos);
}

void CSceneComponent::AddRelativePosition(float x, float y)
{
	m_Transform->AddRelativePosition(x, y);
}

void CSceneComponent::AddRelativePosition(float x, float y, float z)
{
	m_Transform->AddRelativePosition(x, y, z);
}

void CSceneComponent::AddRelativePositionX(float x)
{
	m_Transform->AddRelativePositionX(x);
}

void CSceneComponent::AddRelativePositionY(float y)
{
	m_Transform->AddRelativePositionY(y);
}

void CSceneComponent::AddRelativePositionZ(float z)
{
	m_Transform->AddRelativePositionZ(z);
}

const Vector3& CSceneComponent::GetCenter() const
{
	return m_Transform->GetCenter();
}

const Vector3& CSceneComponent::GetMin() const
{
	return m_Transform->GetMin();
}

const Vector3& CSceneComponent::GetMax() const
{
	return m_Transform->GetMax();
}

float CSceneComponent::GetRadius() const
{
	return m_Transform->GetRadius();
}

const Vector3& CSceneComponent::GetWorldScale() const
{
	return m_Transform->GetWorldScale();
}

const Vector3& CSceneComponent::GetWorldRot() const
{
	return m_Transform->GetWorldRot();
}

const Vector3& CSceneComponent::GetWorldPos() const
{
	return m_Transform->GetWorldPos();
}

const Vector3& CSceneComponent::GetWorldAxis(AXIS axis) const
{
	return m_Transform->GetWorldAxis(axis);
}

const Vector3& CSceneComponent::GetPivot() const
{
	return m_Transform->GetPivot();
}

const Vector3& CSceneComponent::GetMeshSize() const
{
	return m_Transform->GetMeshSize();
}

const Vector3& CSceneComponent::GetOffset() const
{
	return m_Transform->GetOffset();
}

const Matrix& CSceneComponent::GetWorldMatrix() const
{
	return m_Transform->GetWorldMatrix();
}

void CSceneComponent::SetPivot(const Vector3& pivot)
{
	m_Transform->SetPivot(pivot);
}

void CSceneComponent::SetPivot(const Vector2& pivot)
{
	m_Transform->SetPivot(pivot);
}

void CSceneComponent::SetPivot(float x, float y, float z)
{
	m_Transform->SetPivot(x, y, z);
}

void CSceneComponent::SetPivot(float x, float y)
{
	m_Transform->SetPivot(x, y);
}

void CSceneComponent::SetMin(const Vector3& min)
{
	m_Transform->SetMin(min);
}

void CSceneComponent::SetMax(const Vector3& max)
{
	m_Transform->SetMax(max);
}

void CSceneComponent::SetMeshSize(const Vector3& meshSize)
{
	m_Transform->SetMeshSize(meshSize);
}

void CSceneComponent::SetMeshSize(const Vector2& meshSize)
{
	m_Transform->SetMeshSize(meshSize);
}

void CSceneComponent::SetMeshSize(float x, float y, float z)
{
	m_Transform->SetMeshSize(x, y, z);
}

void CSceneComponent::SetMeshSize(float x, float y)
{
	m_Transform->SetMeshSize(x, y);
}

void CSceneComponent::SetWorldScale(const Vector3& scale)
{
	m_Transform->SetWorldScale(scale);
}

void CSceneComponent::SetWorldScale(const Vector2& scale)
{
	m_Transform->SetWorldScale(scale);
}

void CSceneComponent::SetWorldScale(float x, float y)
{
	m_Transform->SetWorldScale(x, y);
}

void CSceneComponent::SetWorldScale(float x, float y, float z)
{
	m_Transform->SetWorldScale(x, y, z);
}

void CSceneComponent::SetWorldScaleX(float x)
{
	m_Transform->SetWorldScaleX(x);
}

void CSceneComponent::SetWorldScaleY(float y)
{
	m_Transform->SetWorldScaleY(y);
}

void CSceneComponent::SetWorldScaleZ(float z)
{
	m_Transform->SetWorldScaleZ(z);
}

void CSceneComponent::SetWorldRotation(const Vector3& rot)
{
	m_Transform->SetWorldRotation(rot);
}

void CSceneComponent::SetWorldRotation(const Vector2& rot)
{
	m_Transform->SetWorldRotation(rot);
}

void CSceneComponent::SetWorldRotation(float x, float y)
{
	m_Transform->SetWorldRotation(x, y);
}

void CSceneComponent::SetWorldRotation(float x, float y, float z)
{
	m_Transform->SetWorldRotation(x, y, z);
}

void CSceneComponent::SetWorldRotationX(float x)
{
	m_Transform->SetWorldRotationX(x);
}

void CSceneComponent::SetWorldRotationY(float y)
{
	m_Transform->SetWorldRotationY(y);
}

void CSceneComponent::SetWorldRotationZ(float z)
{
	m_Transform->SetWorldRotationZ(z);
}

void CSceneComponent::SetWorldPosition(const Vector3& pos)
{
	m_Transform->SetWorldPosition(pos);
}

void CSceneComponent::SetWorldPosition(const Vector2& pos)
{
	m_Transform->SetWorldPosition(pos);
}

void CSceneComponent::SetWorldPosition(float x, float y)
{
	m_Transform->SetWorldPosition(x, y);
}

void CSceneComponent::SetWorldPosition(float x, float y, float z)
{
	m_Transform->SetWorldPosition(x, y, z);
}

void CSceneComponent::SetWorldPositionX(float x)
{
	m_Transform->SetWorldPositionX(x);
}

void CSceneComponent::SetWorldPositionY(float y)
{
	m_Transform->SetWorldPositionY(y);
}

void CSceneComponent::SetWorldPositionZ(float z)
{
	m_Transform->SetWorldPositionZ(z);
}

void CSceneComponent::SetOffset(const Vector3& offset)
{
	m_Transform->SetOffset(offset);
}

void CSceneComponent::SetOffset(const Vector2& offset)
{
	m_Transform->SetOffset(offset);
}

void CSceneComponent::SetOffsetX(float x)
{
	m_Transform->SetOffsetX(x);
}

void CSceneComponent::SetOffsetY(float y)
{
	m_Transform->SetOffsetY(y);
}

void CSceneComponent::SetOffsetZ(float z)
{
	m_Transform->SetOffsetZ(z);
}

void CSceneComponent::AddWorldScale(const Vector3& scale)
{
	m_Transform->AddWorldScale(scale);
}

void CSceneComponent::AddWorldScale(const Vector2& scale)
{
	m_Transform->AddWorldScale(scale);
}

void CSceneComponent::AddWorldScale(float x, float y)
{
	m_Transform->AddWorldScale(x, y);
}

void CSceneComponent::AddWorldScale(float x, float y, float z)
{
	m_Transform->AddWorldScale(x, y, z);
}

void CSceneComponent::AddWorldScaleX(float x)
{
	m_Transform->AddWorldScaleX(x);
}

void CSceneComponent::AddWorldScaleY(float y)
{
	m_Transform->AddWorldScaleY(y);
}

void CSceneComponent::AddWorldScaleZ(float z)
{
	m_Transform->AddWorldScaleZ(z);
}

void CSceneComponent::AddWorldRotation(const Vector3& rot)
{
	m_Transform->AddWorldRotation(rot);
}

void CSceneComponent::AddWorldRotation(const Vector2& rot)
{
	m_Transform->AddWorldRotation(rot);
}

void CSceneComponent::AddWorldRotation(float x, float y)
{
	m_Transform->AddWorldRotation(x, y);
}

void CSceneComponent::AddWorldRotation(float x, float y, float z)
{
	m_Transform->AddWorldRotation(x, y, z);
}

void CSceneComponent::AddWorldRotationX(float x)
{
	m_Transform->AddWorldRotationX(x);
}

void CSceneComponent::AddWorldRotationY(float y)
{
	m_Transform->AddWorldRotationY(y);
}

void CSceneComponent::AddWorldRotationZ(float z)
{
	m_Transform->AddWorldRotationZ(z);
}

void CSceneComponent::AddWorldPosition(const Vector3& pos)
{
	m_Transform->AddWorldPosition(pos);
}

void CSceneComponent::AddWorldPosition(const Vector2& pos)
{
	m_Transform->AddWorldPosition(pos);
}

void CSceneComponent::AddWorldPosition(float x, float y)
{
	m_Transform->AddWorldPosition(x, y);
}

void CSceneComponent::AddWorldPosition(float x, float y, float z)
{
	m_Transform->AddWorldPosition(x, y, z);
}

void CSceneComponent::AddWorldPositionX(float x)
{
	m_Transform->AddWorldPositionX(x);
}

void CSceneComponent::AddWorldPositionY(float y)
{
	m_Transform->AddWorldPositionY(y);
}

void CSceneComponent::AddWorldPositionZ(float z)
{
	m_Transform->AddWorldPositionZ(z);
}

void CSceneComponent::AddOffset(const Vector3& offset)
{
	m_Transform->AddOffset(offset);
}

void CSceneComponent::AddOffset(const Vector2& offset)
{
	m_Transform->AddOffset(offset);
}

void CSceneComponent::AddOffsetX(float x)
{
	m_Transform->AddOffsetX(x);
}

void CSceneComponent::AddOffsetY(float y)
{
	m_Transform->AddOffsetY(y);
}

void CSceneComponent::AddOffsetZ(float z)
{
	m_Transform->AddOffsetZ(z);
}
