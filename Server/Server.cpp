#include "Server.h"

void recvData(SOCKET s, int num)
{ 
	char buffer[PACKET_SIZE] = { 0 };
	recv(s, buffer, sizeof buffer, 0); //이름받기
	Client[num].second = buffer;
	while (TRUE) 
	{
		ZeroMemory(buffer, sizeof buffer);
		recv(s, buffer, sizeof buffer, 0); //메세지를 받는부분
		std::cout << Client[num].second << " : " << buffer << std::endl;
	}
}

void ACCEPT(SOCKET &s) 
{ //클라이언트들의 접속을 수용하는 함수
	int cnt = 0;
	while (TRUE) 
	{
		Client.push_back(pii(CLIENT(), ""));
		Client[cnt].first.client = accept(s, (SOCKADDR*)&Client[cnt].first.clientaddr, &Client[cnt].first.clientsize);
		Client[cnt].first.number = cnt;
		std::thread(recvData, Client[cnt].first.client, cnt).detach();
		cnt += 1;
	}
}

int main()
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

	std::thread(ACCEPT, std::ref(server)).detach();

	char name[PACKET_SIZE], message[PACKET_SIZE];

	while (TRUE)
	{
		ZeroMemory(name, sizeof name);
		ZeroMemory(message, sizeof message);
		std::cin >> name >> message;
		for (size_t i = 0; i < Client.size(); i++)
		{
			if (!strcmp(Client[i].second.c_str(), name))
			{
				send(Client[i].first.client, message, sizeof message, 0);
			}
		}
	}
}