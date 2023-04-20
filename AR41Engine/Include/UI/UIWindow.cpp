#include "UIWindow.h"
#include "../Device.h"
std::unordered_map<std::string, CUIWindow*> CUIWindow::m_mapUIWindowCDO;

CUIWindow::CUIWindow()
	: m_Owner(nullptr)
	, m_Scene(nullptr)
	, m_ZOrder(0)
	, m_Start(false)
{
	m_WindowTypeName = "UIWindow";
	m_Size = Vector2((float)CDevice::GetInst()->GetResolution().width, (float)CDevice::GetInst()->GetResolution().height);
}

CUIWindow::CUIWindow(const CUIWindow& window)
	: CRef(window)
{
	m_ZOrder = window.m_ZOrder;
	m_Size = window.m_Size;
	m_Start = false;
	m_WindowTypeName = window.m_WindowTypeName;
	size_t	size = window.m_vecWidget.size();
	for (size_t i = 0; i < size; ++i)
	{
		CUIWidget* widget = window.m_vecWidget[i]->Clone();
		widget->m_Owner = this;
		m_vecWidget.push_back(widget);
	}
}

CUIWindow::~CUIWindow()
{
}

void CUIWindow::Start()
{
	size_t	size = m_vecWidget.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecWidget[i]->Start();
	}
}

bool CUIWindow::Init()
{
	return true;
}

void CUIWindow::Update(float deltaTime)
{
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(deltaTime);
		++iter;
	}
}

void CUIWindow::PostUpdate(float deltaTime)
{
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->PostUpdate(deltaTime);
		++iter;
	}
}

void CUIWindow::Render()
{
	if (m_vecWidget.size() >= 2)
	{
		std::sort(m_vecWidget.begin(), m_vecWidget.end(), CUIWindow::SortWidget);
	}
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Render();
		++iter;
	}
}

CUIWindow* CUIWindow::Clone()
{
	return new CUIWindow(*this);
}

void CUIWindow::Save(FILE* file)
{
	fwrite(&m_ZOrder, sizeof(int), 1, file);
	fwrite(&m_Pos, sizeof(Vector2), 1, file);
	fwrite(&m_Size, sizeof(Vector2), 1, file);
	int	count = (int)m_vecWidget.size();
	fwrite(&count, sizeof(int), 1, file);
	for (int i = 0; i < count; ++i)
	{
		std::string	typeName = m_vecWidget[i]->GetWidgetTypeName();
		int	length = (int)typeName.length();
		fwrite(&length, sizeof(int), 1, file);
		fwrite(typeName.c_str(), 1, length, file);
		m_vecWidget[i]->Save(file);
	}
}

void CUIWindow::Load(FILE* file)
{
	fread(&m_ZOrder, sizeof(int), 1, file);
	fread(&m_Pos, sizeof(Vector2), 1, file);
	fread(&m_Size, sizeof(Vector2), 1, file);
	int	count = 0;
	fread(&count, sizeof(int), 1, file);
	for (int i = 0; i < count; ++i)
	{
		char	typeName[256] = {};
		int	length = 0;
		fread(&length, sizeof(int), 1, file);
		assert(length >= 0);
		fread(typeName, 1, length, file);
		if (m_vecWidget.empty())
		{
			CUIWidget* CDO = CUIWidget::FindCDO(typeName);
			CUIWidget* widget = CDO->Clone();
			widget->m_Owner = this;
			widget->m_Scene = m_Scene;
			widget->Init();
			widget->Load(file);
			m_vecWidget.push_back(widget);
		}
		else
		{
			m_vecWidget[i]->m_Owner = this;
			m_vecWidget[i]->m_Scene = m_Scene;
			m_vecWidget[i]->Init();
			m_vecWidget[i]->Load(file);
			m_vecWidget[i]->SetWidgetTypeName(typeName);
		}
	}
	size_t	size = m_vecWidget.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecWidget[i]->m_Scene = m_Scene;
	}
}

CUIWidget* CUIWindow::CollisionMouse(const Vector2& MousePos)
{
	if (m_Pos.x > MousePos.x)
	{
		return nullptr;
	}
	else if (m_Pos.x + m_Size.x < MousePos.x)
	{
		return nullptr;
	}
	else if (m_Pos.y > MousePos.y)
	{
		return nullptr;
	}
	else if (m_Pos.y + m_Size.y < MousePos.y)
	{
		return nullptr;
	}
	if (m_vecWidget.size() >= 2)
	{
		std::sort(m_vecWidget.begin(), m_vecWidget.end(), CUIWindow::SortWidgetInv);
	}
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
		{
			continue;
		}
		if ((*iter)->CollisionMouse(MousePos))
		{
			(*iter)->m_MouseHovered = true;
			return *iter;
		}
		else
		{
			(*iter)->m_MouseHovered = false;
		}
	}
	return nullptr;
}

void CUIWindow::GetAllWidgetHierarchyName(std::vector<HierarchyWidgetName>& vecName)
{
	size_t size = m_vecWidget.size();
	for (size_t i = 0; i < size; ++i)
	{
		HierarchyWidgetName	names;
		names.name = m_vecWidget[i]->GetName();
		names.className = m_vecWidget[i]->GetWidgetTypeName();
		names.widget = m_vecWidget[i];
		vecName.push_back(names);
	}
}

bool CUIWindow::SortWidget(CSharedPtr<CUIWidget> src, CSharedPtr<CUIWidget> dest)
{
	return src->GetZOrder() > dest->GetZOrder();
}

bool CUIWindow::SortWidgetInv(CSharedPtr<CUIWidget> src, CSharedPtr<CUIWidget> dest)
{
	return src->GetZOrder() < dest->GetZOrder();
}