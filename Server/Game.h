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
	~Game();

	void Start(bool &Run);

	bool IsFinished();
	std::vector<std::string> GetLog();
private:
	void Play(bool &Run);

	bool UploadSettings();

	void Log(std::string Message);
	void LogError(std::string Error);
private:
	std::vector<Client> Players;
	std::vector<std::string> MessageLog;
	BattleshipSettings Settings;

	std::thread *Handler;

	bool Finished;
};

#endif