#include "Game.h"
#include "ConsoleControl.h"
#include "ShipPlacer.h"

Game::Game()
{
	DisconnectionString="ServerDisconnect";
}

bool Game::Start()
{
	// Get logon
	std::cout<<"Enter your username: ";
	std::getline(std::cin, Username);
	Clear();

	// Connect
	while(true)
	{
		std::cout<<"Enter the Server address: ";
		std::cin>>Address;
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
	if(!Send(Username))
	{
		std::cout<<"Connection lost."<<std::endl;
		return true;
	}
	//Receive username of other Player
	std::cout<<"Waiting for other player to connect...";
	if(!Receive(Opponent))
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
	PlaceShips();

	return false;
}

bool Game::DownloadSettings()
{
	std::string Buffer;

	// Board Width
	if(!Receive(Buffer))
		return false;
	Settings.Width=atoi(Buffer.c_str());

	// Board height
	if(!Receive(Buffer))
		return false;
	Settings.Height=atoi(Buffer.c_str());

	// Time for placing ships
	if(!Receive(Buffer))
		return false;
	Settings.TimeToPlaceShips=atoi(Buffer.c_str());

	// Time per turn
	if(!Receive(Buffer))
		return false;
	Settings.TimePerTurn=atoi(Buffer.c_str());

	// Touching ships
	if(!Receive(Buffer))
		return false;
	Settings.TouchingShips=(atoi(Buffer.c_str())!=0);

	// EmptyBackColour
	if(!Receive(Buffer))
		return false;
	Settings.EmptyBackColour=(ConsoleColour)atoi(Buffer.c_str());

	// EmptyForeColour
	if(!Receive(Buffer))
		return false;
	Settings.EmptyForeColour=(ConsoleColour)atoi(Buffer.c_str());

	// ShipBackColour
	if(!Receive(Buffer))
		return false;
	Settings.ShipBackColour=(ConsoleColour)atoi(Buffer.c_str());

	// ShipForeColour
	if(!Receive(Buffer))
		return false;
	Settings.ShipForeColour=(ConsoleColour)atoi(Buffer.c_str());

	// StrikeBackColour
	if(!Receive(Buffer))
		return false;
	Settings.StrikeBackColour=(ConsoleColour)atoi(Buffer.c_str());

	// StrikeForeColour
	if(!Receive(Buffer))
		return false;
	Settings.StrikeForeColour=(ConsoleColour)atoi(Buffer.c_str());

	// IllegalBackColour
	if(!Receive(Buffer))
		return false;
	Settings.IllegalBackColour=(ConsoleColour)atoi(Buffer.c_str());

	// IllegalForeColour
	if(!Receive(Buffer))
		return false;
	Settings.IllegalForeColour=(ConsoleColour)atoi(Buffer.c_str());

	// Allowed numbers of ships
	for(unsigned int x=0; x<5; x++)
	{
		if(!Receive(Buffer))
			return false;
		Settings.ShipsAllowed[x]=atoi(Buffer.c_str());
	}

	return true;
}

void Game::PlaceShips()
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
	if(Port<0 || Port>65536)
	{
		// Invalid port
		return false;
	}

	WSAData Data;
	if(WSAStartup(MAKEWORD(1, 1), &Data)!=0)
	{
		return -1;
	}

	addrinfo *ServerInfo, Hints;

	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family=AF_UNSPEC;
	Hints.ai_socktype=SOCK_STREAM;

	int Rcv;
	if((Rcv=getaddrinfo(Address.c_str(), Address.c_str(), &Hints, &ServerInfo))!=0)
	{
		return 1;
	}

	addrinfo *p=NULL;
	for(p=ServerInfo; p!=NULL; p=ServerInfo->ai_next)
	{
		if((Server=socket(p->ai_family, p->ai_socktype, p->ai_protocol))==-1)
		{
			continue;
		}

		if(connect(Server, p->ai_addr, p->ai_addrlen)==-1)
		{
			continue;
		}
	}
	if(p==NULL)
	{
		return 2;
	}

	char s[INET6_ADDRSTRLEN];
	inet_ntop(p->ai_family, p->ai_addr, s, sizeof(s));
	freeaddrinfo(ServerInfo);

	return 0;
}

bool Game::Send(std::string Message)
{
	int MessageSize=Message.size();
	int MessageSizeSize=std::string(_itoa(MessageSize, NULL, 10)).size();

	// Send size of size indicator
	if(!SendPlain(_itoa(MessageSizeSize, NULL, 10)))
	{
		return false;
	}
	// Send size indicator
	if(!SendPlain(_itoa(MessageSize, NULL, 10)))
	{
		return false;
	}
	// Send body
	if(!SendPlain(Message))
	{
		return false;
	}

	return true;
}
bool Game::SendPlain(std::string Message)
{
	// Send body
	unsigned int Sent=0;
	while(Sent!=Message.size())
	{
		int Result=send(Server, &Message[Sent], Message.size()-Sent, NULL);
		if(Result<0)
		{
			return false;
		}
		else
		{
			Sent+=Result;
		}
	}

	return true;
}

bool Game::Receive(std::string &Buffer)
{
	unsigned int MessageSizeSize;
	unsigned int MessageSize;
	std::string Temp;

	// Receive MessageSizeSize
	if(!ReceivePlain(1, Temp))
	{
		return false;
	}
	MessageSizeSize=atoi(Temp.c_str());

	// Receive MessageSize
	if(!ReceivePlain(MessageSizeSize, Temp))
	{
		return false;
	}
	MessageSize=atoi(Temp.c_str());

	// Receive Message
	if(!ReceivePlain(MessageSize, Temp))
	{
		return false;
	}

	// Swap buffers
	Buffer=Temp;

	return true;
}
bool Game::ReceivePlain(unsigned int Length, std::string &Buffer)
{
	// Receive a message of Length from Server
	std::string Received;
	while(true)
	{
		if(recv(Server, &Received[Received.size()-1], Length-Received.size()-1, NULL)<0)
		{
			return false;
		}
	}
	// Swap buffers
	Buffer=Received;

	return true;
}