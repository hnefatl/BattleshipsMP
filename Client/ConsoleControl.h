#ifndef _CONSOLECONTROL_H
#define _CONSOLECONTROL_H

#include <iostream>
#include <windows.h>

enum ConsoleColour
{
    Black       = 0,
    DarkBlue   = 1,
    DarkGreen  = 2,
    Teal        = 3,
    DarkRed    = 4,
    DarkPurple = 5,
    Gold        = 6,
    Grey        = 7,
    DarkWhite  = 8,
    Blue        = 9,
    Green       = 10,
    Cyan        = 11,
    Red         = 12,
    Purple      = 13,
    Yellow      = 14,
    White       = 15
};

extern void SetColour(ConsoleColour foreground, ConsoleColour background);

extern void SetCursor(int x, int y);

extern void Clear();

#endif