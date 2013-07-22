#include <iostream>
#include <conio.h>

#include "Game.h"

int main(int argc, char *argv[])
{
	Game Battleships;
	if(Battleships.Start())
	{
		std::cout<<"Press any key to exit...";
		_getch();
	}
	return 0;
}