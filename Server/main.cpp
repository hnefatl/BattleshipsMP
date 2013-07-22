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

	MainServer.Start();

	return 0;
}