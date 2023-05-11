#include "Engine.h"
#include "Device.h"
#include "Timer.h"
#include "Input.h"
#include "PathManager.h"
#include "CollisionManager.h"
#include "SkillManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Shader/GlobalConstantBuffer.h"
#include "Resource/Shader/StructuredBuffer.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "Editor/EditorGUIManager.h"
#include "Thread/ThreadManager.h"
#include "Component/CameraComponent.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
DEFINITION_SINGLE(CEngine)
std::function<bool(HWND, UINT, WPARAM, LPARAM)> CEngine::m_WndProcFunc;
float	g_DeltaTime;
bool CEngine::m_Loop = true;
bool CEngine::m_EditorMode = false;

CEngine::CEngine()
    : m_hInst(0)
    , m_hWnd(0)
    , m_WindowRS{}
    , m_ClearColor{ 0.f,0.f,0.f,0.f }
    , m_TimeScale(1.f)
    , m_Render2D(false)
    , m_AccTime(0.f)
{
    m_Setting = new CEngineSetting;
    m_Timer = new CTimer;
    m_GlobalCBuffer = new CGlobalConstantBuffer;
    m_RandomBuffer = new CStructuredBuffer;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(25913462);
    srand((unsigned int)time(0));
    int a=rand();
}

CEngine::~CEngine()
{
    if (m_RandomBuffer)
    {
        m_RandomBuffer->ResetShader(51, (int)EShaderBufferType::All);
    }
    SAFE_DELETE(m_RandomBuffer);
    SAFE_DELETE(m_GlobalCBuffer);
    CSceneManager::DestroyInst();
    CEditorGUIManager::DestroyInst();
    CCollisionManager::DestroyInst();
    CRenderManager::DestroyInst();
    CInput::DestroyInst();
    CThreadManager::DestroyInst();
    CPathManager::DestroyInst();
    CResourceManager::DestroyInst();
    CSkillManager::DestroyInst();
    SAFE_DELETE(m_Setting);
    SAFE_DELETE(m_Timer);
    CDevice::DestroyInst();
}

float CEngine::GetFPS() const
{
    return m_Timer->GetFPS();
}

void CEngine::SetCameraAxisX(const Vector3& axis)
{
    m_GlobalCBuffer->SetCameraAxisX(axis);
}

void CEngine::SetCameraAxisY(const Vector3& axis)
{
    m_GlobalCBuffer->SetCameraAxisY(axis);
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR* title, const TCHAR* className, int iconID, int smallIconID,
    unsigned int windowWidth, unsigned int windowHeight, unsigned int deviceWidth, unsigned int deviceHeight, 
    bool windowMode, int menuID)
{
    m_hInst = hInst;
    m_WindowRS.width = windowWidth;
    m_WindowRS.height = windowHeight;
    Register(className, iconID, smallIconID,menuID);
    Create(title, className);
    m_Setting->Init();
    if (!CDevice::GetInst()->Init(m_hWnd, deviceWidth, deviceHeight, windowMode))
    {
        return false;
    }
    if (!CPathManager::GetInst()->Init())
    {
        return false;
    }
    if (!CRenderManager::GetInst()->Init())
    {
        return false;
    }
    if (!CResourceManager::GetInst()->Init())
    {
        return false;
    }
    CRenderManager::GetInst()->CreateRenderTarget();
    if (!CCollisionManager::GetInst()->Init())
    {
        return false;
    }
    if (!CInput::GetInst()->Init(m_hInst, m_hWnd))
    {
        return false;
    }
    if (m_EditorMode)
    {
        if (!CEditorGUIManager::GetInst()->Init(m_hWnd))
        {
            return false;
        }
    }
    if (!CThreadManager::GetInst()->Init())
    {
        return false;
    }
    if (!CSceneManager::GetInst()->Init())
    {
        return false;
    }
    if (!CSkillManager::GetInst()->Init())
    {
        return false;
    }
    m_Timer->Init();
    m_GlobalCBuffer->Init();
    CTexture* texture = CResourceManager::GetInst()->FindTexture("EngineNoise");
    m_GlobalCBuffer->SetNoiseResolution((float)texture->GetWidth(), (float)texture->GetHeight());
    //랜덤한 숫자를 가진 벡터 생성
    std::vector<int>	vecRand;
    vecRand.resize(1024 * 1024);
    for (int i = 0; i < 1024 * 1024; ++i)
    {
        short	num1 = (short)rand();
        short	num2 = (short)rand();
        int	random = num1;
        random <<= 16;
        random |= num2;
        vecRand[i] = random;
    }
    m_RandomBuffer->Init("Random", sizeof(int), 1024 * 1024, 51, true, (int)EShaderBufferType::All);
    m_RandomBuffer->UpdateBuffer(&vecRand[0], 1024 * 1024);
    m_RandomBuffer->SetShader(51, (int)EShaderBufferType::All);
    return true;
}

