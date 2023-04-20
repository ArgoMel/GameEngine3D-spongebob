#include "EditorCheckBox.h"

CEditorCheckBox::CEditorCheckBox()
	: m_Check(false)
{
}

CEditorCheckBox::~CEditorCheckBox()
{
}

bool CEditorCheckBox::Init()
{
	return true;
}

void CEditorCheckBox::Render()
{
	bool    check = m_Check;
	ImGui::Checkbox(m_NameUTF8.c_str(), &check);
	if (m_Check != check)
	{
		m_Check = check;
		if (m_CheckCallback)
		{
			m_CheckCallback(check);
		}
	}
}
