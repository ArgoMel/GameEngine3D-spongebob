#include "EditorComboBox.h"

CEditorComboBox::CEditorComboBox()
	: m_Sort(false)
	, m_Select(false)
	, m_SelectPrevViewName(false)
	, m_SelectIndex(-1)
	, m_PageItemCount(3)
	, m_Flag(0)
{
}

CEditorComboBox::~CEditorComboBox()
{
}

void CEditorComboBox::AddItem(const std::string& item)
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

void CEditorComboBox::DeleteItem(const std::string& item)
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

void CEditorComboBox::DeleteItem(int index)
{
	auto	iter = m_vecItem.begin() + index;
	m_vecItem.erase(iter);
	iter = m_vecItemUTF8.begin() + index;
	m_vecItemUTF8.erase(iter);
}

void CEditorComboBox::Sort(bool sort)
{
	m_Sort = sort;
	if (m_Sort)
	{
		std::sort(m_vecItem.begin(), m_vecItem.end());
		std::sort(m_vecItemUTF8.begin(), m_vecItemUTF8.end());
	}
}

bool CEditorComboBox::Init()
{
	SetPrevViewName(m_Name);
	return true;
}

void CEditorComboBox::Render()
{
	ImGui::PushItemWidth(m_Size.x);
	ImVec2	size;
	size.x = m_Size.x;
	size.y = m_PageItemCount * ImGui::GetTextLineHeightWithSpacing();
	if (ImGui::BeginCombo(m_NameUTF8.c_str(), m_PrevViewNameUTF8.c_str(), m_Flag))
	{
		size_t	itemCount = m_vecItemUTF8.size();
		m_Select = false;
		for (size_t i = 0; i < itemCount; ++i)
		{
			bool	select = false;
			if (ImGui::Selectable(m_vecItemUTF8[i].c_str(), &select))
			{
				m_Select = true;
				if (m_SelectIndex != (int)i)
				{
					m_SelectIndex = (int)i;
					if (m_SelectCallback)
					{
						m_SelectCallback(m_SelectIndex, m_vecItem[i]);
					}
				}
				if (m_SelectPrevViewName)
				{
					m_PrevViewName = m_vecItem[i];
					m_PrevViewNameUTF8 = m_vecItemUTF8[i];
				}
			}
			if (select)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
}
