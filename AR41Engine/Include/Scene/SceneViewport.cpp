#include "SceneViewport.h"
#include "Scene.h"
#include "../Input.h"
#include "../GameObject/GameObject.h"

CSceneViewport::CSceneViewport()
    : m_Owner(nullptr)
	, m_Start(false)
{
}

CSceneViewport::~CSceneViewport()
{
}

void CSceneViewport::LoadComplete()
{
}

void CSceneViewport::Start()
{
	m_Start = true;
	size_t	size = m_vecWindow.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecWindow[i]->Start();
	}
}

bool CSceneViewport::Init()
{
    return true;
}

void CSceneViewport::Update(float deltaTime)
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
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

void CSceneViewport::PostUpdate(float deltaTime)
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
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

void CSceneViewport::Render()
{
	if (m_vecWindow.size() >= 2)
	{
		std::sort(m_vecWindow.begin(), m_vecWindow.end(), CSceneViewport::SortWindow);
	}
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
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

void CSceneViewport::Save(FILE* file)
{
	int	count = (int)m_vecWindow.size();
	fwrite(&count, sizeof(int), 1, file);
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();
	for (; iter != iterEnd; ++iter)
	{
		std::string	typeName = (*iter)->GetWindowTypeName();
		int	length = (int)typeName.length();
		fwrite(&length, sizeof(int), 1,file);
		fwrite(typeName.c_str(), 1, length, file);
		(*iter)->Save(file);
	}
}

void CSceneViewport::Load(FILE* file)
{
	int	count = 0;
	fread(&count, sizeof(int), 1, file);
	for (int i = 0; i < count; ++i)
	{
		char	typeName[256] = {};
		int	length = 0;
		fread(&length, sizeof(int), 1,file);
		assert(length >= 0);
		fread(typeName, 1, length, file);
		CUIWindow* CDO = CUIWindow::FindCDO(typeName);
		CUIWindow* window = CDO->Clone();
		window->m_Owner = this;
		window->m_Scene = m_Owner;
		window->Load(file);
		m_vecWindow.push_back(window);
	}
}

bool CSceneViewport::CollisionMouse()
{
	if (m_vecWindow.size() >= 2)
	{
		std::sort(m_vecWindow.begin(), m_vecWindow.end(), CSceneViewport::SortWindowInv);
	}
	if (m_CollisionWidget && !m_CollisionWidget->GetActive())
	{
		m_CollisionWidget = nullptr;
	}
	Vector2	mousePos = CInput::GetInst()->GetMouseUIPos();
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();
	bool	result = false;
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
		{
			continue;
		}
		CUIWidget* Widget = (*iter)->CollisionMouse(mousePos);
		if (m_CollisionWidget && m_CollisionWidget != Widget)
		{
			m_CollisionWidget->m_MouseHovered = false;
		}
		if (Widget)
		{
			m_CollisionWidget = Widget;
		}
		if (Widget)
		{
			result = true;
			break;
		}
	}
	if (!result)	// 충돌이 안되었다면
	{
		if (m_CollisionWidget)
		{
			m_CollisionWidget->m_MouseHovered = false;
		}
		m_CollisionWidget = nullptr;
	}
	iter = m_vecWindow.begin();
	iterEnd = m_vecWindow.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}
		++iter;
	}
	return result;
}

void CSceneViewport::GetAllWindowHierarchyName(std::vector<HierarchyWindowName>& vecName)
{
	size_t size = m_vecWindow.size();
	for (size_t i = 0; i < size; ++i)
	{
		HierarchyWindowName	names;
		names.name = m_vecWindow[i]->GetName();
		names.className = m_vecWindow[i]->GetWindowTypeName();
		if(names.className=="UIWindow")
		{
			continue;
		}
		if (names.name == "")
		{
			names.name = names.className;
		}
		names.window = m_vecWindow[i];
		vecName.push_back(names);
	}
}

bool CSceneViewport::SortWindow(CSharedPtr<CUIWindow> src, CSharedPtr<CUIWindow> dest)
{
	return src->GetZOrder() > dest->GetZOrder();
}

bool CSceneViewport::SortWindowInv(CSharedPtr<CUIWindow> src, CSharedPtr<CUIWindow> dest)
{
	return src->GetZOrder() < dest->GetZOrder();
}