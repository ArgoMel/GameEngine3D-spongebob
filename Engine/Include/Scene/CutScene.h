#pragma once
#include "../EngineInfo.h"

class CCutScene
{
private:
    IGraphBuilder* m_GraphBuilder;   // 필터 그래프 매니저 역할을 하는 인터페이스
    IMediaControl* m_MediaControl;   // 재생/일시정지/정지 등을 수행하기 위한 인터페이스
    IMediaSeeking* m_MediaSeeking;   // 재생 위치를 읽어오거나 변경하기 위한 인터페이스
    IMediaEventEx* m_MediaEventEx;   // 필터 그래프로부터 이벤트를 받기 위한 인터페이스
    IVideoWindow*  m_VidWin;         // 창조절하는 인터페이스
    //IVMRWindowlessControl*  m_Wc;     
    void StopDirectShow();
    int  Stop();
    HRESULT InitWindowlessVMR(
        HWND hwndApp,                  // Window to hold the video. 
        IGraphBuilder* pGraph,         // Pointer to the Filter Graph Manager. 
        IVMRWindowlessControl** ppWc   // Receives a pointer to the VMR.
    );
public:
    CCutScene();
    ~CCutScene();
    bool StartDirectShow(const TCHAR* fileName);
};

