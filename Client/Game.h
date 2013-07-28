#ifndef _GAME_H
#define _GAME_H

#include <vector>
#include <string>

class Game
{
public:
	Game();
	~Game();

	bool Run();

private:
	std::vector<std::vector<Cell>> Board;
};

#endif