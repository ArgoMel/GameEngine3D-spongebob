#include "EditorText.h"

CEditorText::CEditorText()
	: m_Text{}
	, m_wText{}
	, m_TextUTF8{}
	, m_IsMultiLine(false)
{
}

CEditorText::~CEditorText()
{
}

bool CEditorText::Init()
{
	return true;
}

void CEditorText::Render()
{
	//ImGui::Text(m_TextUTF8.c_str());
	if (m_IsMultiLine)
	{
		ImGui::TextWrapped(m_TextUTF8);
	}
	else
	{
		ImGui::TextColored(m_Color, m_TextUTF8);
	}
	if (m_TextCallback)
	{
		m_TextCallback();
	}
}

void CEditorText::SetText(const char* text)
{
	memset(m_Text, 0, 1024);
	memset(m_wText, 0, 2048);
	memset(m_TextUTF8, 0, 1024);
	strcpy_s(m_Text, text);
	int length = (int)MultiByteToWideChar(CP_ACP, 0, text, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, text, -1, m_wText, length);
	length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, length, nullptr, nullptr);
}

void CEditorText::AddText(const char* text)
{
	strcat_s(m_Text, text);
	SetText(m_Text);
}

bool CEditorText::Empty() const
{
	return strlen(m_Text) == 0;
}
