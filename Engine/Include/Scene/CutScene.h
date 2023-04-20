#pragma once
#include "../EngineInfo.h"

class CCutScene
{
private:
    IGraphBuilder* m_GraphBuilder;   // ���� �׷��� �Ŵ��� ������ �ϴ� �������̽�
    IMediaControl* m_MediaControl;   // ���/�Ͻ�����/���� ���� �����ϱ� ���� �������̽�
    IMediaSeeking* m_MediaSeeking;   // ��� ��ġ�� �о���ų� �����ϱ� ���� �������̽�
    IMediaEventEx* m_MediaEventEx;   // ���� �׷����κ��� �̺�Ʈ�� �ޱ� ���� �������̽�
    IVideoWindow*  m_VidWin;         // â�����ϴ� �������̽�
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

