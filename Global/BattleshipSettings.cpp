#include "BattleshipSettings.h"

#include <vector>
#include <string>

#include "Util.h"

std::vector<std::string> BattleshipSettings::EnumerateSettings()
{
	std::vector<std::string> Settings;
	std::string Buffer;
	Settings.push_back(ToString(Width));					// Width
	Settings.push_back(ToString(Height));					// Height
	Settings.push_back(ToString(TimeToPlaceShips));			// Time to place ships
	Settings.push_back(ToString(TimePerTurn));				// Time per turn
	Settings.push_back(TouchingShips?"1":"0");				// Whether ships can touch
	Settings.push_back(DisconnectString);					// String to signal Server force disconnect
	Settings.push_back(ToString((int)EmptyBackColour));		// EmptyBackColour
	Settings.push_back(ToString((int)EmptyForeColour));		// EmptyForeColour
	Settings.push_back(ToString((int)ShipBackColour));		// ShipBackColour
	Settings.push_back(ToString((int)ShipForeColour));		// ShipForeColour
	Settings.push_back(ToString((int)StrikeBackColour));	// StrikeBackColour
	Settings.push_back(ToString((int)StrikeForeColour));	// StrikeForeColour
	Settings.push_back(ToString((int)IllegalBackColour));	// IllegalBackColour
	Settings.push_back(ToString((int)IllegalForeColour));	// IllegalForeColour
	for(unsigned int x=0; x<5; x++)
	{
		Settings.push_back(ToString(ShipsAllowed[x]));		// Allowed ships for size x
	}

	return Settings;
}