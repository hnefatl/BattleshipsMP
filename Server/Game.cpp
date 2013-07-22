#include "Game.h"

Game::Game(std::vector<Client> Players, BattleshipSettings Settings)
	:Players(Players),
	Settings(Settings)
{

}

void Game::Start(bool &Run)
{
	Handler=std::thread(&Game::Play, this, Run);
}

bool Game::IsFinished()
{
	return Finished;
}

void Play(bool &Run)
{
	while(Run)
	{

	}
}