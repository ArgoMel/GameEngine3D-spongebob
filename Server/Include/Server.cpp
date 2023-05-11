#include "Server.h"
//#include <winsock2.h> // winsock

CServer::CServer()
	: clientaddr{ 0 }
	, clientsize(sizeof clientaddr)
	, number(-1)
{

}

CServer::~CServer()
{

}

void CServer::Init()
{
	WSADATA wsa;
	int temp = WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	bind(server, (SOCKADDR*)&addr, sizeof addr);
	listen(server, SOMAXCONN);

	std::thread(&CServer::ACCEPT, this, std::ref(server)).detach();

	while(true)
	{
		for (size_t i = 0; i < Client.size(); i++)
		{
			if (!strcmp(Client[i].second.c_str(), m_Name))
			{
				send(Client[i].first.client, m_Message, sizeof m_Message, 0);
			}
		}
		ZeroMemory(m_Name, sizeof m_Name);
		ZeroMemory(m_Message, sizeof m_Message);
	}
}

void CServer::SetText(const char* name, const char* text)
{
	memset(m_Name, 0, PACKET_SIZE);
	memset(m_Message, 0, PACKET_SIZE);
	strcpy_s(m_Name, name);
	strcpy_s(m_Message, text);
	m_Info.talker = m_Name;
	m_Info.text = m_Message;
	//int length = (int)MultiByteToWideChar(CP_ACP, 0, text, -1, nullptr, 0);
	//MultiByteToWideChar(CP_ACP, 0, text, -1, m_wText, length);
	//length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, nullptr, 0, nullptr, nullptr);
	//WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, length, nullptr, nullptr);
	for (size_t i = 0; i < Client.size(); i++)
	{
		if (!strcmp(Client[i].second.c_str(), m_Name))
		{
			CXmlParser* parser = new CXmlParser;
			parser->AddFile(m_Message);
			SAFE_DELETE(parser)
			send(Client[i].first.client, m_Message, sizeof m_Message, 0);
		}
	}
	ZeroMemory(m_Name, sizeof m_Name);
	ZeroMemory(m_Message, sizeof m_Message);
}

void CServer::recvData(SOCKET s, int num)
{
	char buffer[PACKET_SIZE] = { 0 };
	recv(s, buffer, sizeof buffer, 0); //이름받기
	Client[num].second = buffer;
	while (TRUE)
	{
		ZeroMemory(buffer, sizeof buffer);
		recv(s, buffer, sizeof buffer, 0); //메세지를 받는부분
		CXmlParser* parser = new CXmlParser;
		parser->AddFile(Client[num].second);
		SAFE_DELETE(parser)
	}
}
//클라이언트들의 접속을 수용하는 함수
void CServer::ACCEPT(SOCKET& s)
{
	int cnt = 0;
	while (TRUE)
	{
		Client.push_back(pii(CServer(), ""));
		Client[cnt].first.client = accept(s, (SOCKADDR*)&Client[cnt].first.clientaddr, &Client[cnt].first.clientsize);
		Client[cnt].first.number = cnt;
		std::thread(&CServer::recvData, this, Client[cnt].first.client, cnt).detach();
		cnt += 1;
	}
}

int main()
{
	CServer* server = new CServer;
	server->Init();
	SAFE_DELETE(server)
	return 0;
}