#include "Server.h"

Server::Server()
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

void Server::Start(bool &Run)
{
	while(Run)
	{
		Client *One=NULL, *Two=NULL;

	}
}

bool Server::Accept(Client *Buffer)
{
	return true;
}

bool Server::Send(SOCKET Target, std::string Message)
{
	int MessageSize=Message.size();
	int MessageSizeSize=std::string(_itoa(MessageSize, NULL, 10)).size();

	// Send size of size indicator
	if(!SendPlain(Target, _itoa(MessageSizeSize, NULL, 10)))
	{
		return false;
	}
	// Send size indicator
	if(!SendPlain(Target, _itoa(MessageSize, NULL, 10)))
	{
		return false;
	}
	// Send body
	if(!SendPlain(Target, Message))
	{
		return false;
	}

	return true;
}
bool Server::SendPlain(SOCKET Target, std::string Message)
{
	unsigned int Sent=0;
	while(Sent!=Message.size())
	{
		int Result=send(Target, &Message[Sent], Message.size()-Sent, NULL);
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
bool Server::Send(Client Target, std::string Message)
{
	return Send(Target.ClientSocket, Message);
}
bool Server::SendPlain(Client Target, std::string Message)
{
	return SendPlain(Target.ClientSocket, Message);
}

bool Server::Receive(SOCKET Target, std::string &Buffer)
{
	unsigned int MessageSizeSize;
	unsigned int MessageSize;
	std::string Temp;

	// Receive MessageSizeSize
	if(!ReceivePlain(Target, 1, Temp))
	{
		return false;
	}
	MessageSizeSize=atoi(Temp.c_str());

	// Receive MessageSize
	if(!ReceivePlain(Target, MessageSizeSize, Temp))
	{
		return false;
	}
	MessageSize=atoi(Temp.c_str());

	// Receive Message
	if(!ReceivePlain(Target, MessageSize, Temp))
	{
		return false;
	}

	// Swap buffers
	Buffer=Temp;

	return true;
}
bool Server::ReceivePlain(SOCKET Target, unsigned int Length, std::string &Buffer)
{
	// Receive a message of Length from Server
	std::string Received;
	while(true)
	{
		if(recv(Target, &Received[Received.size()-1], Length-Received.size()-1, NULL)<0)
		{
			return false;
		}
	}
	// Swap buffers
	Buffer=Received;

	return true;
}
bool Server::Receive(Client Target, std::string &Buffer)
{
	return Receive(Target.ClientSocket, Buffer);
}
bool Server::ReceivePlain(Client Target, unsigned int Length, std::string &Buffer)
{
	return ReceivePlain(Target.ClientSocket, Length, Buffer);
}