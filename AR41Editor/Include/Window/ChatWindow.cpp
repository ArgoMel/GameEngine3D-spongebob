#include "ChatWindow.h"
#include "PathManager.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorGroup.h"

CChatWindow::CChatWindow()
	: m_Chat(nullptr)
	, m_ChatRange(nullptr)
	, m_Name(nullptr)
	, m_IP(nullptr)
	, m_TextTree(nullptr)
	, m_ClickCount(0)
	, m_Server(0)
	, m_IsServer(false)
{
}

CChatWindow::~CChatWindow()
{
}

bool CChatWindow::Init()
{
	m_Name = CreateWidget<CEditorInputText>("이름", 150.f, 100.f);
	m_Name->SetInputType(EImGuiInputType::String);
	m_IP = CreateWidget<CEditorInputText>("아이피", 150.f, 100.f);
	m_IP->SetInputType(EImGuiInputType::String);
	m_IP->SetHintText("192.168.219.102");
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos= CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-28.f);
	CEditorButton* button = CreateWidget<CEditorButton>("로그인", 50.f, 52.f);
	button->SetClickCallback<CChatWindow>(this, &CChatWindow::ServerOpen);

	m_TextTree = CreateWidget<CEditorTree<class CEditorText*>>("TextTree");
	m_TextTree->SetHideName("TextTree");
	m_TextTree->SetSize(400.f, 300.f);
	m_TextTree->AddItem(nullptr, "채팅창");
	m_TextTree->SetSelectCallback<CChatWindow>(this, &CChatWindow::TreeCallback);

	m_ChatRange = CreateWidget<CEditorComboBox>("ChatRange", 80.f, 30.f);
	m_ChatRange->SetHideName("ChatRange");
	m_ChatRange->AddItem("전체");
	m_ChatRange->AddItem("친구");
	m_ChatRange->AddItem("길드");
	m_ChatRange->AddItem("개인");
	m_ChatRange->SetPrevViewName("전체");
	m_ChatRange->SetSelectIndex(0);
	m_ChatRange->SetSelectPrevViewName(true);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Chat = CreateWidget<CEditorInputText>("Chat", 256.f, 100.f);
	m_Chat->SetHideName("Chat");
	//m_Chat->SetMultiLine(true);
	m_Chat->SetInputType(EImGuiInputType::String);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("전송", 50.f, 26.f);
	button->SetClickCallback<CChatWindow>(this, &CChatWindow::SendBtnCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("초기화", 50.f, 26.f);
	button->SetClickCallback<CChatWindow>(this, &CChatWindow::Clear);
	return true;
}

void CChatWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CChatWindow::Clear()
{
	m_TextTree->Clear();
	m_TextTree->AddItem(nullptr, "채팅창");
	m_ClickCount = 0;
}

void CChatWindow::ClearSelect()
{
	m_SelectTextItem = "";
}

void CChatWindow::Delete()
{
	if(m_SelectTextItem=="")
	{
		return;
	}
	m_TextTree->DeleteItem(m_SelectTextItem);
}

bool CChatWindow::AddItem(class CEditorText* text, const std::string& name, const std::string& parentName)
{
	return m_TextTree->AddItem(text, name, parentName);
}

void CChatWindow::recvData(SOCKET s)
{
	char buffer[PACKET_SIZE];
	while (TRUE)
	{
		ZeroMemory(buffer, sizeof buffer);
		recv(s, buffer, sizeof buffer, 0);
		CEditorText* txt = m_TextTree->CreateWidget<CEditorText>("채팅창", "Server");
		std::string timeTxt = "Server : ";
		timeTxt += buffer;
		txt->SetText(timeTxt.c_str());
		txt->SetMultiLine(true);
	}
}

void CChatWindow::SendBtnCallback()
{
	struct tm t = {};
	time_t timer = time(NULL); // 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
	localtime_s(&t,&timer); // 포맷팅을 위해 구조체에 넣기
	CEditorText* txt = m_TextTree->CreateWidget<CEditorText>("채팅창", "txt"+ std::to_string(m_ClickCount++));
	std::string timeTxt = "[" + std::to_string(t.tm_hour) + " : " + std::to_string(t.tm_min);
	timeTxt += " : " + std::to_string(t.tm_sec) + "] ";
	timeTxt += "[" + m_ChatRange->GetSelectItem() + "] 플레이어 : ";

	char inputText[256];
	WideCharToMultiByte(CP_ACP, 0, m_Chat->GetTText(), _MAX_EXT, inputText, _MAX_EXT, NULL, NULL);
	std::string s = inputText;
	timeTxt += s;
	txt->SetText(timeTxt.c_str());
	txt->SetMultiLine(true);
	m_Chat->SetText("");
	if(m_IsServer)
	{
		send(m_Server, m_Name->GetTextUTF8(), sizeof m_Name->GetTextUTF8(), 0);
	}
}

void CChatWindow::ServerOpen()
{
	if(m_IsServer)
	{
		return;
	}
	m_IsServer = true;
	m_Name->ReadOnly(true);
	m_IP->ReadOnly(true);

	const PathInfo* info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	//tchar 변환
	char	fullPath[_MAX_EXT] = {};
	TCHAR t_filename[_MAX_EXT] = {};
	strcpy_s(fullPath, info->pathMultibyte);
#ifdef _DEBUG
	strcat_s(fullPath, "\\Server_Debug.exe");
#else
	strcat_s(fullPath, "\\Server.exe");
#endif // _DEBUG
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fullPath, (int)strlen(fullPath), t_filename, 256);
	ShellExecute(NULL, _T("open"), t_filename, NULL, NULL, SW_SHOW);
	//SW_SHOWMINNOACTIVE

	WSADATA wsa;
	int temp = WSAStartup(MAKEWORD(2, 2), &wsa);

	m_Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.s_addr = inet_addr(m_IP->GetText());
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	while (connect(m_Server, (SOCKADDR*)&addr, sizeof addr));
	send(m_Server, m_Name->GetTextUTF8(), sizeof m_Name->GetTextUTF8(), 0); //이름보내기

	std::thread(&CChatWindow::recvData,this, std::ref(m_Server)).detach();
}

void CChatWindow::TreeCallback(CEditorTreeItem<class CEditorText*>* node, const std::string& item)
{
	//m_SelectText = node->GetCustomData();
	m_SelectTextItem = item;
}