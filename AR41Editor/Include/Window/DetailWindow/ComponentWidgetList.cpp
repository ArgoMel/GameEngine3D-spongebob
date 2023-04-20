#include "ComponentWidgetList.h"

CComponentWidgetList::CComponentWidgetList()
    : m_DetailWindow(nullptr)
{
}

CComponentWidgetList::~CComponentWidgetList()
{
}

bool CComponentWidgetList::Init()
{
    return true;
}

void CComponentWidgetList::Render()
{
    CEditorGroup::Render();
}
