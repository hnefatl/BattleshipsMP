#include <iostream>
#include "ShipPlacer.h"
#include "Util.h"

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

	CursorX=Settings.Width/2;
	CursorY=(Settings.Height/2)+1;
	LastCursorX.push_back(CursorX);
	LastCursorY.push_back(CursorY);
	Flipped=false;
	ShipType=Cell::Destroyer;
}

void ShipPlacer::Run(ThreadSignal<bool> *Signal, std::mutex *Mutex)
{	
	std::mutex *InputLock=new std::mutex();
	KeyMonitor Monitor(InputLock);
	Monitor.Lock();

	// Draw board
	Mutex->lock();
	SetColour(ConsoleColour::Grey, ConsoleColour::Grey);
	SetCursor(0, 0);
	for(unsigned int y=0; y<Settings.Height; y++)
	{
		for(unsigned int x=0; x<Settings.Width; x++)
		{
			std::cout<<" ";
		}
		std::cout<<std::endl;
	}
	Mutex->unlock();

	// Initial drawing
	Draw(Mutex);
	// While signalled to keep running
	while(!Signal->GetSignal())
	{
		if(Update(Monitor))
		{
			// Only draw if a change has been made
			Draw(Mutex);
		}
	}
}

bool ShipPlacer::Update(KeyMonitor &Monitor)
{
	int MaxShipSize=5;

	if(Monitor.KeyAvailable())
	{
		// Update
		int Key=Monitor.GetKey();

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
			if((int)ShipType+1>1)
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
			if(!Conflict)
			{
				for(int x=Round(0-((float)ShipType+1)/2); x<Round(((float)ShipType+1)/2); x++)
				{
					if(Flipped)
					{
						Board[CursorY][CursorX+x]=ShipType;
					}
					else
					{
						Board[CursorY+x][CursorX]=ShipType;
					}
				}
			}
		}

		return true; // Change
	}
	return false; // No change
}

void ShipPlacer::Draw(std::mutex *Mutex)
{
	// Draw
	Mutex->lock();

	// Clear last Cursor
	for(unsigned int x=0; x<LastCursorX.size(); x++)
	{
		SetCursor(LastCursorX[x], LastCursorY[x]);
		if(Board[LastCursorY[x]][LastCursorX[x]]==Cell::Empty)
		{
			SetColour(Settings.EmptyForeColour, Settings.EmptyBackColour);
		}
		else
		{
			SetColour(Settings.ShipForeColour, Settings.ShipBackColour);
		}
		std::cout<<" ";
	}

	LastCursorX.clear();
	LastCursorY.clear();

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
		Conflict=false;
		if(Settings.TouchingShips)
		{
			// Loop (size of ship) times, so that x=0 is central to marker
			for(int x=Round(0-((float)ShipType+1)/2); x<Round(((float)ShipType+1)/2); x++)
			{
				if(Flipped)
				{
					if(CursorX+x<0 || CursorX+x>Settings.Width-1)
					{
						Conflict=true;
						break;
					}
					else
					{
						// Horizontal marker
						if(Board[CursorY][CursorX+x]!=Cell::Empty)
						{
							Conflict=true;
							break;
						}
					}
				}
				else
				{
					if(CursorY+x<0 || CursorY+x>Settings.Height-1)
					{
						Conflict=true;
						break;
					}
					else
					{
						// Vertical marker
						if(Board[CursorY+x][CursorX]!=Cell::Empty)
						{
							Conflict=true;
							break;
						}
					}
				}
			}
		}
		else
		{
			// Run same check as with TouchingShips, but +1 to each axis
			// Loop (size of ship) times, so that x=0 is central to marker, and y goes from -1 - 1 to traverse each side
			for(int y=-1; y<=1; y++)
			{
				for(int x=Round(0-((float)ShipType+1)/2); x<Round(((float)ShipType+1)/2); x++)
				{
					if(Flipped)
					{
						if((CursorX+x<0 || CursorX+x>Settings.Width-1) ||
							(CursorY+y<0 || CursorX+x>Settings.Height-1))
						{
							Conflict=true;
							// Exit clause
							y=1;
							break;
						}
						else
						{
							// Horizontal marker
							if(Board[CursorY+y][CursorX+x]!=Cell::Empty)
							{
								Conflict=true;
								// Exit clause
								y=1;
								break;
							}
						}
					}
					else
					{
						if((CursorX+y<0 || CursorX+y>Settings.Width-1) ||
							(CursorY+x<0 || CursorX+x>Settings.Height-1))
						{
							Conflict=true;
							// Exit clause
							y=1;
							break;
						}
						else
						{
							// Vertical marker
							if(Board[CursorY+x][CursorX+y]!=Cell::Empty)
							{
								Conflict=true;
								// Exit clause
								y=1;
								break;
							}
						}
					}
				}
			}
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
	for(int x=Round(0-((float)ShipType+1)/2); x<Round(((float)ShipType+1)/2); x++)
	{
		if(Flipped)
		{
			if(!(CursorX+x<0 || CursorX+x>Settings.Width-1))
			{
				// Horizontal marker
				SetCursor(CursorX+x, CursorY);
				LastCursorX.push_back(CursorX+x);
				LastCursorY.push_back(CursorY);
				std::cout<<" ";
			}
		}
		else
		{
			if(!(CursorY+x<0 || CursorY+x>Settings.Height-1))
			{
				// Vertical marker
				SetCursor(CursorX, CursorY+x);
				LastCursorX.push_back(CursorX);
				LastCursorY.push_back(CursorY+x);
				std::cout<<" ";
			}
		}
	}
	Mutex->unlock();
}
