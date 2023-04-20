#include "CameraWindow.h"
#include "Device.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorComboBox.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"

CCameraWindow::CCameraWindow()
	: m_CameraCB(nullptr)
	, m_CameraViewDistance(nullptr)
	, m_CameraHorizon(nullptr)
	, m_CameraVertical(nullptr)
	, m_CameraSpeed(nullptr)
{
}

CCameraWindow::~CCameraWindow()
{
}

void CCameraWindow::SetSelectComponent(CSceneComponent* component)
{
	m_SelectCamera = (CCameraComponent*)component;
	m_CameraCB->SetSelectIndex((int)m_SelectCamera->GetCameraType());
	m_CameraHorizon->SetCheck(m_SelectCamera->GetCameraHorizon());
	m_CameraVertical->SetCheck(m_SelectCamera->GetCameraVertical());
	m_CameraViewDistance->SetFloat(m_SelectCamera->GetCameraViewDistance());
	m_CameraSpeed->SetFloat(m_SelectCamera->GetCameraSpeed());
}

bool CCameraWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("카메라");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CCameraWindow>(this, &CCameraWindow::CameraChangeCallback);
	m_CameraCB = CreateWidget<CEditorComboBox>("CameraType", 110.f);
	m_CameraCB->AddItem("Camera2D");
	m_CameraCB->AddItem("Camera3D");
	m_CameraCB->AddItem("CameraUI");
	m_CameraCB->SetSelectPrevViewName(true);
	m_CameraCB->SetSelectIndex(0);

	m_CameraHorizon = CreateWidget<CEditorCheckBox>("CameraHorizon", 90.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	m_CameraVertical = CreateWidget<CEditorCheckBox>("CameraVertical", 90.f, 30.f);

	m_CameraViewDistance = CreateWidget<CEditorInputText>("CameraViewDistance", 90.f, 30.f);
	m_CameraViewDistance->SetInputType(EImGuiInputType::Float);
	m_CameraSpeed = CreateWidget<CEditorInputText>("CameraSpeed", 90.f, 30.f);
	m_CameraSpeed->SetInputType(EImGuiInputType::Float);
	return true;
}

void CCameraWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectCamera)
	{
		if (!m_SelectCamera->GetActive())
		{
			m_SelectCamera = nullptr;
		}
	}
}

void CCameraWindow::CameraChangeCallback()
{
	if (!m_SelectCamera)
	{
		return;
	}
	m_SelectCamera->SetCameraViewDistance(m_CameraViewDistance->GetFloat());
	m_SelectCamera->SetCameraType((ECameraType)m_CameraCB->GetSelectIndex());
	m_SelectCamera->SetCameraHorizon(m_CameraHorizon->GetCheck());
	m_SelectCamera->SetCameraVertical(m_CameraVertical->GetCheck());
	m_SelectCamera->SetCameraSpeed(m_CameraSpeed->GetFloat());
}
