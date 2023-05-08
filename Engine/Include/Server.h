#pragma once
#include "EngineInfo.h"
#include "UI/RapidXml/CXmlParser.h"

class CServer
{
private:
	typedef std::pair<CServer, std::string> pii;
	SOCKET client;
	SOCKADDR_IN clientaddr;
	int clientsize;
	int number;
	DialogWriteInfo m_Info;
	char m_Name[PACKET_SIZE];
	char m_Message[PACKET_SIZE];
	std::vector<pii> Client;
public:
	CServer();
	~CServer();
	void Init();
	void Update();
	void recvData(SOCKET s, int num);
	void ACCEPT(SOCKET& s);
	void SetText(const char* name, const char* text);
};