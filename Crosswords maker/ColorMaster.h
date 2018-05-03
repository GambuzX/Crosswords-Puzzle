#ifndef COLOR_MASTER_H
#define COLOR_MASTER_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>

class ColorMaster
{
public:
	void clrscr(void);
	void gotoxy(int x, int y);
	void setcolor(unsigned int color);
	void setcolor(unsigned int color, unsigned int background_color);
private:
};

#endif