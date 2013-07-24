#ifndef _GAME_H
#define _GAME_H

#include <iostream>
#include <conio.h>
#include <string>
#include <vector>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

#include <thread>
#include <mutex>

#include "BattleshipSettings.h"
#include "KeyMonitor.h"
#include "Cell.h"

class Game
{
public:
	Game();

	bool Start();

private:
	int Connect();

	bool PlaceShips();

private:
	SOCKET Client, Server;

	std::string Username, Opponent;

	BattleshipSettings Settings;

	std::vector<std::vector<Cell>> Board;

	std::string Address, Port;

	bool DownloadSettings();

	void TimerFunction(unsigned int Time, bool &Signal, std::mutex *Mutex);
};

#endif