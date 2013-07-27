#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

#include "Networking.h"

int main(int argc, char *argv[])
{
	WSAData Data;

	if(WSAStartup(MAKEWORD(1, 1), &Data)!=0)
	{
		return false;
	}

	std::cout<<"TestNetworking: "<<TestNetworking()<<std::endl;

	_getch();
	WSACleanup();
	return 0;
}