#include "Gizmo.h"
#include "Input.h"
#include "Component/StaticMeshComponent.h"
#include "Component/TargetArm.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"

CGizmo::CGizmo()
	: m_Speed(1000.f)
{
	SetTypeID<CGizmo>();
	m_ObjectTypeName = "Gizmo";
}

CGizmo::CGizmo(const CGizmo& obj)
	: CGameObject(obj)
	, m_Speed(obj.m_Speed)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Arm = (CTargetArm*)FindComponent("Arm");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
}

CGizmo::~CGizmo()
{
}

void CGizmo::Destroy()
{
	CGameObject::Destroy();
	//CGameObject* player = m_Scene->GetPlayerObject();
	//if(player)
	//{
	//	player->Reset();
	//}
}

void CGizmo::Start()
{
	CGameObject::Start();
	CInput::GetInst()->AddBindFunction<CGizmo>("CtrlG", Input_Type::Down, this, &CGizmo::SetCamera, m_Scene);
	CInput::GetInst()->AddBindFunction<CGizmo>("UArrow", Input_Type::Stay, this, &CGizmo::UArrow, m_Scene);
	CInput::GetInst()->AddBindFunction<CGizmo>("DArrow", Input_Type::Stay, this, &CGizmo::DArrow, m_Scene);
	CInput::GetInst()->AddBindFunction<CGizmo>("LArrow", Input_Type::Stay, this, &CGizmo::LArrow, m_Scene);
	CInput::GetInst()->AddBindFunction<CGizmo>("RArrow", Input_Type::Stay, this, &CGizmo::RArrow, m_Scene);
	CInput::GetInst()->AddBindFunction<CGizmo>("PageUp", Input_Type::Stay, this, &CGizmo::PageUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CGizmo>("PageDown", Input_Type::Stay, this, &CGizmo::PageDown, m_Scene);
}

bool CGizmo::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_Mesh->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);

	m_Mesh->SetWorldScale(10.f, 10.f, 10.f);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	return true;
}

void CGizmo::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Vector2 mouseMove = CInput::GetInst()->GetMouseMove() * m_Camera->GetCameraSpeed() * g_DeltaTime * 0.2f;
	mouseMove.x = m_Camera->GetCameraHorizon() ? mouseMove.x : -mouseMove.x;
	mouseMove.y = m_Camera->GetCameraVertical() ? mouseMove.y : -mouseMove.y;
	m_Arm->AddRelativeRotationY(mouseMove.x);
	m_Arm->AddRelativeRotationX(mouseMove.y);
	if (m_Arm->GetRelativeRot().x > 90.f)
	{
		m_Arm->SetRelativeRotationX(90.f);
	}
	else if (m_Arm->GetRelativeRot().x < -90.f)
	{
		m_Arm->SetRelativeRotationX(-90.f);
	}
}

void CGizmo::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CGizmo* CGizmo::Clone() const
{
	return new CGizmo(*this);
}

void CGizmo::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CGizmo::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CGizmo::SetCamera()
{
	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
	m_Camera->SetCameraSpeed(400.f);
}

void CGizmo::UArrow()
{
	float angle = m_Camera->GetWorldRot().y;
	SetWorldRotationY(angle + 180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CGizmo::DArrow()
{
	float angle = m_Camera->GetWorldRot().y - 180.f;
	SetWorldRotationY(angle + 180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CGizmo::LArrow()
{
	float angle = m_Camera->GetWorldRot().y - 90.f;
	SetWorldRotationY(angle + 180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CGizmo::RArrow()
{
	float angle = m_Camera->GetWorldRot().y + 90.f;
	SetWorldRotationY(angle + 180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CGizmo::PageUp()
{
	AddWorldPositionY(m_Speed * g_DeltaTime);
}

void CGizmo::PageDown()
{
	AddWorldPositionY(-m_Speed * g_DeltaTime);
}
