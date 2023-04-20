#include "Component.h"
std::unordered_map<std::string, CComponent*> CComponent::m_mapComponent;

CComponent::CComponent()
	: m_ComponentType(ComponentType::Object)
	, m_Owner(nullptr)
	, m_Scene(nullptr)
	, m_SerialNumber(0)
{
	SetTypeID<CComponent>();
	m_ComponentTypeName = "Component";
}

CComponent::CComponent(const CComponent& obj)
	: CRef(obj)
	, m_Owner(nullptr)
	, m_Scene(nullptr)
{
	m_ComponentType = obj.m_ComponentType;
	m_ComponentTypeName = obj.m_ComponentTypeName;
	m_SerialNumber = obj.m_SerialNumber;
}

CComponent::~CComponent()
{
}

void CComponent::Destroy()
{
	CRef::Destroy();
}

void CComponent::Start()
{
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Update(float deltaTime)
{
}

void CComponent::PostUpdate(float deltaTime)
{
}

void CComponent::Render()
{
}

CComponent* CComponent::Clone() const
{
	return nullptr;
}

void CComponent::Save(FILE* file)
{
	CRef::Save(file);
	int	length = (int)m_ComponentTypeName.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_ComponentTypeName.c_str(), 1, length, file);
	fwrite(&m_ComponentType, sizeof(ComponentType), 1, file);
	fwrite(&m_SerialNumber, sizeof(int), 1, file);
}

void CComponent::Load(FILE* file)
{
	CRef::Load(file);
	int	length = 0;
	char	typeName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(typeName, 1, length, file);
	m_ComponentTypeName = typeName;
	fread(&m_ComponentType, sizeof(ComponentType), 1, file);
	fread(&m_SerialNumber, sizeof(int), 1, file);
}
