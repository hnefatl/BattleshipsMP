#ifndef _CLIENT_H
#define _CLIENT_H

#include <string>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

class Client
{
public:
	Client(SOCKET ClientSocket);

	std::string Username;
	SOCKET ClientSocket;
};

#endif