#ifndef _SERVER_H
#define _SERVER_H

#include <vector>
#include <thread>
#include <mutex>

#include "Client.h"
#include "Game.h"
#include "BattleshipSettings.h"
#include "Networking.h"

class Server
{
public:
	Server(bool &Run);

	bool LoadSettings(std::string PathToSettings);
	bool Init(std::string Port);

	void Start(unsigned int Backlog);

private:
	void Shutdown();

	void RunGame(Client *One, Client *Two);

	bool Listen();
	bool Accept(Client *Buffer);

private:
	SOCKET ServerSocket;

	BattleshipSettings GameSettings;
	std::vector<Game> Games;

	bool &Run;
	unsigned int Backlog;
};

#endif