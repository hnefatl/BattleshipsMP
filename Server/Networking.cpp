#include "Networking.h"

bool Send(SOCKET Target, std::string Message)
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
bool SendPlain(SOCKET Target, std::string Message)
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
bool Send(Client *Target, std::string Message)
{
	return Send(Target->ClientSocket, Message);
}
bool SendPlain(Client *Target, std::string Message)
{
	return SendPlain(Target->ClientSocket, Message);
}

bool Receive(SOCKET Target, std::string &Buffer)
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
bool ReceivePlain(SOCKET Target, unsigned int Length, std::string &Buffer)
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
bool Receive(Client *Target, std::string &Buffer)
{
	return Receive(Target->ClientSocket, Buffer);
}
bool ReceivePlain(Client *Target, unsigned int Length, std::string &Buffer)
{
	return ReceivePlain(Target->ClientSocket, Length, Buffer);
}