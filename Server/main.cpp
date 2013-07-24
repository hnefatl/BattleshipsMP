#include <iostream>
#include <conio.h>

#include "Server.h"

int main(int argc, char *argv[])
{
	bool Run=true;
	Server MainServer(Run);
	if(!MainServer.Init("34652"))
	{
		std::cout<<"Server failed to initialise. Press any key to continue..."<<std::endl;
		_getch();
		return -1;
	}
	std::cout<<"Server initialised."<<std::endl;
	if(!MainServer.LoadSettings("Settings.txt"))
	{
		std::cout<<"Failed to load settings. Press any key to continue..."<<std::endl;
		_getch();
		return -1;
	}
	std::cout<<"Settings loaded."<<std::endl;

	std::cout<<"Starting server."<<std::endl;
	MainServer.Start(100);

	return 0;
}