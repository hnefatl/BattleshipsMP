#include <iostream>
#include <fstream>

#include "Server.h"

Server::Server(bool &Run)
	:Run(Run)
{
}

bool Server::LoadSettings(std::string PathToSettings)
{
	std::ifstream In(PathToSettings);
	if(!In.good())
	{
		return false;
	}

	try
	{
		std::string Buffer;
		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.Width=atoi(Buffer.c_str());							// Width

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.Height=atoi(Buffer.c_str());							//Height

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.TimeToPlaceShips=atoi(Buffer.c_str());					// Time to place ships

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.TimePerTurn=atoi(Buffer.c_str());						// Time per turn

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.TouchingShips=atoi(Buffer.c_str())!=0;					// Whether ships can touch or not

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.DisconnectString=Buffer;								// Disconnection string

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.EmptyBackColour=(ConsoleColour)atoi(Buffer.c_str());	// Empty Back colour

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.EmptyForeColour=(ConsoleColour)atoi(Buffer.c_str());	// Empty Fore colour

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.ShipBackColour=(ConsoleColour)atoi(Buffer.c_str());	// Ship Back colour

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.ShipForeColour=(ConsoleColour)atoi(Buffer.c_str());	// Ship Fore colour

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.StrikeBackColour=(ConsoleColour)atoi(Buffer.c_str());	// Strike Back colour

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.StrikeForeColour=(ConsoleColour)atoi(Buffer.c_str());	// Strike Fore colour

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.IllegalBackColour=(ConsoleColour)atoi(Buffer.c_str());	// Illegal back colour

		std::getline(In, Buffer);
		Buffer=Split(Buffer, ':')[1];
		GameSettings.IllegalForeColour=(ConsoleColour)atoi(Buffer.c_str());	// Illegal Fore colour

		for(unsigned int x=0; x<5; x++)
		{
			std::getline(In, Buffer);
			Buffer=Split(Buffer, ':')[1];
			GameSettings.ShipsAllowed[x]=atoi(Buffer.c_str());				// Ships allowed per size
		}
	}
	catch(std::exception)
	{
		return false;
	}

	return true;
}

bool Server::Init(std::string Port)
{
	WSAData Data;

	if(WSAStartup(MAKEWORD(1, 1), &Data)!=0)
	{
		return false;
	}

	addrinfo Hints, *ServerInfo;

	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family=AF_INET;
	Hints.ai_socktype=SOCK_STREAM;
	Hints.ai_protocol=IPPROTO_TCP;
	Hints.ai_flags=AI_PASSIVE;

	int Rcv;
	if((Rcv=getaddrinfo(NULL, Port.c_str(), &Hints, &ServerInfo))!=0)
	{
		Shutdown();
		return false;
	}

	char yes=1;
	addrinfo *p;
	for(p=ServerInfo; p!=NULL; p=p->ai_next)
	{
		if((ServerSocket=socket(p->ai_family, p->ai_socktype, p->ai_protocol))==-1)
		{
			continue;
		}

		if(setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1)
		{
			Shutdown();
			return false;
		}

		if(bind(ServerSocket, p->ai_addr, p->ai_addrlen)==-1)
		{
			closesocket(ServerSocket);
			continue;
		}

		break;
	}

	if(p==NULL)
	{
		return false;
	}

	freeaddrinfo(ServerInfo);

	return true;
}

void Server::Shutdown()
{
	WSACleanup();
}

void Server::Start(unsigned int Backlog)
{
	std::vector<Client> Accepted;

	Listen();
	while(Run)
	{
		Client New;
		std::cout<<"Awaiting clients...";
		if(!Accept(&New))
		{
			std::cout<<"Failed to accept Client."<<std::endl;
			continue;
		}
		std::cout<<"Client accepted: "+New.Username<<std::endl;
		Accepted.push_back(New);

		if(Accepted.size()>1)
		{
			// Create Game and store it
			Game NewGame(std::vector<Client>(Accepted.begin(), Accepted.begin()+2), GameSettings);
			Games.push_back(NewGame);
			Accepted.erase(Accepted.begin(), Accepted.begin()+2);

			// Start Game
			Games[Games.size()-1].Start(Run);
			std::cout<<"New Game started."<<std::endl;

			// Dispose of finished games
			for(int x=0; x<(int)Games.size(); x++)
			{
				if(Games[(unsigned)x].IsFinished())
				{
					Games.erase(Games.begin()+x);
					x--;
				}
			}
		}
	}
}

bool Server::Listen()
{
	if(!listen(ServerSocket, Backlog))
	{
		return false;
	}

	return true;
}

bool Server::Accept(Client *Buffer)
{
	Client Temp;
	sockaddr_storage Storage;
	socklen_t Size=sizeof(Storage);

	while(Run)
	{
		Temp.ClientSocket=accept(ServerSocket, (sockaddr *)&Storage, &Size);
		if(Temp.ClientSocket!=-1)
		{
			break;
		}
	}

	if(!Receive(Temp.ClientSocket, &Temp.Username))
	{
		return false;
	}

	*Buffer=Temp;

	return true;
}

std::vector<std::string> Server::Split(std::string Input, char Delimiter)
{
	std::vector<std::string> Segments;
	std::string Temp;
	for(unsigned int x=0; x<Input.size(); x++)
	{
		if(Input[x]==Delimiter)
		{
			if(Temp.size()>0)
			{
				Segments.push_back(Temp);
				Temp.clear();
			}
		}
		else
		{
			Temp+=Input[x];
		}
	}
	if(Temp.size()>0)
	{
		Segments.push_back(Temp);
	}
	return Segments;
}