#ifndef _GAME_H
#define _GAME_H

#include <vector>
#include <string>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

#include <thread>
#include <mutex>

#include "BattleshipSettings.h"
#include "Client.h"

class Game
{
public:
	Game(std::vector<Client> Players, BattleshipSettings Settings);

	void Start(bool &Run);

	bool IsFinished();

private:

	void Play(bool &Run);

private:
	std::vector<Client> Players;
	BattleshipSettings Settings;

	std::thread Handler;

	bool Finished;
};

#endif