int CEngine::Run()
{
    MSG msg = {};
    while (m_Loop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Logic();
        }
    }
    return (int)msg.wParam;
}

void CEngine::Exit()
{
    DestroyWindow(m_hWnd);
}

void CEngine::Logic()
{
    m_Timer->Update();
    g_DeltaTime = m_Timer->GetDeltaTime()* m_TimeScale;
    m_AccTime += g_DeltaTime;

    Resolution	RS = CDevice::GetInst()->GetResolution();
    Resolution	shadowMapRS = CRenderManager::GetInst()->GetShadowMapResolution();
    m_GlobalCBuffer->SetDeltaTime(g_DeltaTime);
    m_GlobalCBuffer->SetAccTime(m_AccTime);
    m_GlobalCBuffer->SetResolution((float)RS.width, (float)RS.height);
    m_GlobalCBuffer->SetShadowMapResolution((float)shadowMapRS.width, (float)shadowMapRS.height);

    CInput::GetInst()->Update(g_DeltaTime);
    CSkillManager::GetInst()->Update(g_DeltaTime);
    if (m_EditorMode)
    {
        CEditorGUIManager::GetInst()->Update(g_DeltaTime);
    }
    CResourceManager::GetInst()->Update();
    if(Update(g_DeltaTime))
    {
        if (m_EditorMode)
        {
            CEditorGUIManager::GetInst()->Render();
        }
        return;
    }
    if(PostUpdate(g_DeltaTime))
    {
        if (m_EditorMode)
        {
            CEditorGUIManager::GetInst()->Render();
        }
        return;
    }
    Collision(g_DeltaTime);
    Render(g_DeltaTime);
}

bool CEngine::Update(float deltaTime)
{
    bool result = CSceneManager::GetInst()->Update(deltaTime);
    return result;
}

bool CEngine::PostUpdate(float deltaTime)
{
    bool result = CSceneManager::GetInst()->PostUpdate(deltaTime);
    return result;
}

bool CEngine::Collision(float deltaTime)
{
    return CSceneManager::GetInst()->Collision(deltaTime);
}

void CEngine::Render(float deltaTime)
{
    CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
    if (camera)
    {
        m_GlobalCBuffer->SetCameraAxisX(camera->GetWorldAxis(AXIS::AXIS_X));
        m_GlobalCBuffer->SetCameraAxisY(camera->GetWorldAxis(AXIS::AXIS_Y));
    }

    m_GlobalCBuffer->UpdateBuffer();
    CDevice::GetInst()->ClearRenderTarget(m_ClearColor);
    CDevice::GetInst()->ClearDepthStencil(1.f, 0);
    CDevice::GetInst()->RenderStart();
    CRenderManager::GetInst()->Render(deltaTime);
    if (m_EditorMode)
    {
        CEditorGUIManager::GetInst()->Render();
    }
    CDevice::GetInst()->Flip();
    CInput::GetInst()->SetWheel(0);
}

void CEngine::Register(const TCHAR* className, int iconID, int smallIconID, int menuID)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(iconID));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (menuID != 0)
    {
        wcex.lpszMenuName = MAKEINTRESOURCEW(menuID);
    }
    else
    {
        wcex.lpszMenuName = nullptr;
    }
    wcex.lpszClassName = className;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(smallIconID));
    RegisterClassExW(&wcex);
}

bool CEngine::Create(const TCHAR* title, const TCHAR* className)
{
    m_hWnd = CreateWindowW(className, title, WS_OVERLAPPEDWINDOW,100, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);
    if (!m_hWnd)
    {
        return false;
    }
    RECT    rc = { 0, 0, (LONG)m_WindowRS.width, (LONG)m_WindowRS.height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    MoveWindow(m_hWnd, 640, 0, rc.right - rc.left,rc.bottom - rc.top, TRUE);
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_EditorMode)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        {
            return 1;
        }
    }
    if (m_WndProcFunc)
    {
        if (m_WndProcFunc(hWnd, message, wParam, lParam))
        {
            return 1;
        }
    }
    switch (message)
    {
    case WM_MOUSEWHEEL:
    {
        // wParam의 상위 16비트를 얻어온다.
        // 	위로 굴리면 1 아래로 -1
        short	Wheel = HIWORD(wParam);
        CInput::GetInst()->SetWheel(Wheel / 120);
    }
        break;
    case WM_DESTROY:
        m_Loop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
