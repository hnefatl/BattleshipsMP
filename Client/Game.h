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
#include "ThreadSignal.h"

class Game
{
public:
	Game();

	bool Start();

private:
	int Connect();

	bool PlaceShips();

	bool Play();

private:
	SOCKET Client, Server;

	std::string Username, Opponent;

	BattleshipSettings Settings;

	std::vector<std::vector<Cell>> Board;

	std::string Address, Port;

	bool DownloadSettings();

	void TimerFunction(unsigned int Time, ThreadSignal<bool> *Signal, std::mutex *Mutex);
};

#endif