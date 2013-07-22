#ifndef _NETWORKING_H
#define _NETWORKING_H

#include <string>
#include <WS2tcpip.h>
#include <winsock.h>
#include "Client.h"

bool Send(SOCKET Target, std::string Message);
bool SendPlain(SOCKET Target, std::string Message);
bool Send(Client *Target, std::string Message);
bool SendPlain(Client *Target, std::string Message);
bool Receive(SOCKET Target, std::string &Buffer);
bool ReceivePlain(SOCKET Target, unsigned int Length, std::string &Buffer);
bool Receive(Client *Target, std::string &Buffer);
bool ReceivePlain(Client *Target, unsigned int Length, std::string &Buffer);

#endif