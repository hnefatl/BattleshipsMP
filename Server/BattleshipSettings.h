#ifndef _BATTLESHIPSETTINGS_H
#define _BATTLESHIPSETTINGS_H

#include "ConsoleControl.h"

class BattleshipSettings
{
public:
	unsigned int Width, Height;
	unsigned int TimeToPlaceShips, TimePerTurn;
	unsigned int ShipsAllowed[5];
	bool TouchingShips;
	ConsoleColour EmptyBackColour, EmptyForeColour;
	ConsoleColour ShipBackColour, ShipForeColour;
	ConsoleColour StrikeBackColour, StrikeForeColour;
	ConsoleColour IllegalBackColour, IllegalForeColour;
};

#endif