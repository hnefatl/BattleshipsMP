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

	void PlaceShips();

	bool Send(std::string Message);
	bool SendPlain(std::string Message);
	bool Receive(std::string &Buffer);
	bool ReceivePlain(unsigned int Length, std::string &Buffer);

private:
	int Client, Server;

	std::string Username, Opponent;

	BattleshipSettings Settings;

	std::vector<std::vector<Cell>> Board;

	std::string Address;
	int Port;

	std::string DisconnectionString;

	bool DownloadSettings();

	void TimerFunction(unsigned int Time, bool &Signal, std::mutex *Mutex);
};

#endif