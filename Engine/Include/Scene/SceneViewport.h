#pragma once
#include "../EngineInfo.h"
#include "../UI/UIWindow.h"
#include "../UI/UIWidget.h"

class CSceneViewport
{
private:
	friend class CScene;
	friend class CSceneManager;
	static bool SortWindow(CSharedPtr<CUIWindow> src, CSharedPtr<CUIWindow> dest);
	static bool SortWindowInv(CSharedPtr<CUIWindow> src, CSharedPtr<CUIWindow> dest);
protected:
	class CScene* m_Owner;
	CSharedPtr<class CUIWidget>	m_CollisionWidget;
	bool		m_Start;
	std::vector<CSharedPtr<CUIWindow>>	m_vecWindow;
	CSceneViewport();
	~CSceneViewport();
public:
	void LoadComplete();
	void Start();
	bool Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void Render();
	void Save(FILE* file);
	void Load(FILE* file);
	bool CollisionMouse();
	void GetAllWindowHierarchyName(std::vector<HierarchyWindowName>& vecName);
	template <typename T>
	T* FindUIWindow(const std::string& name)
	{
		size_t	size = m_vecWindow.size();
		for (size_t i = 0; i < size; ++i)
		{
			//if (m_vecWindow[i]->GetName() == name)
			if (m_vecWindow[i]->GetWindowTypeName() == name)
			{
				return (T*)m_vecWindow[i].Get();
			}
		}
		return nullptr;
	}
	template <typename T>
	T* CreateUIWindow(const std::string& name)
	{
		T* window = FindUIWindow<T>(name);
		if (window)
		{
			return window;
		}
		window = new T;
		window->SetName(name);
		window->m_Owner = this;
		window->m_Scene = m_Owner;
		if (!window->Init())
		{
			SAFE_DELETE(window);
			return nullptr;
		}
		m_vecWindow.push_back((CUIWindow*)window);
		if (m_Start)
		{
			window->Start();
		}
		return window;
	}
};

