#include "UIWidget.h"
#include "UIWindow.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Component/CameraComponent.h"
std::unordered_map<std::string, CUIWidget*> CUIWidget::m_mapUIWidgetCDO;
CUIConstantBuffer* CUIWidget::m_CBuffer = nullptr;
CAnimation2DConstantBuffer* CUIWidget::m_AnimCBuffer = nullptr;

CUIWidget::CUIWidget()
    : m_Owner(nullptr)
    , m_Scene(nullptr)
    , m_ZOrder(0)
    , m_Angle(0.f)
    , m_Opacity(1.f)
    , m_Start(false)
    , m_Tint(1.f, 1.f, 1.f, 1.f)
    , m_MouseHovered(false)
{
    m_WidgetTypeName = "UIWidget";
}

CUIWidget::CUIWidget(const CUIWidget& widget)
    : CRef(widget)
{
    m_ZOrder = widget.m_ZOrder;
    m_Pos = widget.m_Pos;
    m_RenderPos = widget.m_RenderPos;
    m_Size = widget.m_Size;
    m_Angle = widget.m_Angle;
    m_Opacity = widget.m_Opacity;
    m_Mesh = widget.m_Mesh;
    m_Shader = widget.m_Shader;
    m_Tint = widget.m_Tint;
    m_MeshSize = widget.m_MeshSize;
    m_MouseHovered = false;
}

CUIWidget::~CUIWidget()
{
}

void CUIWidget::SetShader(const std::string& name)
{
    if (m_Scene)
    {
        m_Shader = m_Scene->GetResource()->FindShader(name);
    }
    else
    {
        m_Shader = CResourceManager::GetInst()->FindShader(name);
    }
}

void CUIWidget::SetMesh(const std::string& name)
{
    if (m_Scene)
    {
        m_Mesh = m_Scene->GetResource()->FindMesh(name);
    }
    else
    {
        m_Mesh = CResourceManager::GetInst()->FindMesh(name);
    }
    Vector3 meshSize = m_Mesh->GetMax() - m_Mesh->GetMin();
    m_MeshSize = Vector2(meshSize.x, meshSize.y);
}

void CUIWidget::Start()
{
}

bool CUIWidget::Init()
{
    if (m_Scene)
    {
        m_Mesh = m_Scene->GetResource()->FindMesh("LBUVRect");
        m_Shader = m_Scene->GetResource()->FindShader("UIShader");
    }
    else
    {
        m_Mesh = CResourceManager::GetInst()->FindMesh("LBUVRect");
        m_Shader = CResourceManager::GetInst()->FindShader("UIShader");
    }
    Vector3 meshSize = m_Mesh->GetMax() - m_Mesh->GetMin();
    m_MeshSize = Vector2(meshSize.x, meshSize.y);
    return true;
}

void CUIWidget::Update(float deltaTime)
{
    
}

void CUIWidget::PostUpdate(float deltaTime)
{
    m_RenderPos = m_Owner->GetPos() + m_Pos;
}

void CUIWidget::Render()
{
    m_CBuffer->SetMeshSize(m_MeshSize);
    m_CBuffer->SetOpacity(m_Opacity);
    m_CBuffer->SetPivot(m_Pivot);
    m_CBuffer->SetTint(m_Tint);
    CCameraComponent* camera = m_Scene->GetCameraManager()->GetUICamera();
    Matrix  matProj = camera->GetProjMatrix();
    Matrix  matScale, matRot, matTranslate;
    matScale.Scaling(m_Size);
    matRot.RotationZ(m_Angle);
    matTranslate.Translation(m_RenderPos);
    Matrix  matWorld = matScale * matRot * matTranslate;
    Matrix  matWVP = matWorld * matProj;
    m_CBuffer->SetWVP(matWVP);
    m_CBuffer->UpdateBuffer();
    m_AnimCBuffer->UpdateBuffer();
    m_Shader->SetShader();
    m_Mesh->Render();
}

CUIWidget* CUIWidget::Clone()
{
    return nullptr;
}

void CUIWidget::Save(FILE* file)
{
    fwrite(&m_ZOrder, sizeof(int), 1, file);
    fwrite(&m_Pos, sizeof(Vector2), 1, file);
    fwrite(&m_RenderPos, sizeof(Vector2), 1, file);
    fwrite(&m_Size, sizeof(Vector2), 1, file);
    fwrite(&m_Pivot, sizeof(Vector2), 1, file);
    fwrite(&m_MeshSize, sizeof(Vector2), 1, file);
    fwrite(&m_Angle, sizeof(float), 1, file);
    fwrite(&m_Opacity, sizeof(float), 1, file);
    fwrite(&m_Tint, sizeof(Vector4), 1, file);
    int	length = (int)m_Mesh->GetName().length();
    fwrite(&length, 4, 1, file);
    fwrite(m_Mesh->GetName().c_str(), 1, length, file);
    length = (int)m_Shader->GetName().length();
    fwrite(&length, 4, 1, file);
    fwrite(m_Shader->GetName().c_str(), 1, length, file);
}

void CUIWidget::Load(FILE* file)
{
    fread(&m_ZOrder, sizeof(int), 1, file);
    fread(&m_Pos, sizeof(Vector2), 1, file);
    fread(&m_RenderPos, sizeof(Vector2), 1, file);
    fread(&m_Size, sizeof(Vector2), 1,file);
    fread(&m_Pivot, sizeof(Vector2), 1, file);
    fread(&m_MeshSize, sizeof(Vector2), 1, file);
    fread(&m_Angle, sizeof(float), 1, file);
    fread(&m_Opacity, sizeof(float), 1, file);
    fread(&m_Tint, sizeof(Vector4), 1, file);
    int	length = 0;
    char    meshName[256] = {};
    char    shaderName[256] = {};
    fread(&length, 4, 1, file);
    assert(length >= 0);
    fread(meshName, 1, length, file);
    length = 0;
    fread(&length, 4, 1, file);
    assert(length >= 0);
    fread(shaderName, 1, length, file);
    if (m_Scene)
    {
        m_Mesh = m_Scene->GetResource()->FindMesh(meshName);
        m_Shader = m_Scene->GetResource()->FindShader(shaderName);
    }
    else
    {
        m_Mesh = CResourceManager::GetInst()->FindMesh(meshName);
        m_Shader = CResourceManager::GetInst()->FindShader(shaderName);
    }
}

bool CUIWidget::CollisionMouse(const Vector2& mousePos)
{
    Vector2 pos = m_Owner->GetPos() + m_Pos - m_Pivot * m_Size;
    if (pos.x > mousePos.x)
    {
        return false;
    }
    else if (pos.x + m_Size.x < mousePos.x)
    {
        return false;
    }
    else if (pos.y > mousePos.y)
    {
        return false;
    }
    else if (pos.y + m_Size.y < mousePos.y)
    {
        return false;
    }
    return true;
}