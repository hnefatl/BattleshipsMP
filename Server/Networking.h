#ifndef _NETWORKING_H
#define _NETWORKING_H

#include <string>
#include <WS2tcpip.h>
#include <winsock.h>

bool Send(SOCKET Target, std::string Message);
bool SendPlain(SOCKET Target, std::string Message);
bool Receive(SOCKET Target, std::string *Buffer);
bool ReceivePlain(SOCKET Target, unsigned int Length, std::string *Buffer);

#endif