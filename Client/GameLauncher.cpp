#include "GameLauncher.h"
#include "ConsoleControl.h"
#include "ShipPlacer.h"
#include "Networking.h"

GameLauncher::GameLauncher()
{

}

bool GameLauncher::Start()
{
	WSAData Data;
	if(WSAStartup(MAKEWORD(1, 1), &Data)!=0)
	{
		return false;
	}

#ifndef _DEBUG
	// Get logon
	std::cout<<"Enter your username: ";
	std::getline(std::cin, Username);
	Clear();
#else
	Username="hnefatl";
#endif
	// Connect
	while(true)
	{
#ifndef _DEBUG
		std::cout<<"Enter the Server address: ";
		std::cin>>Address;
		if(Address=="exit")
		{
			return false;
		}
		std::cout<<"Enter the Server port: ";
		std::cin>>Port;
#else
		Address="127.0.0.1";
		Port="34652";
#endif

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
	std::cout<<"Player "<<Opponent<<" connected."<<std::endl;

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

	if(!Play())
	{
		std::cout<<"A network error occurred."<<std::endl;
		return true;
	}

	return false;
}

bool GameLauncher::DownloadSettings()
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
	Settings.DisconnectString=Buffer;

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

bool GameLauncher::PlaceShips()
{
	Clear();
	SetCursor(false);

	std::mutex *ConsoleLock=new std::mutex();
	ThreadSignal<bool> Signal=ThreadSignal<bool>(false);

	ShipPlacer Placer(Settings);
	std::thread *TimerThread=new std::thread(&Game::TimerFunction, this, Settings.TimeToPlaceShips, &Signal, ConsoleLock);
	// Start a ShipPlacer running
	std::thread *PlacingThread=new std::thread(&ShipPlacer::Run, Placer, &Signal, ConsoleLock);

	// Wait for both threads to terminate
	if(TimerThread->joinable())
		TimerThread->join();
	if(PlacingThread->joinable())
		PlacingThread->join();

	Clear();
	// Ships placed, send Board to Server
	std::cout<<"Sending Board to server...";
	std::string StringBoard;
	for(unsigned int y=0; y<Placer.Board.size(); y++)
	{
		for(unsigned int x=0; x<Placer.Board[y].size(); x++)
		{
			StringBoard+=(char)((int)Placer.Board[y][x]);
		}
	}
	if(!Send(Server, StringBoard))
	{
		return false;
	}
	std::cout<<"Done."<<std::endl;

	return true;
}

bool GameLauncher::Play()
{
	Clear();


	return true;
}

void GameLauncher::TimerFunction(unsigned int Time, ThreadSignal<bool> *Signal, std::mutex *Mutex)
{
	// Initial draw
	Mutex->lock();

	SetCursor(0, Settings.Height+1);
	SetColour(ConsoleColour::Grey, ConsoleColour::Black);
	// Clear enough space for 10 digit time
	std::cout<<"                     ";
	SetCursor(0, Settings.Height+1);
	std::cout<<"Time left: "<<Time;

	Mutex->unlock();

	Signal->SetSignal(false);
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
		if(((Start/CLOCKS_PER_SEC)+1+SecondsPassed)<(clock()/CLOCKS_PER_SEC))
		{
			SecondsPassed++;
			// Draw time
			Mutex->lock();

			SetCursor(0, Settings.Height+1);
			SetColour(ConsoleColour::Grey, ConsoleColour::Black);
			// Clear enough space for 10 digit time
			std::cout<<"                     ";
			SetCursor(0, Settings.Height+1);
			std::cout<<"Time left: "<<Time-SecondsPassed;

			Mutex->unlock();
		}

		// Sleep for 0.05 second
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	// Activate Signal
	Signal->SetSignal(true);
}

int GameLauncher::Connect()
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