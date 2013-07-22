#include "Client.h"

Client::Client()
	:ClientSocket(-1)
{

}
Client::Client(SOCKET ClientSocket)
	:ClientSocket(ClientSocket)
{

}
Client::Client(SOCKET ClientSocket, std::string Username)
	:ClientSocket(ClientSocket),
	Username(Username)
{

}