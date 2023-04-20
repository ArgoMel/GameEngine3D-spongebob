#include "CutScene.h"
#include "../Engine.h"
#include "../Device.h"
#include "../PathManager.h"

CCutScene::CCutScene()
	: m_GraphBuilder(nullptr)
	, m_MediaControl(nullptr)
	, m_MediaSeeking(nullptr)
	, m_MediaEventEx(nullptr)
	, m_VidWin(nullptr)
{
}

CCutScene::~CCutScene()
{
	m_GraphBuilder = NULL;
	m_MediaControl = NULL;
	m_MediaSeeking = NULL;
	m_MediaEventEx = NULL;
	m_VidWin = NULL;
}

bool CCutScene::StartDirectShow(const TCHAR* fileName)
{
    HRESULT hr = 0;
    if (m_GraphBuilder)
    {
        return false;
    }
    // Initialize the COM library.
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        StopDirectShow();
        MessageBox(0, TEXT("ERROR !"), TEXT("ERROR - Could not initialize COM library"), 0);
        return false;
    }
    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_GraphBuilder);
    if (FAILED(hr))
    {
        StopDirectShow();
        MessageBox(0, TEXT("ERROR !"), TEXT("ERROR - Could not create the Filter Graph Manager."), 0);
        return false;
    }

    IVMRWindowlessControl* pWc = NULL;
    hr = InitWindowlessVMR(CEngine::GetInst()->GetWindowHandle(), m_GraphBuilder, &pWc);
    if (SUCCEEDED(hr))
    {
        hr = m_GraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_MediaControl);
        hr = m_GraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_MediaEventEx);
        // Build the graph. For example:
        const PathInfo* info = CPathManager::GetInst()->FindPath(CUTSCENE_PATH);
        TCHAR path[MAX_PATH] = {};
        _tcscpy_s(path, MAX_PATH, info->path);
        TCHAR* text = new TCHAR[MAX_PATH];
        _tcscpy_s(text, MAX_PATH, fileName);
        _tcscat_s(path, MAX_PATH, text);
        SAFE_DELETE_ARRAY(text);
        hr = m_GraphBuilder->RenderFile(path, NULL);
        //ThrowIfError(hr);
        if (FAILED(hr))
        {
            //MessageBox(0, TEXT("ERROR - Could not Find the request file to play."), TEXT("ERROR !"), 0);
            return false;
        }
        // Find the native video size.
        long lWidth, lHeight;
        HRESULT hr = pWc->GetNativeVideoSize(&lWidth, &lHeight, NULL, NULL);
        if (SUCCEEDED(hr))
        {
            RECT rcSrc, rcDest;
            // Set the source rectangle.
            SetRect(&rcSrc, 0, 0, lWidth, lHeight);
            // Get the window client area.
            GetClientRect(CEngine::GetInst()->GetWindowHandle(), &rcDest);
            // Set the destination rectangle.
            SetRect(&rcDest, 0, 0, rcDest.right, rcDest.bottom);
            // Set the video position.
            hr = pWc->SetVideoPosition(&rcSrc, &rcDest);
        }
        if (SUCCEEDED(hr))
        {
            // Run the graph.
            hr = m_MediaControl->Run();
            long evCode = 0;
            m_MediaEventEx->WaitForCompletion(INFINITE, &evCode);
        }
        // Release the VMR interface when you are done.
        pWc->Release();
        StopDirectShow();
    }
    return true;
}

void CCutScene::StopDirectShow()
{
    if (m_GraphBuilder) 
    {
        m_GraphBuilder->Release();
        m_GraphBuilder = NULL;
    }
    if (m_MediaControl) 
    {
        m_MediaControl->Release();
        m_MediaControl = NULL;
    }
    if (m_MediaEventEx) 
    {
        m_MediaEventEx->Release();
        m_MediaEventEx = NULL;
    }
    CoUninitialize();
}

HRESULT CCutScene::InitWindowlessVMR(HWND hwndApp, IGraphBuilder* pGraph, IVMRWindowlessControl** ppWc)
{
    if (!pGraph || !ppWc)
    {
        return E_POINTER;
    }
    IBaseFilter* pVmr = NULL;
    IVMRWindowlessControl* pWc = NULL;
    // Create the VMR. 
    HRESULT hr = CoCreateInstance(CLSID_VideoMixingRenderer, NULL,
        CLSCTX_INPROC, IID_IBaseFilter, (void**)&pVmr);
    if (FAILED(hr))
    {
        return hr;
    }

    // Add the VMR to the filter graph.
    hr = pGraph->AddFilter(pVmr, L"Video Mixing Renderer");
    if (FAILED(hr))
    {
        pVmr->Release();
        return hr;
    }
    // Set the rendering mode.  
    IVMRFilterConfig* pConfig;
    hr = pVmr->QueryInterface(IID_IVMRFilterConfig, (void**)&pConfig);
    if (SUCCEEDED(hr))
    {
        hr = pConfig->SetRenderingMode(VMRMode_Windowless);
        pConfig->Release();
    }
    if (SUCCEEDED(hr))
    {
        // Set the window. 
        hr = pVmr->QueryInterface(IID_IVMRWindowlessControl, (void**)&pWc);
        if (SUCCEEDED(hr))
        {
            hr = pWc->SetVideoClippingWindow(hwndApp);
            if (SUCCEEDED(hr))
            {
                *ppWc = pWc; // Return this as an AddRef'd pointer. 
            }
            else
            {
                // An error occurred, so release the interface.
                pWc->Release();
            }
        }
    }
    pVmr->Release();
    return hr;
}

int CCutScene::Stop()
{
    if (m_GraphBuilder)
    {
        m_GraphBuilder->Release();
        m_GraphBuilder = NULL;
    }
    if (m_MediaControl)
    {
        m_MediaControl->Release();
        m_MediaControl = NULL;
    }
    if (m_MediaEventEx)
    {
        m_MediaEventEx->Release();
        m_MediaEventEx = NULL;
    }
    CoUninitialize();
    return 0;
}
