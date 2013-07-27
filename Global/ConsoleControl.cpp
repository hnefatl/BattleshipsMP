#include "ConsoleControl.h"

void SetColour(ConsoleColour foreground, ConsoleColour background)
{

    int Color = foreground + (background * 16);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, Color);
}

void SetCursor(int x, int y)
{
  HANDLE hStdout;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
  csbiInfo.dwCursorPosition.X=x;
  csbiInfo.dwCursorPosition.Y=y;
  SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition);
}

void SetCursor(bool Active)
{
	HANDLE Console=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	CursorInfo.bVisible=Active;
	CursorInfo.dwSize=1;
	SetConsoleCursorInfo(Console, &CursorInfo);
}

void Clear()
{
	system("cls");
}