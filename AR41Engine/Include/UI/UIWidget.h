#pragma once
#include "../Ref.h"
#include "../Resource/Shader/UIConstantBuffer.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Resource/Texture/Texture.h"

struct UIWidgetImageInfo
{
    CSharedPtr<class CTexture>	texture;
    Vector4		tint;
    std::vector<Animation2DFrameData>   vecFrameData;
    int     frame;
    float   time;
    float   frameTime;
    float   playTime;
    float   playScale;
    UIWidgetImageInfo() 
        : tint(Vector4::white)
        , frame(0)
        , time(0.f)
        , frameTime(0.f)
        , playTime(1.f)
        , playScale(1.f)
    {
    }
};

class CUIWidget : public CRef
{
private:
    friend class CUIWindow;
    friend class CResourceManager;
    friend class CScene;
    friend class CSceneManager;
    friend class CSceneViewport;
    static std::unordered_map<std::string, CUIWidget*>	m_mapUIWidgetCDO;
protected:
    static CUIConstantBuffer* m_CBuffer;
    static CAnimation2DConstantBuffer* m_AnimCBuffer;
    class CScene* m_Scene;
    class CUIWindow* m_Owner;
    CSharedPtr<class CShader>   m_Shader;
    CSharedPtr<class CMesh>     m_Mesh;
    Vector2	m_Pos;
    Vector2	m_RenderPos;
    Vector2	m_Size;
    Vector2 m_Pivot;
    Vector2 m_MeshSize;
    Vector4 m_Tint;
    std::string		m_WidgetTypeName;
    int     m_ZOrder;
    float   m_Angle;
    float   m_Opacity;
    bool	m_Start;
    bool    m_MouseHovered;
    CUIWidget();
    CUIWidget(const CUIWidget& widget);
    virtual ~CUIWidget() = 0;
public:
    static void AddUIWidgetCDO(const std::string& name, CUIWidget* CDO)
    {
        m_mapUIWidgetCDO.insert(std::make_pair(name, CDO));
    }
    static CUIWidget* FindCDO(const std::string& name)
    {
        auto	iter = m_mapUIWidgetCDO.find(name);
        if (iter == m_mapUIWidgetCDO.end())
        {
            return nullptr;
        }
        return iter->second;
    }
    class CUIWindow* GetWindow()	const
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
    const Vector4& GetTint()    const
    {
        return m_Tint;
    }
    const std::string& GetWidgetTypeName()	const
    {
        return m_WidgetTypeName;
    }
    int GetZOrder() const
    {
        return m_ZOrder;
    }
    float GetAngle()    const
    {
        return m_Angle;
    }
    float GetOpacity()  const
    {
        return m_Opacity;
    }
    bool GetMouseHovered()  const
    {
        return m_MouseHovered;
    }
    void SetShader(const std::string& name);
    void SetMesh(const std::string& name);
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
    void SetPivot(const Vector2& pivot)
    {
        m_Pivot = pivot;
    }
    void SetPivot(float x, float y)
    {
        m_Pivot.x = x;
        m_Pivot.y = y;
    }
    void SetTint(const Vector4& tint)
    {
        m_Tint = tint;
    }
    void SetTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        m_Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    }
    void SetWidgetTypeName(const std::string& name)
    {
        m_WidgetTypeName = name;
    }
    void SetZOrder(int zOrder)
    {
        m_ZOrder = zOrder;
    }
    void SetOpacity(float opacity)
    {
        m_Opacity = opacity;
    }
    void AddOpacity(float opacity)
    {
        m_Opacity += opacity;
    }
    void SetAngle(float angle)
    {
        m_Angle = angle;
    }
    void AddPos(const Vector2& pos)
    {
        m_Pos += pos;
    }
    void AddPos(float x, float y)
    {
        m_Pos.x += x;
        m_Pos.y += y;
    }
    void AddSize(const Vector2& size)
    {
        m_Size += size;
    }
    void AddSize(float x, float y)
    {
        m_Size.x += x;
        m_Size.y += y;
    }
    void AddAngle(float angle)
    {
        m_Angle += angle;
    }
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CUIWidget* Clone();
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
    virtual bool CollisionMouse(const Vector2& mousePos);
};

