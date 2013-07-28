#include "Networking.h"

#include "Util.h"

bool Send(SOCKET Target, std::string Message)
{
	int MessageSize=Message.size();
	int MessageSizeSize=ToString(MessageSize).length();

	// Send size of size indicator
	if(!SendPlain(Target, ToString(MessageSizeSize)))
	{
		return false;
	}
	// Send size indicator
	if(!SendPlain(Target, ToString(MessageSize)))
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

bool Receive(SOCKET Target, std::string *Buffer)
{
	unsigned int MessageSizeSize;
	unsigned int MessageSize;
	std::string Temp;

	// Receive MessageSizeSize
	if(!ReceivePlain(Target, 1, &Temp))
	{
		return false;
	}
	MessageSizeSize=atoi(Temp.c_str());
	Temp.clear();

	// Receive MessageSize
	if(!ReceivePlain(Target, MessageSizeSize, &Temp))
	{
		return false;
	}
	MessageSize=atoi(Temp.c_str());
	Temp.clear();

	// Receive Message
	if(!ReceivePlain(Target, MessageSize, &Temp))
	{
		return false;
	}

	// Swap buffers
	*Buffer=Temp;

	return true;
}
bool ReceivePlain(SOCKET Target, unsigned int Length, std::string *Buffer)
{
	// Receive a message of Length from Server
	std::string Received;
	unsigned int AmountReceived=0;

	while(AmountReceived!=Length)
	{
		std::string TempString;
		int Temp=recv(Target, &TempString[0], Length-AmountReceived, NULL);
		if(Temp<0)
		{
			return false;
		}
		else
		{
			TempString=std::string(TempString.c_str());
			Received+=std::string(TempString.begin(), TempString.begin()+Temp); // Trim excess characters
			AmountReceived+=Temp;
		}
	}
	// Swap buffers
	*Buffer=std::string(Received.c_str());

	return true;
}