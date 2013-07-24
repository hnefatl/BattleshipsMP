#include "Game.h"
#include "ConsoleControl.h"
#include "ShipPlacer.h"
#include "Networking.h"

Game::Game()
{

}

bool Game::Start()
{
	WSAData Data;
	if(WSAStartup(MAKEWORD(1, 1), &Data)!=0)
	{
		return false;
	}

	// Get logon
	std::cout<<"Enter your username: ";
	std::getline(std::cin, Username);
	Clear();

	// Connect
	while(true)
	{
		std::cout<<"Enter the Server address: ";
		std::cin>>Address;
		if(Address=="exit")
		{
			return false;
		}
		std::cout<<"Enter the Server port: ";
		std::cin>>Port;

		std::cout<<"Connecting...";
		int ConnectionResult=Connect();
		if(ConnectionResult==-1)
		{
			std::cout<<"Fatal: Failed to load WSA."<<std::endl;
			return true;
		}
		else if (ConnectionResult==1)
		{
			std::cout<<"Invalid IP or Port."<<std::endl;
		}
		else if (ConnectionResult==2)
		{
			std::cout<<"Failed to connect."<<std::endl;
		}
		else
		{
			break;
		}

		std::cout<<std::endl;
	}

	// Now connected
	std::cout<<"Connected."<<std::endl;
	// Send Username
	if(!Send(Server, Username))
	{
		std::cout<<"Connection lost."<<std::endl;
		return true;
	}
	//Receive username of other Player
	std::cout<<"Waiting for other player to connect...";
	if(!Receive(Server, &Opponent))
	{
		std::cout<<"Connection lost."<<std::endl;
		return true;
	}
	std::cout<<Opponent<<" connected.";

	// Set board
	std::cout<<"Receiving Game data...";
	if(!DownloadSettings())
	{
		std::cout<<"Connection lost."<<std::endl;
		return true;
	}
	std::cout<<"Done."<<std::endl;

	// Place ships (timed)
	if(!PlaceShips())
	{
		std::cout<<"Failed to send Board."<<std::endl;
		return true;
	}

	return false;
}

bool Game::DownloadSettings()
{
	std::string Buffer;

	// Board Width
	if(!Receive(Server, &Buffer))
		return false;
	Settings.Width=atoi(Buffer.c_str());

	// Board height
	if(!Receive(Server, &Buffer))
		return false;
	Settings.Height=atoi(Buffer.c_str());

	// Time for placing ships
	if(!Receive(Server, &Buffer))
		return false;
	Settings.TimeToPlaceShips=atoi(Buffer.c_str());

	// Time per turn
	if(!Receive(Server, &Buffer))
		return false;
	Settings.TimePerTurn=atoi(Buffer.c_str());

	// Touching ships
	if(!Receive(Server, &Buffer))
		return false;
	Settings.TouchingShips=(atoi(Buffer.c_str())!=0);

	// Disconnect String
	if(!Receive(Server, &Buffer))
		return false;
	Settings.DisconnectString=Server, Buffer;

	// EmptyBackColour
	if(!Receive(Server, &Buffer))
		return false;
	Settings.EmptyBackColour=(ConsoleColour)atoi(Buffer.c_str());

	// EmptyForeColour
	if(!Receive(Server, &Buffer))
		return false;
	Settings.EmptyForeColour=(ConsoleColour)atoi(Buffer.c_str());

	// ShipBackColour
	if(!Receive(Server, &Buffer))
		return false;
	Settings.ShipBackColour=(ConsoleColour)atoi(Buffer.c_str());

	// ShipForeColour
	if(!Receive(Server, &Buffer))
		return false;
	Settings.ShipForeColour=(ConsoleColour)atoi(Buffer.c_str());

	// StrikeBackColour
	if(!Receive(Server, &Buffer))
		return false;
	Settings.StrikeBackColour=(ConsoleColour)atoi(Buffer.c_str());

	// StrikeForeColour
	if(!Receive(Server, &Buffer))
		return false;
	Settings.StrikeForeColour=(ConsoleColour)atoi(Buffer.c_str());

	// IllegalBackColour
	if(!Receive(Server, &Buffer))
		return false;
	Settings.IllegalBackColour=(ConsoleColour)atoi(Buffer.c_str());

	// IllegalForeColour
	if(!Receive(Server, &Buffer))
		return false;
	Settings.IllegalForeColour=(ConsoleColour)atoi(Buffer.c_str());

	// Allowed numbers of ships
	for(unsigned int x=0; x<5; x++)
	{
		if(!Receive(Server, &Buffer))
			return false;
		Settings.ShipsAllowed[x]=atoi(Buffer.c_str());
	}

	return true;
}

bool Game::PlaceShips()
{
	Clear();

	std::mutex *ConsoleLock=new std::mutex();
	bool Signal=false;

	ShipPlacer Placer(Settings);
	std::thread TimerThread(&Game::TimerFunction, this, Settings.TimeToPlaceShips, Signal, ConsoleLock);
	// Start a ShipPlacer running
	std::thread PlacingThread(&ShipPlacer::Run, Placer, Signal, ConsoleLock);

	// Wait for both threads to terminate
	if(TimerThread.joinable())
		TimerThread.join();
	if(PlacingThread.joinable())
		PlacingThread.join();

	// Ships placed, send Board to Server
	std::string StringBoard;
	for(unsigned int y=0; y<Board.size(); y++)
	{
		for(unsigned int x=0; x<Board[y].size(); x++)
		{
			StringBoard+=(char)((int)Board[y][x]);
		}
	}
	if(!Send(Server, StringBoard))
	{
		return false;
	}

	return true;
}

void Game::TimerFunction(unsigned int Time, bool &Signal, std::mutex *Mutex)
{
	if(Time==0)
	{
		// Deactivate Signal off, then exit
		Signal=false;
	}
	else
	{
		Signal=false;
		// Get timer start time
		clock_t Start=clock();

		int SecondsPassed=0;
		while(true)
		{
			// If current time (seconds) is greater than start time (seconds) + length of wait
			if((unsigned)clock()/CLOCKS_PER_SEC>(Start/CLOCKS_PER_SEC)+Time)
			{
				break;
			}
			// Draw time
			Mutex->lock();

			SetCursor(0, 0);
			// Clear enough space for 10 digit time
			std::cout<<"                     ";
			SetCursor(0, 0);
			std::cout<<"Time left: "<<((Start/CLOCKS_PER_SEC)+Time)-(clock()/CLOCKS_PER_SEC);

			Mutex->unlock();

			// Sleep for 0.05 second
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		// Activate Signal
		Signal=true;
	}
}

int Game::Connect()
{
	addrinfo *ServerInfo, Hints;

	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family=AF_UNSPEC;
	Hints.ai_socktype=SOCK_STREAM;

	int Rcv;
	if((Rcv=getaddrinfo(Address.c_str(), Port.c_str(), &Hints, &ServerInfo))!=0)
	{
		return false;
	}

	addrinfo *p=NULL;
	for(p=ServerInfo; p!=NULL; p=p->ai_next)
	{
		if((Server=socket(p->ai_family, p->ai_socktype, p->ai_protocol))==-1)
		{
			continue;
		}

		if(connect(Server, p->ai_addr, p->ai_addrlen)==-1)
		{
			continue;
		}

		break;
	}
	if(p==NULL)
	{
		return 2;
	}

	freeaddrinfo(ServerInfo);

	return 0;
}