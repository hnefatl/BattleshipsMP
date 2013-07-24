#include "BattleshipSettings.h"

#include <vector>
#include <string>

std::vector<std::string> BattleshipSettings::EnumerateSettings()
{
	std::vector<std::string> Settings;
	std::string Buffer;
	Settings.push_back(std::string(_itoa(Width, &Buffer[0], 10)));		// Width
	Settings.push_back(_itoa(Height, &Buffer[0], 10));					// Height
	Settings.push_back(_itoa(TimeToPlaceShips, &Buffer[0], 10));		// Time to place ships
	Settings.push_back(_itoa(TimePerTurn, &Buffer[0], 10));				// Time per turn
	Settings.push_back(TouchingShips?"1":"0");							// Whether ships can touch
	Settings.push_back(DisconnectString);								// String to signal Server force disconnect
	Settings.push_back(_itoa((int)EmptyBackColour, &Buffer[0], 10));	// EmptyBackColour
	Settings.push_back(_itoa((int)EmptyForeColour, &Buffer[0], 10));	// EmptyForeColour
	Settings.push_back(_itoa((int)ShipBackColour, &Buffer[0], 10));		// ShipBackColour
	Settings.push_back(_itoa((int)ShipForeColour, &Buffer[0], 10));		// ShipForeColour
	Settings.push_back(_itoa((int)StrikeBackColour, &Buffer[0], 10));	// StrikeBackColour
	Settings.push_back(_itoa((int)StrikeForeColour, &Buffer[0], 10));	// StrikeForeColour
	Settings.push_back(_itoa((int)IllegalBackColour, &Buffer[0], 10));	// IllegalBackColour
	Settings.push_back(_itoa((int)IllegalForeColour, &Buffer[0], 10));	// IllegalForeColour
	for(unsigned int x=0; x<5; x++)
	{
		Settings.push_back(_itoa(ShipsAllowed[x], &Buffer[0], 10));		// Allowed ships for size x
	}

	return Settings;
}