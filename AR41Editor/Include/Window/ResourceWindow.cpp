#include "ResourceWindow.h"
#include "PathManager.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorGUIManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Sound/Sound.h"

CResourceWindow::CResourceWindow()
	: m_TextureList(nullptr)
	, m_SoundList(nullptr)
	, m_FontList(nullptr)
	, m_TextureImageBox(nullptr)
	, m_FileName(nullptr)
	, m_SoundName(nullptr)
	, m_FontName(nullptr)
{
}

CResourceWindow::~CResourceWindow()
{
}

bool CResourceWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("TextureList");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(150.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_FileName = CreateWidget<CEditorInputText>("TextureName", 200.f, 30.f);
	m_FileName->SetHideName("TextureName");
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("이미지 검색", 100.f, 30.f);
	button->SetClickCallback<CResourceWindow>(this, &CResourceWindow::TextureSearch);
	m_TextureList = CreateWidget<CEditorListBox>("TextureListBox");
	m_TextureList->SetHideName("TextureListBox");
	m_TextureList->SetSize(500.f, 300.f);
	m_TextureList->SetPageItemCount(6);
	m_TextureList->SetSelectCallback<CResourceWindow>(this, &CResourceWindow::TextureClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_TextureImageBox = CreateWidget<CEditorImage>("Texture");
	m_TextureImageBox->SetSize(200.f, 200.f);
	m_TextureImageBox->SetHideName("Texture");
	m_TextureImageBox->SetIsFixed(true);

	label = CreateWidget<CEditorLabel>("SoundList");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(150.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_SoundName = CreateWidget<CEditorInputText>("SoundName", 200.f, 30.f);
	m_SoundName->SetHideName("SoundName");
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("소리 검색", 80.f, 30.f);
	button->SetClickCallback<CResourceWindow>(this, &CResourceWindow::SoundSearch);
	m_SoundList = CreateWidget<CEditorListBox>("SoundListBox");
	m_SoundList->SetHideName("SoundListBox");
	m_SoundList->SetSize(500.f, 300.f);
	m_SoundList->SetPageItemCount(6);
	m_SoundList->SetSelectCallback<CResourceWindow>(this, &CResourceWindow::SoundClickCallback);

	label = CreateWidget<CEditorLabel>("FontList");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(150.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_FontName = CreateWidget<CEditorInputText>("FondName", 200.f, 30.f);
	m_FontName->SetHideName("FondName");
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("폰트 검색", 80.f, 30.f);
	button->SetClickCallback<CResourceWindow>(this, &CResourceWindow::FontSearch);
	m_FontList = CreateWidget<CEditorListBox>("FontListBox");
	m_FontList->SetHideName("FontListBox");
	m_FontList->SetSize(500.f, 300.f);
	m_FontList->SetPageItemCount(6);
	m_FontList->SetSelectCallback<CResourceWindow>(this, &CResourceWindow::FontClickCallback);

    LoadTextureList();
	LoadSoundList();
	LoadFontList();
    return true;
}

void CResourceWindow::Update(float deltaTime)
{
    CEditorWindow::Update(deltaTime);
}

void CResourceWindow::LoadTextureList()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(TEXTURE_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	const char* textureExtArray[5] = { ".BMP", ".JPG", ".PNG", ".TGA", ".DDS" };
	for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
	{
		char	name[256] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		char	folderName[10] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		int	length = (int)strlen(fullPath);
		if (length >= 9)
		{
			for (int i = length - 10; i >= 0; --i)
			{
				memcpy(folderName, &fullPath[i], 9);
				if (strcmp(folderName, "/Texture/") == 0)
				{
					strcpy_s(name, &fullPath[i + 9]);
					break;
				}
			}
		}
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
		_strupr_s(ext);
		bool	fileExt = false;
		int size = (int)std::size(textureExtArray);
		for (int i = 0; i < size; ++i)
		{
			if (strcmp(ext, textureExtArray[i]) == 0)
			{
				fileExt = true;
				break;
			}
		}
		if (!fileExt)
		{
			continue;
		}
		m_TextureList->AddItem(name);
		TCHAR	fileName[MAX_PATH] = {};
		length = (int)MultiByteToWideChar(CP_ACP, 0, name, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, name, -1, fileName, length);
		CResourceManager::GetInst()->LoadTexture(name, fileName);
	}
}

void CResourceWindow::LoadSoundList()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(SOUND_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	const char* textureExtArray[2] = { ".MP3", ".WAV"};
	for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
	{
		char	name[256] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		char	folderName[10] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		int	length = (int)strlen(fullPath);
		if (length >= 7)
		{
			for (int i = length - 8; i >= 0; --i)
			{
				memcpy(folderName, &fullPath[i], 7);
				if (strcmp(folderName, "/Sound/") == 0)
				{
					strcpy_s(name, &fullPath[i + 7]);
					break;
				}
			}
		}
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
		_strupr_s(ext);
		bool	fileExt = false;
		int size = (int)std::size(textureExtArray);
		for (int i = 0; i < size; ++i)
		{
			if (strcmp(ext, textureExtArray[i]) == 0)
			{
				fileExt = true;
				break;
			}
		}
		if (!fileExt)
		{
			continue;
		}
		m_SoundList->AddItem(name);

		CResourceManager::GetInst()->LoadSound("Editor", name, false, name);
	}
}

void CResourceWindow::LoadFontList()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(FONT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	const char* textureExtArray[2] = { ".OTF", ".TTF" };
	for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
	{
		char	name[256] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		char	folderName[10] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		int	length = (int)strlen(fullPath);
		if (length >= 6)
		{
			for (int i = length - 7; i >= 0; --i)
			{
				memcpy(folderName, &fullPath[i], 6);
				if (strcmp(folderName, "/Font/") == 0)
				{
					strcpy_s(name, &fullPath[i + 6]);
					break;
				}
			}
		}
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
		_strupr_s(ext);
		bool	fileExt = false;
		int size = (int)std::size(textureExtArray);
		for (int i = 0; i < size; ++i)
		{
			if (strcmp(ext, textureExtArray[i]) == 0)
			{
				fileExt = true;
				break;
			}
		}
		if (!fileExt)
		{
			continue;
		}
		m_FontList->AddItem(name);
		TCHAR	fileName[MAX_PATH] = {};
		length = (int)MultiByteToWideChar(CP_ACP, 0, name, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, name, -1, fileName, length);
		CResourceManager::GetInst()->CreateFontCollection(name, fileName);
		CResourceManager::GetInst()->LoadFont(name, fileName,600,20.f, TEXT("ko"));
	}
}

void CResourceWindow::TextureClickCallback(int index, const std::string& item)
{
	m_SelectTexture = CResourceManager::GetInst()->FindTexture(item);
	m_TextureImageBox->SetTexture(m_SelectTexture);
}

void CResourceWindow::SoundClickCallback(int index, const std::string& item)
{
	CSound* sound = CResourceManager::GetInst()->FindSound(item);
	sound->Play();
}

void CResourceWindow::FontClickCallback(int index, const std::string& item)
{
	m_SelectFont = CResourceManager::GetInst()->FindFont(item);
}
//새로 검색할려면 지우고 검색 누르고 검색해야한다
void CResourceWindow::TextureSearch()
{
	std::string name(m_FileName->GetText());
	if (name=="")
	{
		m_TextureList->Clear();
		LoadTextureList();
		return;
	}
	size_t size = m_TextureList->GetSize();
	std::vector<std::string> vecName;
	std::string checkedName;
	for (size_t i = 0; i < size; ++i)
	{
		checkedName = m_TextureList->CheckPartitionItem(i, name);
		if (checkedName != "")
		{
			vecName.push_back(checkedName);
		}
	}
	size_t vecSize = vecName.size();
	if (vecSize != 0)
	{
		m_TextureList->Clear();
		for (size_t i = 0; i < vecSize; ++i)
		{
			m_TextureList->AddItem(vecName[i]);
		}
	}
}

void CResourceWindow::SoundSearch()
{
	std::string name(m_SoundName->GetText());
	if (name == "")
	{
		m_SoundList->Clear();
		LoadSoundList();
		return;
	}
	size_t size = m_SoundList->GetSize();
	std::vector<std::string> vecName;
	std::string checkedName;
	for (size_t i = 0; i < size; ++i)
	{
		checkedName = m_SoundList->CheckPartitionItem(i, name);
		if (checkedName != "")
		{
			vecName.push_back(checkedName);
		}
	}
	size_t vecSize = vecName.size();
	if (vecSize != 0)
	{
		m_SoundList->Clear();
		for (size_t i = 0; i < vecSize; ++i)
		{
			m_SoundList->AddItem(vecName[i]);
		}
	}
}

void CResourceWindow::FontSearch()
{
	std::string name(m_FontName->GetText());
	if (name == "")
	{
		m_FontList->Clear();
		LoadFontList();
		return;
	}
	size_t size = m_FontList->GetSize();
	std::vector<std::string> vecName;
	std::string checkedName;
	for (size_t i = 0; i < size; ++i)
	{
		checkedName = m_FontList->CheckPartitionItem(i, name);
		if (checkedName != "")
		{
			vecName.push_back(checkedName);
		}
	}
	size_t vecSize = vecName.size();
	if (vecSize != 0)
	{
		m_FontList->Clear();
		for (size_t i = 0; i < vecSize; ++i)
		{
			m_FontList->AddItem(vecName[i]);
		}
	}
}
