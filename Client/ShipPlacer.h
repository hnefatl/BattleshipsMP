#ifndef _SHIPPLACER_H
#define _SHIPPLACER_H

#include <vector>

#include "KeyMonitor.h"
#include "BattleshipSettings.h"
#include "Cell.h"
#include <mutex>

class ShipPlacer
{
public:
	ShipPlacer(BattleshipSettings Settings);
	
	void Run(bool &Signal, std::mutex *Mutex);

	bool Update(KeyMonitor &Monitor);
	void Draw(std::mutex *Mutex);

private:
	std::vector<std::vector<Cell>> Board;
	unsigned int CursorX, CursorY;
	std::vector<unsigned int> LastCursorX, LastCursorY;
	Cell ShipType;
	bool Flipped;

	unsigned int ShipsRemaining[5];

	BattleshipSettings Settings;
};

#endif