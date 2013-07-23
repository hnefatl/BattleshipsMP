#ifndef _BATTLESHIPSETTINGS_H
#define _BATTLESHIPSETTINGS_H

#include "ConsoleControl.h"

#include <vector>
#include <string>

class BattleshipSettings
{
public:
	unsigned int Width, Height;
	unsigned int TimeToPlaceShips, TimePerTurn;
	unsigned int ShipsAllowed[5];
	bool TouchingShips;
	std::string DisconnectString;
	ConsoleColour EmptyBackColour, EmptyForeColour;
	ConsoleColour ShipBackColour, ShipForeColour;
	ConsoleColour StrikeBackColour, StrikeForeColour;
	ConsoleColour IllegalBackColour, IllegalForeColour;

	std::vector<std::string> EnumerateSettings();
};

#endif