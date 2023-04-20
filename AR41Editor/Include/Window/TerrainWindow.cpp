#include "TerrainWindow.h"
#include "Input.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorGUIManager.h"
#include "Component/TerrainComponent.h"
#include "Scene/SceneManager.h"

CTerrainWindow::CTerrainWindow()
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
{
}

CTerrainWindow::~CTerrainWindow()
{
}

void CTerrainWindow::SetSelectComponent(CTerrainComponent* component)
{
	m_SelectComponent = component;
	if (m_SelectComponent)
	{
		m_MeshName->SetText(m_SelectComponent->GetMesh()->GetName().c_str());

		std::string imgPath = m_MeshName->GetText();
		TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
		t_filename[imgPath.size()] = 0;
		std::copy(imgPath.begin(), imgPath.end(), t_filename);
		m_Image->SetTexture(imgPath, t_filename, ROOT_PATH);
		SAFE_DELETE_ARRAY(t_filename);

		m_Grid->SetCheck(m_SelectComponent->GetGrid());
		m_CountX->SetInt(m_SelectComponent->GetCountX());
		m_CountY->SetInt(m_SelectComponent->GetCountY());
		m_HeightMapX->SetInt(m_SelectComponent->GetHeightMapX());
		m_HeightMapY->SetInt(m_SelectComponent->GetHeightMapY());
		m_Height->SetFloat(m_SelectComponent->GetHeight());
		m_CellSizeX->SetFloat(m_SelectComponent->GetCellSize().x);
		m_CellSizeY->SetFloat(m_SelectComponent->GetCellSize().y);
		m_SizeX->SetFloat(m_SelectComponent->GetSize().x);
		m_SizeY->SetFloat(m_SelectComponent->GetSize().y);
		m_DetailLevel->SetFloat(m_SelectComponent->GetDetailLevel());
		m_SplatCount->SetInt(m_SelectComponent->GetSplatCount());
	}
}

bool CTerrainWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("터레인");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_Grid = CreateWidget<CEditorCheckBox>("격자");
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CTerrainWindow>(this, &CTerrainWindow::ChangeTerrain);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("리셋", 50.f, 30.f);
	button->SetClickCallback<CTerrainWindow>(this, &CTerrainWindow::ResetTerrain);

	m_MeshName = CreateWidget<CEditorInputText>("이름", 300.f, 30.f);

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

	m_DetailLevel = CreateWidget<CEditorInputText>("DetailLevel", 80.f, 30.f);
	m_DetailLevel->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(33.f);
	m_SplatCount = CreateWidget<CEditorInputText>("SplatCount", 100.f, 30.f);
	m_SplatCount->SetInputType(EImGuiInputType::Int);

	m_Image = CreateWidget<CEditorImage>("Texture");
	m_Image->SetSize(150.f, 150.f);
	m_Image->SetHideName("Texture");
	m_Image->SetIsFixed(true);
	return true;
}

void CTerrainWindow::Update(float deltaTime)
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

void CTerrainWindow::ChangeTerrain()
{
	if (!m_SelectComponent)
	{
		return;
	}
	m_SelectComponent->SetDetailLevel(m_DetailLevel->GetFloat());
	m_SelectComponent->SetSplatCount(m_SplatCount->GetInt());
}

void CTerrainWindow::ResetTerrain()
{
	if (!m_SelectComponent)
	{
		return;
	}
	m_SelectComponent->SetDetailLevel(m_DetailLevel->GetFloat());
	m_SelectComponent->SetSplatCount(m_SplatCount->GetInt());
	m_SelectComponent->SetHeight(m_Height->GetFloat());
	//삭제함수 필요
	m_SelectComponent->DeleteTerrain();
	m_SelectComponent->CreateTerrain(m_CountX->GetInt(), m_CountY->GetInt(), m_CellSizeX->GetFloat(),
		m_CellSizeY->GetFloat(), m_Image->GetTexture()->GetFullPath());
}

void CTerrainWindow::SetImage(const std::string& name, const TCHAR* path)
{
	m_Image->SetTextureFullPath(name, path);
	m_CountX->SetFloat((float)m_Image->GetImgWidth());
	m_CountY->SetFloat((float)m_Image->GetImgHeight());
	m_MeshName->SetText(m_Image->GetTexture()->GetName().c_str());
}