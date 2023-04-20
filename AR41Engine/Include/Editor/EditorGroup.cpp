#include "EditorGroup.h"

CEditorGroup::CEditorGroup()
{
}

CEditorGroup::~CEditorGroup()
{
    size_t  size = m_vecWidget.size();
    for (size_t i = 0; i < size; ++i)
    {
        SAFE_DELETE(m_vecWidget[i]);
    }
    m_vecWidget.clear();
}

bool CEditorGroup::Init()
{
	return true;
}

void CEditorGroup::Render()
{
    ImGui::BeginGroup();
    size_t  size = m_vecWidget.size();
    for (size_t i = 0; i < size; ++i)
    {
        m_vecWidget[i]->Render();
    }
    ImGui::EndGroup();
}
