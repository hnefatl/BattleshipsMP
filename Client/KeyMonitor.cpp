#include "KeyMonitor.h"

#include <conio.h>

KeyMonitor::KeyMonitor(std::mutex *CinLock)
	:CinLock(CinLock)
{
	KeyMon=new std::thread(&KeyMonitor::KeyMonFunc, this);
	KeyMon->detach();
}
KeyMonitor::~KeyMonitor()
{
	delete CinLock;
	CinLock=NULL;
	delete KeyMon;
	KeyMon=NULL;
}

bool KeyMonitor::KeyAvailable()
{
	return KeyPresses.size()>0;
}
int KeyMonitor::GetKey()
{
	if(KeyAvailable())
	{
		int Key=KeyPresses.front();
		KeyPresses.pop();
		return Key;
	}
	else
	{
		return -1;
	}
}

void KeyMonitor::KeyMonFunc()
{
	while(true)
	{
		int Pressed=_getch();
		KeyPresses.push(Pressed);
	}
}