#include "EditorInputText.h"

CEditorInputText::CEditorInputText()
	: m_Text{}
	, m_wText{}
	, m_TText{}
	, m_TextUTF8{}
	, m_HintText{}
	, m_HintwText{}
	, m_HintTText{}
	, m_HintTextUTF8{}
	, m_InputType(EImGuiInputType::String)
	, m_MultiLine(false)
	, m_Flag(0)
	, m_ValueInt(0)
	, m_ValueFloat(0.f)
{
}

CEditorInputText::~CEditorInputText()
{
}

bool CEditorInputText::Init()
{
	return true;
}

void CEditorInputText::Render()
{
	ImGui::PushItemWidth(m_Size.x);
	bool	input = false;
	if (m_MultiLine)
	{
		input = ImGui::InputTextMultiline(m_NameUTF8.c_str(), m_TextUTF8, IM_ARRAYSIZE(m_TextUTF8), m_Size, m_Flag);
	}
	else
	{
		switch (m_InputType)
		{
		case EImGuiInputType::String:
			if (strlen(m_HintText) > 0)
			{
				input = ImGui::InputTextWithHint(m_NameUTF8.c_str(), m_HintTextUTF8, m_Text, IM_ARRAYSIZE(m_Text), m_Flag);
			}
			else
			{
				input = ImGui::InputText(m_NameUTF8.c_str(), m_TextUTF8, IM_ARRAYSIZE(m_TextUTF8), m_Flag);
			}
			break;
		case EImGuiInputType::Int:
			input = ImGui::InputInt(m_NameUTF8.c_str(), &m_ValueInt, 1, 10, m_Flag);
			break;
		case EImGuiInputType::Float:
			input = ImGui::InputFloat(m_NameUTF8.c_str(), &m_ValueFloat, 0.f, 0.f, "%.5f", m_Flag);
			break;
		}
	}
	if (input)
	{
		if (m_InputType == EImGuiInputType::String)
		{
			memset(m_Text, 0, 1024);
			memset(m_wText, 0, 2048);
			memset(m_TText, 0, 2048);
			//memset(m_TextUTF8, 0, 1024);
			int length = (int)MultiByteToWideChar(CP_ACP, 0, m_TextUTF8, -1, nullptr, 0);
			MultiByteToWideChar(CP_ACP, 0, m_TextUTF8, -1, m_wText, length);
			MultiByteToWideChar(CP_UTF8, 0, m_TextUTF8, -1, m_TText, length);
			length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, nullptr, 0, nullptr, nullptr);
			WideCharToMultiByte(CP_ACP, 0, m_wText, -1, m_Text, length, nullptr, nullptr);
		}
		if (m_InputCallback)
		{
			m_InputCallback();
		}
	}
	ImGui::PopItemWidth();
}

void CEditorInputText::SetText(const char* text)
{
	memset(m_Text, 0, 1024);
	memset(m_wText, 0, 2048);
	memset(m_TText, 0, 2048);
	memset(m_TextUTF8, 0, 1024);
	strcpy_s(m_Text, text);
	int length = (int)MultiByteToWideChar(CP_ACP, 0, text, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, text, -1, m_wText, length);
	MultiByteToWideChar(CP_ACP, 0, text, -1, m_TText, length);
	length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, length, nullptr, nullptr);
}

void CEditorInputText::SetHintText(const char* text)
{
	memset(m_HintText, 0, 1024);
	memset(m_HintwText, 0, 2048);
	memset(m_HintTText, 0, 2048);
	memset(m_HintTextUTF8, 0, 1024);
	strcpy_s(m_HintText, text);
	int length = (int)MultiByteToWideChar(CP_ACP, 0, text, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, text, -1, m_HintwText, length);
	MultiByteToWideChar(CP_ACP, 0, text, -1, m_HintTText, length);
	length = WideCharToMultiByte(CP_UTF8, 0, m_HintwText, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_UTF8, 0, m_HintwText, -1, m_HintTextUTF8, length, nullptr, nullptr);
}

void CEditorInputText::SetInt(int value)
{
	m_ValueInt = value;
}

void CEditorInputText::SetFloat(float value)
{
	m_ValueFloat = value;
}

void CEditorInputText::SetMultiLine(bool multiLine)
{
	m_MultiLine = multiLine;
}

void CEditorInputText::AddFlag(ImGuiInputTextFlags_ flag)
{
	m_Flag |= flag;
}

void CEditorInputText::ReadOnly(bool enable)
{
	if (enable)
	{
		m_Flag |= ImGuiInputTextFlags_ReadOnly;
	}
	else if (m_Flag & ImGuiInputTextFlags_ReadOnly)
	{
		m_Flag ^= ImGuiInputTextFlags_ReadOnly;
	}
}

void CEditorInputText::PasswordMode(bool enable)
{
	if (enable)
	{
		m_Flag |= ImGuiInputTextFlags_Password;
	}
	else if (m_Flag & ImGuiInputTextFlags_Password)
	{
		m_Flag ^= ImGuiInputTextFlags_Password;
	}
}

void CEditorInputText::SetInputType(EImGuiInputType type)
{
	m_InputType = type;
}

bool CEditorInputText::Empty() const
{
	return strlen(m_Text) == 0;
}
