#pragma once
#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow : public CRef
{
private:
	friend class CSceneViewport;
	friend class CScene;
	friend class CSceneManager;
	static std::unordered_map<std::string, CUIWindow*>	m_mapUIWindowCDO;
	static bool SortWidget(CSharedPtr<CUIWidget> src, CSharedPtr<CUIWidget> dest);
	static bool SortWidgetInv(CSharedPtr<CUIWidget> src, CSharedPtr<CUIWidget> dest);
protected:
	class CScene* m_Scene;
	class CSceneViewport* m_Owner;
	std::string		m_WindowTypeName;
	Vector2	m_Pos;
	Vector2	m_Size;
	int     m_ZOrder;
	bool	m_Start;
	std::vector<CSharedPtr<CUIWidget>>	m_vecWidget;
	CUIWindow();
	CUIWindow(const CUIWindow& window);
	virtual ~CUIWindow();
public:
	static void AddUIWindowCDO(const std::string& name, CUIWindow* CDO)
	{
		m_mapUIWindowCDO.insert(std::make_pair(name, CDO));
	}
	static CUIWindow* FindCDO(const std::string& name)
	{
		auto	iter = m_mapUIWindowCDO.find(name);
		if (iter == m_mapUIWindowCDO.end())
		{
			return nullptr;
		}
		return iter->second;
	}
	const std::string& GetWindowTypeName()	const
	{
		return m_WindowTypeName;
	}
	class CSceneViewport* GetViewport()	const
	{
		return m_Owner;
	}
	const Vector2& GetPos()	const
	{
		return m_Pos;
	}
	const Vector2& GetSize()	const
	{
		return m_Size;
	}
	int GetZOrder() const
	{
		return m_ZOrder;
	}
	int GetWidgetCount()	const
	{
		return (int)m_vecWidget.size();
	}
	void SetPos(const Vector2& pos)
	{
		m_Pos = pos;
	}
	void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}
	void SetSize(const Vector2& size)
	{
		m_Size = size;
	}
	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}
	void SetZOrder(int zOrder)
	{
		m_ZOrder = zOrder;
	}
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CUIWindow* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	CUIWidget* CollisionMouse(const Vector2& mousePos);
	void GetAllWidgetHierarchyName(std::vector<HierarchyWidgetName>& vecName);
	template <typename T>
	T* FindWidget(const std::string& name)
	{
		size_t	size = m_vecWidget.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (m_vecWidget[i]->GetName() == name)
			{
				return (T*)m_vecWidget[i].Get();
			}
		}
		return nullptr;
	}
	template <typename T>
	T* CreateWidget(const std::string& name)
	{
		T* widget = FindWidget<T>(name);
		if (widget)
		{
			return widget;
		}
		widget = new T;
		widget->SetName(name);
		widget->m_Owner = this;
		widget->m_Scene = m_Scene;
		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}
		m_vecWidget.push_back((CUIWidget*)widget);
		return widget;
	}
};

