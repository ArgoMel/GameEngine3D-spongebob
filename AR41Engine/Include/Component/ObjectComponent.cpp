#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{
	m_ComponentType = ComponentType::Object;
	SetTypeID<CObjectComponent>();
	m_ComponentTypeName = "ObjectComponent";
}

CObjectComponent::CObjectComponent(const CObjectComponent& obj)
	: CComponent(obj)
{
}

CObjectComponent::~CObjectComponent()
{
}

void CObjectComponent::Destroy()
{
	CComponent::Destroy();
}

void CObjectComponent::Start()
{
	CComponent::Start();
}

bool CObjectComponent::Init()
{
	if (!CComponent::Init())
	{
		return false;
	}
	return true;
}

void CObjectComponent::Update(float deltaTime)
{
	CComponent::Update(deltaTime);
}

void CObjectComponent::PostUpdate(float deltaTime)
{
	CComponent::PostUpdate(deltaTime);
}

void CObjectComponent::Render()
{
	CComponent::Render();
}

CObjectComponent* CObjectComponent::Clone() const
{
	return nullptr;
}

void CObjectComponent::Save(FILE* file)
{
	CComponent::Save(file);
}

void CObjectComponent::Load(FILE* file)
{
	CComponent::Load(file);
}
