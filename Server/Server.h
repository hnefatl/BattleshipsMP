#ifndef _SERVER_H
#define _SERVER_H

#include <vector>
#include <thread>
#include <mutex>

#include "Client.h"
#include "GamePair.h"

class Server
{
public:
	Server();

	bool Init(std::string Port);

	void Start(bool &Run);

	void Shutdown();

private:

	void Run(Client One, Client Two);


	bool Accept(Client *Buffer);

	bool Send(SOCKET Target, std::string Message);
	bool SendPlain(SOCKET Target, std::string Message);
	bool Send(Client Target, std::string Message);
	bool SendPlain(Client Target, std::string Message);
	bool Receive(SOCKET Target, std::string &Buffer);
	bool ReceivePlain(SOCKET Target, unsigned int Length, std::string &Buffer);
	bool Receive(Client Target, std::string &Buffer);
	bool ReceivePlain(Client Target, unsigned int Length, std::string &Buffer);

private:
	SOCKET ServerSocket;

	std::vector<GamePair> Pairs;
};

#endif