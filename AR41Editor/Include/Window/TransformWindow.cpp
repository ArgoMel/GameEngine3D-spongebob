#include "TransformWindow.h"
#include "Component/SceneComponent.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"

CTransformWindow::CTransformWindow()
	: m_Pos{}
	, m_Rot{}
	, m_Scale{}
{
}

CTransformWindow::~CTransformWindow()
{
}

void CTransformWindow::SetSelectComponent(CSceneComponent* component)
{
	m_SelectComponent = component;
}

void CTransformWindow::SetPos(const Vector3& pos)
{
	m_Pos[0]->SetFloat(pos.x);
	m_Pos[1]->SetFloat(pos.y);
	m_Pos[2]->SetFloat(pos.z);
}

void CTransformWindow::SetRotation(const Vector3& rot)
{
	m_Rot[0]->SetFloat(rot.x);
	m_Rot[1]->SetFloat(rot.y);
	m_Rot[2]->SetFloat(rot.z);
}

void CTransformWindow::SetScale(const Vector3& scale)
{
	m_Scale[0]->SetFloat(scale.x);
	m_Scale[1]->SetFloat(scale.y);
	m_Scale[2]->SetFloat(scale.z);
}

bool CTransformWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("위치");
	label->SetColor(29, 47, 73, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSizeY(30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_Pos[0] = CreateWidget<CEditorInputText>("PosX", 80.f, 30.f);
	m_Pos[0]->SetHideName("PosX");
	m_Pos[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Pos[1] = CreateWidget<CEditorInputText>("PosY", 80.f, 30.f);
	m_Pos[1]->SetHideName("PosY");
	m_Pos[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Pos[2] = CreateWidget<CEditorInputText>("PosZ", 80.f, 30.f);
	m_Pos[2]->SetHideName("PosZ");
	m_Pos[2]->SetInputType(EImGuiInputType::Float);
// ============== Rotation
	label = CreateWidget<CEditorLabel>("회전");
	label->SetColor(29, 47, 73, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSizeY(30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Rot[0] = CreateWidget<CEditorInputText>("RotX", 80.f, 30.f);
	m_Rot[0]->SetHideName("RotX");
	m_Rot[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Rot[1] = CreateWidget<CEditorInputText>("RotY", 80.f, 30.f);
	m_Rot[1]->SetHideName("RotY");
	m_Rot[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Rot[2] = CreateWidget<CEditorInputText>("RotZ", 80.f, 30.f);
	m_Rot[2]->SetHideName("RotZ");
	m_Rot[2]->SetInputType(EImGuiInputType::Float);
// ============== Scale
	label = CreateWidget<CEditorLabel>("크기");
	label->SetColor(29, 47, 73, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSizeY(30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Scale[0] = CreateWidget<CEditorInputText>("ScaleX", 80.f, 30.f);
	m_Scale[0]->SetHideName("ScaleX");
	m_Scale[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Scale[1] = CreateWidget<CEditorInputText>("ScaleY", 80.f, 30.f);
	m_Scale[1]->SetHideName("ScaleY");
	m_Scale[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Scale[2] = CreateWidget<CEditorInputText>("ScaleZ", 80.f, 30.f);
	m_Scale[2]->SetHideName("ScaleZ");
	m_Scale[2]->SetInputType(EImGuiInputType::Float);

	m_Pos[0]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::PosXCallback);
	m_Pos[1]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::PosYCallback);
	m_Pos[2]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::PosZCallback);
	m_Rot[0]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::RotXCallback);
	m_Rot[1]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::RotYCallback);
	m_Rot[2]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::RotZCallback);
	m_Scale[0]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::ScaleXCallback);
	m_Scale[1]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::ScaleYCallback);
	m_Scale[2]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::ScaleZCallback);
	return true;
}

void CTransformWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
		}
	}
}

void CTransformWindow::PosXCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativePositionX(m_Pos[0]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldPositionX(m_Pos[0]->GetFloat());
		}
	}
}

void CTransformWindow::PosYCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativePositionY(m_Pos[1]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldPositionY(m_Pos[1]->GetFloat());
		}
	}
}

void CTransformWindow::PosZCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativePositionZ(m_Pos[2]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldPositionZ(m_Pos[2]->GetFloat());
		}
	}
}

void CTransformWindow::RotXCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeRotationX(m_Rot[0]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldRotationX(m_Rot[0]->GetFloat());
		}
	}
}

void CTransformWindow::RotYCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeRotationY(m_Rot[1]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldRotationY(m_Rot[1]->GetFloat());
		}
	}
}

void CTransformWindow::RotZCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeRotationZ(m_Rot[2]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldRotationZ(m_Rot[2]->GetFloat());
		}
	}
}

void CTransformWindow::ScaleXCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeScaleX(m_Scale[0]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldScaleX(m_Scale[0]->GetFloat());
		}
	}
}

void CTransformWindow::ScaleYCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeScaleY(m_Scale[1]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldScaleY(m_Scale[1]->GetFloat());
		}
	}
}

void CTransformWindow::ScaleZCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeScaleZ(m_Scale[2]->GetFloat());
		}
		else
		{
			m_SelectComponent->SetWorldScaleZ(m_Scale[2]->GetFloat());
		}
	}
}

