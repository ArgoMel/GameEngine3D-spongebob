#include "ImageWindow.h"
#include "PathManager.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorSlider.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material/Material.h"
#include "GameObject/GameObject.h"
#include "../GameObject/Player2D.h"

CImageWindow::CImageWindow()
	: m_SpriteList(nullptr)
	, m_SpriteSelectName(nullptr)
	, m_Image(nullptr)
	, m_Slider(nullptr)
{
}

CImageWindow::~CImageWindow()
{
}

void CImageWindow::SetSelectComponent(CSpriteComponent* component)
{
	m_SelectComponent = component;
}

bool CImageWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("캐릭터 스프라이트");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("적용", 50.f, 30.f);
	button->SetClickCallback<CImageWindow>(this, &CImageWindow::SpriteCreateCallback);
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_SpriteSelectName = CreateWidget<CEditorInputText>("SpriteName");
	m_SpriteSelectName->SetHideName("SpriteName");
	m_SpriteSelectName->SetSize(150.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("검색", 50.f, 30.f);
	button->SetClickCallback<CImageWindow>(this, &CImageWindow::SpriteInputCallback);
	m_Slider = CreateWidget<CEditorSlider>("Slider");
	m_Slider->SetSliderCallback<CImageWindow>(this, &CImageWindow::SpriteColorCallback);
	m_Slider->SetText("색상");
	m_SpriteList = CreateWidget<CEditorListBox>("SpriteListBox");
	m_SpriteList->SetHideName("SpriteListBox");
	m_SpriteList->SetSize(200.f, 300.f);
	m_SpriteList->SetPageItemCount(6);
	m_SpriteList->SetSelectCallback<CImageWindow>(this, &CImageWindow::SpriteClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPos(20.f,25.f);
	m_Image = CreateWidget<CEditorImage>("Char_sprite");
	m_Image->SetTexture("Char_sprite", TEXT("teemo.png"));
	LoadSpriteName();
	return true;
}

void CImageWindow::Update(float deltaTime)
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

void CImageWindow::SpriteInputCallback()
{
	std::string name(m_SpriteSelectName->GetText());
	if (name == "")
	{
		m_SpriteList->Clear();
		LoadSpriteName();
		return;
	}
	size_t size = m_SpriteList->GetSize();
	std::vector<std::string> vecName;
	std::string checkedName;
	for (size_t i = 0; i < size; ++i)
	{
		checkedName = m_SpriteList->CheckPartitionItem(i, name);
		if (checkedName != "")
		{
			vecName.push_back(checkedName);
		}
	}
	size_t vecSize = vecName.size();
	if (vecSize != 0)
	{
		m_SpriteList->Clear();
		for (size_t i = 0; i < vecSize; ++i)
		{
			m_SpriteList->AddItem(vecName[i]);
		}
	}
}

void CImageWindow::SpriteClickCallback(int index, const std::string& item)
{
	m_SelectSpriteItem = item;
	m_SpriteSelectName->SetText(item.c_str());
	m_SpritePath = "Character//" + item + "//pixelart//animated//neutral.gif";
	TCHAR* t_filename = new TCHAR[m_SpritePath.size() + 1];
	t_filename[m_SpritePath.size()] = 0;
	std::copy(m_SpritePath.begin(), m_SpritePath.end(), t_filename);
	m_Image->SetTexture(m_SelectSpriteItem, t_filename);
	SAFE_DELETE_ARRAY(t_filename);
	if (!m_Image->GetTexture())
	{
		m_SpritePath = "Character//" + item + "//pixelart//sprite_sheet//pixelart0001.png";
		TCHAR* t_filename = new TCHAR[m_SpritePath.size() + 1];
		t_filename[m_SpritePath.size()] = 0;
		std::copy(m_SpritePath.begin(), m_SpritePath.end(), t_filename);
		m_Image->SetTexture(m_SelectSpriteItem, t_filename);
		SAFE_DELETE_ARRAY(t_filename);
	}
	if (!m_Image->GetTexture())
	{
		m_SpritePath = "Character//" + item + "//pixelart//sprite_sheet//pixelart0002.png";
		TCHAR* t_filename = new TCHAR[m_SpritePath.size() + 1];
		t_filename[m_SpritePath.size()] = 0;
		std::copy(m_SpritePath.begin(), m_SpritePath.end(), t_filename);
		m_Image->SetTexture(m_SelectSpriteItem, t_filename);
		SAFE_DELETE_ARRAY(t_filename);
	}
	if (!m_Image->GetTexture())
	{
		m_SpritePath = "Character//" + item + "//pixelart//sprite_sheet//000a.png";
		TCHAR* t_filename = new TCHAR[m_SpritePath.size() + 1];
		t_filename[m_SpritePath.size()] = 0;
		std::copy(m_SpritePath.begin(), m_SpritePath.end(), t_filename);
		m_Image->SetTexture(m_SelectSpriteItem, t_filename);
		SAFE_DELETE_ARRAY(t_filename);
	}
}

// 애니메이션이 존재하는 물체에는 보이지 않음. 적용은 됌
void CImageWindow::SpriteCreateCallback()
{
	if (m_SelectComponent)
	{
		TCHAR* t_filename = new TCHAR[m_SpritePath.size() + 1];
		t_filename[m_SpritePath.size()] = 0;
		std::copy(m_SpritePath.begin(), m_SpritePath.end(), t_filename);
		m_SelectComponent->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, m_SelectSpriteItem, t_filename);
		m_SelectComponent->GetMaterial(0)->SetBaseColorUnsignedChar((unsigned char)m_Slider->GetPosition(0),
			(unsigned char)m_Slider->GetPosition(1), (unsigned char)m_Slider->GetPosition(2), (unsigned char)m_Slider->GetPosition(3));
		m_SelectComponent->GetMaterial(0)->SetOpacity((unsigned char)m_Slider->GetPosition(3) / 255.f);
		SAFE_DELETE_ARRAY(t_filename);
		std::string componentName = m_SelectComponent->GetName();
		m_SelectComponent->SetName(m_SelectSpriteItem);
	}
}

void CImageWindow::SpriteColorCallback()
{
	m_Image->SetColor((unsigned char)m_Slider->GetPosition(0), (unsigned char)m_Slider->GetPosition(1), 
		(unsigned char)m_Slider->GetPosition(2), (unsigned char)m_Slider->GetPosition(3));
}

void CImageWindow::LoadSpriteName()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(TEXTURE_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	strcat_s(path, "Character/");
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		m_SpriteList->AddItem(name);
	}
	//strcpy_s(path, info->pathMultibyte);
	//strcat_s(path, "Bullet/");
	//for (const auto& file : std::filesystem::directory_iterator(path))
	//{
	//	char	name[64] = {};
	//	char	fullPath[MAX_PATH] = {};
	//	char	ext[_MAX_EXT] = {};
	//	strcpy_s(fullPath, file.path().generic_string().c_str());
	//	_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
	//	m_SpriteList->AddItem(name);
	//}
}
