#include <iostream>
#include <conio.h>

#include "GameLauncher.h"

int main(int argc, char *argv[])
{
	GameLauncher Battleships;
	if(Battleships.Start())
	{
		std::cout<<"Press any key to exit...";
		_getch();
	}
	return 0;
}