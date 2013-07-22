#include "KeyMonitor.h"

#include <conio.h>

KeyMonitor::KeyMonitor(std::mutex *CinLock)
	:CinLock(CinLock),
	Locked(false)
{

}

void KeyMonitor::Lock()
{
	if(!Locked)
	{
		CinLock->lock();
		Locked=true;
		// Start thread
		KeyMon=std::thread(&KeyMonitor::KeyMonFunc, this);
	}
}
void KeyMonitor::Unlock()
{
	if(Locked)
	{
		CinLock->unlock();
		Locked=false;
		KeyMon.detach();
	}
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
		return 0;
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