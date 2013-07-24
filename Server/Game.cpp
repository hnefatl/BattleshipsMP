#include "Game.h"

#include "Networking.h"
#include "Cell.h"

Game::Game(std::vector<Client> Players, BattleshipSettings Settings)
	:Players(Players),
	Settings(Settings),
	Finished(false)
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
		if(!Send(Players[x].ClientSocket, Players[1-x].Username))
		{
			LogError("Opponent name send failed to "+x);
			Shutdown();
		}
	}
	Log("Opponent names transmitted");
	// Upload settings data
	UploadSettings();
	// Send Flag (trigger to Place ships)
	for(unsigned int x=0; x<Players.size(); x++)
	{
		if(!Send(Players[x].ClientSocket, "1"))
		{
			LogError("Flag send failed for player "+x);
			Shutdown();
		}
	}

	// Receive Flag (Clients have placed ships)
	for(unsigned int x=0; x<Players.size(); x++)
	{
		std::string Flag;
		if(!Receive(Players[x].ClientSocket, &Flag))
		{
			LogError("Flag receive failed for player "+x);
			Shutdown();
		}
	}

	// Receive boards
	for(unsigned int x=0; x<Players.size(); x++)
	{
		std::string Board;
		if(!Receive(Players[x].ClientSocket, &Board))
		{
			LogError("Board receive failed for Player "+x);
			Shutdown();
		}
		if(Board.size()!=Settings.Width*Settings.Height)
		{
			LogError("Board size invalid for Player "+x);
			Shutdown();
		}

		// Store Board
		Players[x].Board.resize(Settings.Height);
		for(unsigned int y=0; y<Players[x].Board.size(); y++)
		{
			Players[x].Board[y].resize(Settings.Width);
		}

		for(unsigned int a=0; a<Players[x].Board.size(); a++)
		{
			for(unsigned int b=0; b<Players[x].Board[a].size(); b++)
			{
				Players[x].Board[a][b]=(Cell)((int)Board[(a*Settings.Width)+b]);
			}
		}
	}

	while(Run)
	{

	}
}

void Game::Shutdown()
{
	for(unsigned int x=0; x<Players.size(); x++)
	{
		Log("Disconnect: "+x);
		Send(Players[x].ClientSocket, Settings.DisconnectString);
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
			if(!Send(Players[x].ClientSocket, GameSettings[y]))
			{
				return false;
			}
		}
	}
	return true;
}