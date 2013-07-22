#ifndef _GAMEPAIR_H
#define _GAMEPAIR_H

#include <thread>
#include <mutex>

#include "Client.h"

class GamePair
{
public:
	GamePair(Client *One, Client *Two);
	~GamePair();

private:
	Client *One, *Two;
	std::thread Runner;
};

#endif