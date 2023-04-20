#include "BillboardComponent.h"
#include "CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"

//����Ʈ ��ƼŬ������
// �׻� ī�޶� �Ĵٺ��� �Ҷ� ���
//�ܵ����� ������(�ڽ��� ������ ȸ�������� ��������)

CBillboardComponent::CBillboardComponent()
{
	SetTypeID<CBillboardComponent>();
	m_ComponentTypeName = "BillboardComponent";
	SetRenderLayerName("Particle");
}

CBillboardComponent::CBillboardComponent(const CBillboardComponent& component)
	: CPrimitiveComponent(component)
{
}

CBillboardComponent::~CBillboardComponent()
{
}

void CBillboardComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CBillboardComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}
	SetMesh("CenterUVRect");
	SetMaterial(0, "Billboard");
	SetRelativeScale(500.f, 500.f, 1.f);
	return true;
}

void CBillboardComponent::Update(float deltaTime)
{
	CPrimitiveComponent::Update(deltaTime);
	Vector3	cameraPos = m_Scene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();
	Vector3	view = cameraPos - GetWorldPos();
	view.Normalize();
	Vector3	originDir(0.f, 0.f, -1.f);
	m_Transform->SetWorldRotationAxis(originDir, view);
}

void CBillboardComponent::PostUpdate(float deltaTime)
{
	CPrimitiveComponent::PostUpdate(deltaTime);
}

void CBillboardComponent::Render()
{
	CPrimitiveComponent::Render();
}

CBillboardComponent* CBillboardComponent::Clone() const
{
	return new CBillboardComponent(*this);
}

void CBillboardComponent::Save(FILE* file)
{
	CPrimitiveComponent::Save(file);
}

void CBillboardComponent::Load(FILE* file)
{
	CPrimitiveComponent::Load(file);
}
