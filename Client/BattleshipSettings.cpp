#include "BattleshipSettings.h"

#include <vector>
#include <string>

std::vector<std::string> BattleshipSettings::EnumerateSettings()
{
	std::vector<std::string> Settings;

	Settings.push_back(_itoa(Width, NULL, 10));						// Width
	Settings.push_back(_itoa(Height, NULL, 10));					// Height
	Settings.push_back(_itoa(TimeToPlaceShips, NULL, 10));			// Time to place ships
	Settings.push_back(_itoa(TimePerTurn, NULL, 10));				// Time per turn
	Settings.push_back(TouchingShips?"1":"0");						// Whether ships can touch
	Settings.push_back(DisconnectString);							// String to signal Server force disconnect
	Settings.push_back(_itoa((int)EmptyBackColour, NULL, 10));		// EmptyBackColour
	Settings.push_back(_itoa((int)EmptyForeColour, NULL, 10));		// EmptyForeColour
	Settings.push_back(_itoa((int)ShipBackColour, NULL, 10));		// ShipBackColour
	Settings.push_back(_itoa((int)ShipForeColour, NULL, 10));		// ShipForeColour
	Settings.push_back(_itoa((int)StrikeBackColour, NULL, 10));		// StrikeBackColour
	Settings.push_back(_itoa((int)StrikeForeColour, NULL, 10));		// StrikeForeColour
	Settings.push_back(_itoa((int)IllegalBackColour, NULL, 10));	// IllegalBackColour
	Settings.push_back(_itoa((int)IllegalForeColour, NULL, 10));	// IllegalForeColour
	for(unsigned int x=0; x<5; x++)
	{
		Settings.push_back(_itoa(ShipsAllowed[x], NULL, 10));		// Allowed ships for size x
	}

	return Settings;
}