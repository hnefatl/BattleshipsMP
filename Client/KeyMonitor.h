#ifndef _KEYMONITOR_H
#define _KEYMONITOR_H

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

class KeyMonitor
{
public:
	KeyMonitor(std::mutex *CinLock);

	void Lock();
	void Unlock();

	bool KeyAvailable();
	int GetKey();

private:
	std::queue<int> KeyPresses;
	std::mutex *CinLock;

	bool Locked;

	std::thread KeyMon;
	void KeyMonFunc();
};

#endif