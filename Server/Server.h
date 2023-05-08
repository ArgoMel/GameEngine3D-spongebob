#pragma once
#pragma comment(lib, "ws2_32.lib") //¸µÄ¿

#define PORT 8080 
#define PACKET_SIZE 1024

#include <iostream> // c++ stadard input output
#include <winsock2.h> // winsock
#include <windows.h> // ZeroMemory
#include <vector> //vector
#include <utility> //pair
#include <thread> //thread
#include <cstdlib>

class CLIENT 
{
public:
	SOCKET client;
	SOCKADDR_IN clientaddr = { 0 };
	int clientsize = sizeof clientaddr;
	int number = -1;
	CLIENT() { }
};

typedef std::pair<CLIENT, std::string> pii;

std::vector<pii> Client;