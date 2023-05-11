#include "EditorManager.h"
#include "CollisionManager.h"
#include "PathManager.h"
#include "Input.h"
#include "Scene/EditorDefaultScene.h"
#include "Scene/LoadingSceneInfo.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneViewport.h"
#include "Scene/Scene.h"
#include "Window/ObjectWindow.h"
#include "Window/ClassWindow.h"
#include "Window/ComponentWindow.h"
#include "Window/TransformWindow.h"
#include "Window\SceneWindow.h"
#include "Window\DetailWindow.h"
#include "Window/AnimationWindow.h"
#include "Window/Animation2DWindow.h"
#include "Window/ColiderWindow.h"
#include "Window/ColliderPixelWindow.h"
#include "Window/CameraWindow.h"
#include "Window/StatusWindow.h"
#include "Window/UIButtonWindow.h"
#include "Window/UIImageWindow.h"
#include "Window/UINumberWindow.h"
#include "Window/UIProgressBarWindow.h"
#include "Window/UITextWindow.h"
#include "Window/FBXWindow.h"
#include "Window/PathWindow.h"
#include "Window/MaterialWindow.h"
#include "Window/MeshWindow.h"
#include "Window/TerrainWindow.h"
#include "Window/ChatWindow.h"
#include "Editor/EditorGUIManager.h"
#include "Setting/EngineShareSetting.h"

CEditorManager::CEditorManager()
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

bool CEditorManager::Init(HINSTANCE hInst)
{
    CEngine::GetInst()->EnableEditor();
    if (!CEngine::GetInst()->Init(hInst, TEXT("KBJ41_Editor"), TEXT("KBJ41_Editor"), IDI_ICON1, IDI_ICON1
        , 1280, 720, 1920, 1080, true, IDR_MENU1))
    {
        return false;
    }
    CSceneInfo* info = new CEditorDefaultScene;
    CScene::AddSceneInfoCDO("EditorDefaultScene", info);
    CEngineShareSetting::Setting();
    CEngine::SetWndProcCallback<CEditorManager>(this, &CEditorManager::WndProc);
    CSceneManager::GetInst()->CreateSceneInfo<CEditorDefaultScene>();

    WindowOnOff();
    WindowOnOff();
    CInput::GetInst()->AddBindFunction<CEditorManager>
        ("CtrlShiftC", Input_Type::Down, this, &CEditorManager::ChatGPT, CSceneManager::GetInst()->GetScene());
    CInput::GetInst()->AddBindFunction<CEditorManager>
        ("CtrlShiftT", Input_Type::Down, this, &CEditorManager::ChatWindow, CSceneManager::GetInst()->GetScene());
    CInput::GetInst()->AddBindFunction<CEditorManager>
        ("ShiftF1", Input_Type::Down, this, &CEditorManager::WindowOnOff, CSceneManager::GetInst()->GetScene());
    return true;
}

int CEditorManager::Run()
{
	return CEngine::GetInst()->Run();
}

bool CEditorManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_FILE_SCENE_SAVE:
            SaveScene();
            return true;
        case ID_FILE_SCENE_OPEN:
            OpenScene();
            return true;
        case ID_FILE_EXIT:
            DestroyWindow(hWnd);
            return true;
        case ID_OBJECT_EMPTY:
            CreateEmptyObject();
            return true;
        case ID_OBJECT:
            CreateObject();
            return true;
        case ID_WINDOW_OBJECT:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ObjectWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("ObjectWindow");
            }
        }
            return true;
        case ID_Window_Scene:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("SceneWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("SceneWindow");
            }
        }
            return true;
        case ID_Window_Detail:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("DetailWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("DetailWindow");
            }
        }
            return true;
        case ID_Window_Component:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ComponentWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("ComponentWindow");
            }
        }
            return true;
        case ID_Window_Camera:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("CameraWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CCameraWindow>("CameraWindow");
            }
        }
            return true;
        case ID_Window_Collider2D:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ColliderWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CColiderWindow>("ColliderWindow");
            }
        }
            return true;
        case ID_Window_ColliderPixel:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ColliderPixelWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CColliderPixelWindow>("ColliderPixelWindow");
            }
        }
            return true;
        case ID_Window_BUTTON:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UIButtonWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUIButtonWindow>("UIButtonWindow");
            }
        }
            return true;
        case ID_Window_UIImage:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UIImageWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUIImageWindow>("UIImageWindow");
            }
        }
            return true;  
        case ID_Window_NUMBER:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UINumberWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUINumberWindow>("UINumberWindow");
            }
        }
            return true;  
        case ID_Window_PROGRESSBAR:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UIProgressBarWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
            }
        }
            return true;
        case ID_Window_TEXT:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UITextWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUITextWindow>("UITextWindow");
            }
        }
            return true;
        case ID_Window_Animation:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("AnimationWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CAnimationWindow>("AnimationWindow");
            }
        }
            return true;
        case ID_Window_Animation2D:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("Animation2DWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CAnimation2DWindow>("Animation2DWindow");
            }
        }
            return true;
        }
        break;
    }
    return false;
}

