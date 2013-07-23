#include "Game.h"

#include "Networking.h"

Game::Game(std::vector<Client> Players, BattleshipSettings Settings)
	:Players(Players),
	Settings(Settings)
{
	Log("Constructed");
}
Game::~Game()
{
	Log("Disposed");
	delete Handler;
	Handler=NULL;
}

void Game::Start(bool &Run)
{
	if(Players.size()==2)
	{
		Log("Game started");
		Handler=new std::thread(&Game::Play, this, Run);
	}
	else
	{
		LogError("Not enough players");
		Finished=true;
	}
}

bool Game::IsFinished()
{
	return Finished;
}
std::vector<std::string> Game::GetLog()
{
	return MessageLog;
}

void Game::Play(bool &Run)
{
	// Transmit opponent names
	for(unsigned int x=0; x<Players.size(); x++)
	{
		if(!Send(&Players[x], Players[1-x].Username))
		{
			LogError("Opponent name transmit failed");
		}
	}
	Log("Opponent names transmitted");
	// Upload settings data
	UploadSettings();
	// Send Flag (trigger to Place ships)
	for(unsigned int x=0; x<Players.size(); x++)
	{
		if(!Send(&Players[x], "1"))
		{
			LogError("");
		}
	}

	// Receive Flag (Clients have placed ships)
	for(unsigned int x=0; x<Players.size(); x++)
	{
		std::string Flag;
		Receive(&Players[x], Flag);
	}

	// Receive boards
	for(unsigned int x=0; x<Players.size(); x++)
	{
		std::string Board;
		Receive(&Players[x], Board);
	}

	while(Run)
	{

	}
}

void Game::Log(std::string Message)
{
	MessageLog.push_back(Message);
}
void Game::LogError(std::string Error)
{
	Log("Error: "+Error);
}

bool Game::UploadSettings()
{
	std::vector<std::string> GameSettings=Settings.EnumerateSettings();
	for(unsigned int x=0; x<Players.size(); x++)
	{
		for(unsigned int y=0; y<GameSettings.size(); y++)
		{
			if(!Send(&Players[x], GameSettings[y]))
			{
				return false;
			}
		}
	}
	return true;
}