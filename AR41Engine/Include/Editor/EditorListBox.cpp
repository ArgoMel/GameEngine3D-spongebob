#include "EditorListBox.h"

CEditorListBox::CEditorListBox()
	: m_Sort(false)
	, m_Select(false)
	, m_SelectIndex(-1)
	, m_PageItemCount(3)
{
}

CEditorListBox::~CEditorListBox()
{
}

void CEditorListBox::AddItem(const std::string& item)
{
	TCHAR	wText[256] = {};
	int length = (int)MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wText, length);
	char	textUTF8[256] = {};
	length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_UTF8, 0, wText, -1, textUTF8, length, nullptr, nullptr);
	m_vecItem.push_back(item);
	m_vecItemUTF8.push_back(textUTF8);
}

void CEditorListBox::DeleteItem(const std::string& item)
{
	size_t	size = m_vecItem.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecItem[i] == item)
		{
			DeleteItem((int)i);
			break;
		}
	}
}

void CEditorListBox::DeleteItem(int index)
{
	auto	iter = m_vecItem.begin() + index;
	m_vecItem.erase(iter);
	iter = m_vecItemUTF8.begin() + index;
	m_vecItemUTF8.erase(iter);
}

void CEditorListBox::Sort(bool sort)
{
	m_Sort = sort;
	if (m_Sort)
	{
		std::sort(m_vecItem.begin(), m_vecItem.end());
		std::sort(m_vecItemUTF8.begin(), m_vecItemUTF8.end());
	}
}

bool CEditorListBox::Init()
{
	return true;
}

void CEditorListBox::Render()
{
	ImVec2	size;
	size.x = m_Size.x;
	size.y = m_PageItemCount * ImGui::GetTextLineHeightWithSpacing();
	if (ImGui::BeginListBox(m_Name.c_str(), size))
	{
		size_t	itemCount = m_vecItemUTF8.size();
		m_Select = false;
		for (size_t i = 0; i < itemCount; ++i)
		{
			bool	select = false;
			if (ImGui::Selectable(m_vecItemUTF8[i].c_str(), &select))
			{
				m_Select = true;
				if (m_SelectIndex != (int)i && m_SelectCallback)
				{
					m_SelectIndex = (int)i;
					m_SelectCallback(m_SelectIndex, m_vecItem[i]);
				}
			}
			if (select)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
}