void CEditorManager::SaveScene()
{
}

void CEditorManager::OpenScene()
{
}

void CEditorManager::CreateEmptyObject()
{
    CScene* scene = CSceneManager::GetInst()->GetScene();
    CGameObject* emptyObj = scene->CreateObject<CGameObject>("GameObjectEmpty");
    CObjectWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
    if (window)
    {
        window->AddItem(emptyObj,"GameObjectEmpty");
    }
}

void CEditorManager::CreateObject()
{
    CClassWindow* ClassWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CClassWindow>("ClassWindow");
    if (ClassWindow)
    {
        ClassWindow->ObjectCreateCallback();
    }
}

void CEditorManager::ChatGPT()
{
    CObjectWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
    if (window)
    {
        window->Pause();
    }
    const PathInfo* info = CPathManager::GetInst()->FindPath(CHATGPT_PATH);
    //tchar 변환
    char	fullPath[_MAX_EXT] = {};
    TCHAR t_filename[_MAX_EXT] = {};
    strcpy_s(fullPath, info->pathMultibyte);
    strcat_s(fullPath, "\\ChatGPT.exe");
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fullPath, (int)strlen(fullPath), t_filename, 256);
    ShellExecute(NULL, _T("open"), t_filename, NULL, NULL, SW_SHOW);
    //ShellExecute( NULL, _T( "edit" ), _T( "C:\\TEST.txt" ), NULL, NULL, SW_SHOW ); 편집기능
}

void CEditorManager::ChatWindow()
{
    CChatWindow* chatWnd = CEditorGUIManager::GetInst()->FindEditorWindow<CChatWindow>("ChatWindow");
    if(!chatWnd)
    {
        CEditorGUIManager::GetInst()->CreateEditorWindow<CChatWindow>("ChatWindow");
    }
    else if(chatWnd->GetOpen())
    {
        chatWnd->Close();
    }
    else
    {
        chatWnd->Open();
    }
}

void CEditorManager::WindowOnOff()
{
    CObjectWindow* objWnd = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
    if(CEditorGUIManager::GetInst()->IsWindowEmpty())
    {
        objWnd = CEditorGUIManager::GetInst()->CreateEditorWindow<CObjectWindow>("ObjectWindow");
        CEditorGUIManager::GetInst()->CreateEditorWindow<CClassWindow>("ClassWindow");
        CEditorGUIManager::GetInst()->CreateEditorWindow<CComponentWindow>("ComponentWindow");
        CEditorGUIManager::GetInst()->CreateEditorWindow<CTransformWindow>("TransformWindow");
        CEditorGUIManager::GetInst()->CreateEditorWindow<CSceneWindow>("SceneWindow");
        CEditorGUIManager::GetInst()->CreateEditorWindow<CPathWindow>("PathWindow");
        CEditorGUIManager::GetInst()->CreateEditorWindow<CStatusWindow>("StatusWindow");
        CEditorGUIManager::GetInst()->CreateEditorWindow<CMaterialWindow>("MaterialWindow");
        CEditorGUIManager::GetInst()->CreateEditorWindow<CMeshWindow>("MeshWindow");
        //CEditorGUIManager::GetInst()->FindEditorWindow<CDetailWindow>("DetailWindow");
        //시작하자마자 현재씬의 컴포넌트들 출력
        if (objWnd)
        {
            objWnd->AddItemList(CSceneManager::GetInst()->GetScene());
        }
    }
    else if(objWnd)
    {
        CEditorGUIManager::GetInst()->OnOffEditorWindow(!objWnd->GetOpen());
    }


}
