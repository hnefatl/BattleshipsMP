#ifndef _THREADSIGNAL_H
#define _THREADSIGNAL_H

#include <mutex>

template<class T>
class ThreadSignal
{
public:
	ThreadSignal()
	{

	}
	ThreadSignal(T Signal)
	{
		SetSignal(Signal);
	}

	T GetSignal()
	{
		Lock.lock();
		T Temp=InnerObject;
		Lock.unlock();
		return Temp;
	}
	void SetSignal(T Signal)
	{
		Lock.lock();
		this->InnerObject=Signal;
		Lock.unlock();
	}

private:
	std::mutex Lock;
	T InnerObject;
};

#endif