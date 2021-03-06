#ifndef _SHIPPLACER_H
#define _SHIPPLACER_H

#include <vector>

#include "KeyMonitor.h"
#include "BattleshipSettings.h"
#include "Cell.h"
#include "ThreadSignal.h"
#include <mutex>

class ShipPlacer
{
public:
	ShipPlacer(BattleshipSettings Settings);
	
	void Run(ThreadSignal<bool> *Signal, std::mutex *Mutex);

	bool Update(KeyMonitor &Monitor);
	void Draw(std::mutex *Mutex);

	std::vector<std::vector<Cell>> Board;
private:
	unsigned int CursorX, CursorY;
	std::vector<unsigned int> LastCursorX, LastCursorY;
	Cell ShipType;
	bool Flipped;
	bool Conflict;

	unsigned int ShipsRemaining[5];

	BattleshipSettings Settings;
};

#endif