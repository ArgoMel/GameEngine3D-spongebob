#include "OceanWindow.h"
//#include "Input.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorGUIManager.h"
#include "Component/OceanComponent.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"

COceanWindow::COceanWindow()
	: m_Grid(nullptr)
	, m_CountX(nullptr)
	, m_CountY(nullptr)
	, m_HeightMapX(nullptr)
	, m_HeightMapY(nullptr)
	, m_Height(nullptr)
	, m_CellSizeX(nullptr)
	, m_CellSizeY(nullptr)
	, m_SizeX(nullptr)
	, m_SizeY(nullptr)
	, m_DetailLevel(nullptr)
	, m_SplatCount(nullptr)
	, m_MeshName(nullptr)
	, m_Image(nullptr)
	, m_Dir(nullptr)
	, m_Time(nullptr)
	, m_MaxTime(nullptr)
	, m_WaveSpeed(nullptr)
{
}

COceanWindow::~COceanWindow()
{
}

void COceanWindow::SetSelectComponent(COceanComponent* component)
{
	m_SelectComponent = component;
	if (m_SelectComponent)
	{
		m_MeshName->SetText(m_SelectComponent->GetMesh()->GetName().c_str());

		std::string imgPath = m_MeshName->GetText();
		TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
		t_filename[imgPath.size()] = 0;
		std::copy(imgPath.begin(), imgPath.end(), t_filename);
		m_Image->SetTexture(imgPath, t_filename,ROOT_PATH);
		SAFE_DELETE_ARRAY(t_filename);

		m_Grid->SetCheck(m_SelectComponent->GetGrid());
		m_CountX->SetInt(m_SelectComponent->GetCountX());
		m_CountY->SetInt(m_SelectComponent->GetCountY());
		m_HeightMapX->SetInt(m_SelectComponent->GetHeightMapX());
		m_HeightMapY->SetInt(m_SelectComponent->GetHeightMapY());
		m_CellSizeX->SetFloat(m_SelectComponent->GetCellSize().x);
		m_CellSizeY->SetFloat(m_SelectComponent->GetCellSize().y);
		m_SizeX->SetFloat(m_SelectComponent->GetSize().x);
		m_SizeY->SetFloat(m_SelectComponent->GetSize().y);
		m_Height->SetFloat(m_SelectComponent->GetHeight());
		m_Dir->SetFloat(m_SelectComponent->GetDir());
		m_Time->SetFloat(m_SelectComponent->GetTime());
		m_MaxTime->SetFloat(m_SelectComponent->GetMaxTime());
		m_DetailLevel->SetFloat(m_SelectComponent->GetDetailLevel());
		m_SplatCount->SetInt(m_SelectComponent->GetSplatCount());
		m_WaveSpeed->SetFloat(m_SelectComponent->GetWaveSpeed());
	}
}

