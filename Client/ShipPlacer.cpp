#include <iostream>
#include "ShipPlacer.h"

ShipPlacer::ShipPlacer(BattleshipSettings Settings)
	:Settings(Settings)
{
	Board.resize(Settings.Height);
	for(unsigned int x=0; x<Board.size(); x++)
	{
		Board[x].resize(Settings.Width, Cell::Empty);
	}

	for(unsigned int x=0; x<5; x++)
	{
		ShipsRemaining[x]=Settings.ShipsAllowed[x];
	}
}

void ShipPlacer::Run(bool &Signal, std::mutex *Mutex)
{	
	std::mutex *InputLock=new std::mutex();
	KeyMonitor Monitor(InputLock);
	Monitor.Lock();

	Draw(Mutex); // Initial Draw
	// While signalled to keep running
	while(!Signal)
	{
		Update(Monitor);
		Draw(Mutex);
	}
}

void ShipPlacer::Update(KeyMonitor &Monitor)
{
	int MaxShipSize=5;

	if(Monitor.KeyAvailable())
	{
		// Update
		int Key=Monitor.GetKey();

		LastCursorX=CursorX;
		LastCursorY=CursorY;

		if(Key==224 && Monitor.KeyAvailable()) // Arrow Key
		{
			Key=Monitor.GetKey();
			if(Key==72 && CursorY>0) // Up arrow
			{
				CursorY--;
			}
			else if(Key==80 && CursorY<Settings.Height-1) // Down arrow
			{
				CursorY++;
			}
			else if(Key==75 && CursorX>0) // Left arrow
			{
				CursorX--;
			}
			else if(Key==77 && CursorX<Settings.Width-1) // Right arrow
			{
				CursorX++;
			}
		}
		else if(Key==43 || Key==61) // Plus key (with and without shift)
		{
			if((int)ShipType+1<MaxShipSize)
			{
				ShipType=(Cell)((int)ShipType+1);
			}
		}
		else if(Key==45 || Key==95) // Minus key (with and without shift)
		{
			if((int)ShipType-1>1)
			{
				ShipType=(Cell)((int)ShipType-1);
			}
		}
		else if (Key==102) // F key
		{
			Flipped=!Flipped;
		}
		else if(Key=13) // Enter key
		{

		}
	}
}

void ShipPlacer::Draw(std::mutex *Mutex)
{
	// Draw
	Mutex->lock();

	// Clear last Cursor
	SetCursor(LastCursorX, LastCursorY);
	if(Board[LastCursorY][LastCursorX]==Cell::Empty)
	{
		SetColour(Settings.EmptyForeColour, Settings.EmptyBackColour);
	}
	else
	{
		SetColour(Settings.ShipForeColour, Settings.ShipBackColour);
	}
	std::cout<<" ";

	// Draw new Cursor
	SetCursor(CursorX, CursorY);

	// Determine marker colour
	if(ShipsRemaining[ShipType]==0) // No Ships
	{
		SetColour(Settings.IllegalForeColour, Settings.IllegalBackColour);
	}
	else // Ships left
	{
		// Does it conflict with board or other ships?
		bool Conflict=false;
		try
		{
			if(Settings.TouchingShips)
			{
				// Loop (size of ship) times, so that x=0 is central to marker
				for(int x=0-((int)ShipType+1)/2; x<((int)ShipType+1)/2; x++)
				{
					if(Flipped)
					{
						// Horizontal marker
						if(Board[CursorY][CursorX+x]!=Cell::Empty)
						{
							Conflict=true;
						}
					}
					else
					{
						// Vertical marker
						if(Board[CursorY+x][CursorX]!=Cell::Empty)
						{
							Conflict=true;
						}
					}
				}
			}
			else
			{
				// Run same check as with TouchingShips, but +1 to each axis
				// Loop (size of ship) times, so that x=0 is central to marker, and y goes from -1 - 1 to traverse each side
				for(unsigned int y=-1; y<=1; y++)
				{
					for(int x=(0-((int)ShipType+1)/2)-1; x<(((int)ShipType+1)/2)+1; x++)
					{
						if(Flipped)
						{
							// Horizontal marker
							if(Board[CursorY+y][CursorX+x]!=Cell::Empty)
							{
								Conflict=true;
							}
						}
						else
						{
							// Vertical marker
							if(Board[CursorY+x][CursorX+y]!=Cell::Empty)
							{
								Conflict=true;
							}
						}
					}
				}
			}
		}
		catch(std::exception)
		{
			// Out of bounds of array (Board), thus conflicts
			Conflict=true;
		}
		if(Conflict)
		{
			SetColour(Settings.IllegalForeColour, Settings.IllegalBackColour);
		}
		else
		{
			SetColour(Settings.ShipForeColour, Settings.ShipBackColour);
		}
	}

	// Actually draw marker
	// Loop (size of ship) times, so that x=0 is central to marker
	for(int x=0-((int)ShipType+1)/2; x<((int)ShipType+1)/2; x++)
	{
		try
		{
			if(Flipped)
			{
				// Horizontal marker
				SetCursor(CursorY, CursorX+x);
			}
			else
			{
				// Vertical marker
				SetCursor(CursorY+x, CursorX);
			}
			std::cout<<" ";
		}
		catch(std::exception)
		{
			// Ignore
		}
	}

	Mutex->unlock();
}
