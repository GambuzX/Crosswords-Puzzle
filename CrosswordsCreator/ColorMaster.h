/*
Class to implement console color changing. Its methods were provided by JAS.
*
AUTHORS: JAS, Mario Gil.
*/

#ifndef COLOR_MASTER_H
#define COLOR_MASTER_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>

class ColorMaster
{
public:
	//Constructor
	ColorMaster() {};

	//Utility
	void clrscr(void);
	void gotoxy(int x, int y);
	void setcolor(unsigned int color);
	void setcolor(unsigned int color, unsigned int background_color);
};

#endif