bool COceanWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("����");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_Grid = CreateWidget<CEditorCheckBox>("����");
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("����", 50.f, 30.f);
	button->SetClickCallback<COceanWindow>(this, &COceanWindow::ChangeOcean);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("����", 50.f, 30.f);
	button->SetClickCallback<COceanWindow>(this, &COceanWindow::ResetOcean);

	m_MeshName = CreateWidget<CEditorInputText>("�̸�", 300.f, 30.f);

	m_CountX = CreateWidget<CEditorInputText>("CountX", 100.f, 30.f);
	m_CountX->SetInputType(EImGuiInputType::Int);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(45.f);
	m_CountY = CreateWidget<CEditorInputText>("CountY", 100.f, 30.f);
	m_CountY->SetInputType(EImGuiInputType::Int);

	m_HeightMapX = CreateWidget<CEditorInputText>("HeightMapX", 100.f, 30.f);
	m_HeightMapX->SetInputType(EImGuiInputType::Int);
	m_HeightMapX->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("Line");
	m_HeightMapY = CreateWidget<CEditorInputText>("HeightMapY", 100.f, 30.f);
	m_HeightMapY->SetInputType(EImGuiInputType::Int);
	m_HeightMapY->ReadOnly(true);

	m_Height = CreateWidget<CEditorInputText>("Height", 80.f, 30.f);
	m_Height->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Dir = CreateWidget<CEditorInputText>("Dir", 100.f, 30.f);
	m_Dir->SetInputType(EImGuiInputType::Float);
	m_Dir->ReadOnly(true);

	m_CellSizeX = CreateWidget<CEditorInputText>("CellSizeX", 80.f, 30.f);
	m_CellSizeX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(48.f);
	m_CellSizeY = CreateWidget<CEditorInputText>("CellSizeY", 80.f, 30.f);
	m_CellSizeY->SetInputType(EImGuiInputType::Float);

	m_SizeX = CreateWidget<CEditorInputText>("SizeX", 80.f, 30.f);
	m_SizeX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(77.f);
	m_SizeY = CreateWidget<CEditorInputText>("SizeY", 80.f, 30.f);
	m_SizeY->SetInputType(EImGuiInputType::Float);

	m_Time = CreateWidget<CEditorInputText>("Time", 80.f, 30.f);
	m_Time->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_MaxTime = CreateWidget<CEditorInputText>("MaxTime", 100.f, 30.f);
	m_MaxTime->SetInputType(EImGuiInputType::Float);

	m_DetailLevel = CreateWidget<CEditorInputText>("DetailLevel", 80.f, 30.f);
	m_DetailLevel->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(33.f);
	m_SplatCount = CreateWidget<CEditorInputText>("SplatCount", 100.f, 30.f);
	m_SplatCount->SetInputType(EImGuiInputType::Int);

	m_WaveSpeed	= CreateWidget<CEditorInputText>("WaveSpeed", 100.f, 30.f);
	m_WaveSpeed->SetInputType(EImGuiInputType::Float);

	m_Image = CreateWidget<CEditorImage>("Texture");
	m_Image->SetSize(150.f, 150.f);
	m_Image->SetHideName("Texture");
	m_Image->SetIsFixed(true);
	return true;
}

void COceanWindow::Update(float deltaTime)
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

void COceanWindow::SetImage(const std::string& name, const TCHAR* path)
{
	m_Image->SetTextureFullPath(name, path);
	m_CountX->SetFloat((float)m_Image->GetImgWidth());
	m_CountY->SetFloat((float)m_Image->GetImgHeight());
	m_MeshName->SetText(m_Image->GetTexture()->GetName().c_str());
}

void COceanWindow::ChangeOcean()
{
	if (!m_SelectComponent)
	{
		return;
	}
	m_SelectComponent->SetDetailLevel(m_DetailLevel->GetFloat());
	m_SelectComponent->SetSplatCount(m_SplatCount->GetInt());
	m_SelectComponent->SetTime(m_Time->GetFloat());
	m_SelectComponent->SetMaxTime(m_MaxTime->GetFloat());
	m_SelectComponent->SetWaveSpeed(m_WaveSpeed->GetFloat());
}

void COceanWindow::ResetOcean()
{
	if (!m_SelectComponent)
	{
		return;
	}
	m_SelectComponent->SetDetailLevel(m_DetailLevel->GetFloat());
	m_SelectComponent->SetSplatCount(m_SplatCount->GetInt());
	m_SelectComponent->SetTime(m_Time->GetFloat());
	m_SelectComponent->SetMaxTime(m_MaxTime->GetFloat());
	m_SelectComponent->SetWaveSpeed(m_WaveSpeed->GetFloat());
	m_SelectComponent->SetHeight(m_Height->GetFloat());
	m_SelectComponent->DeleteOcean();
	m_SelectComponent->CreateOcean(m_CountX->GetInt(), m_CountY->GetInt(), m_CellSizeX->GetFloat(),
		m_CellSizeY->GetFloat(), m_Image->GetTexture()->GetFullPath());
}
