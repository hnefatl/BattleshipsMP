#include <iostream>

#include "Server.h"

Server::Server(bool &Run)
	:Run(Run)
{

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

void Server::Start()
{
	std::vector<Client> Accepted;
	while(Run)
	{
		Client New;
		if(!Accept(&New))
		{
			std::cout<<"Failed to accept Client."<<std::endl;
			continue;
		}
		Accepted.push_back(New);

		if(Accepted.size()>1)
		{
			// Create Game and store it
			Game NewGame(std::vector<Client>(Accepted.begin(), Accepted.begin()+1), GameSettings);
			Games.push_back(NewGame);
			Accepted.erase(Accepted.begin(), Accepted.begin()+2);

			// Start Game
			Games[Games.size()-1].Start(Run);

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

bool Server::Accept(Client *Buffer)
{
	Buffer=new Client();
	sockaddr_storage Storage;
	socklen_t Size=sizeof(Storage);

	while(Run)
	{
		Buffer->ClientSocket=accept(ServerSocket, (sockaddr *)&Storage, &Size);
		if(Buffer->ClientSocket!=-1)
		{
			break;
		}
	}

	if(!Receive(Buffer, Buffer->Username))
	{
		return false;
	}

	return true;
}