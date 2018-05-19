/*
Interface to connect the 2 programs, cwcreator and cwplayer.
Executables names must be CrosswordsCreator.exe and CrosswordsPlayer.exe respectively.

AUTHORS: Carolina, Mario Gil.
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>

using namespace std;

//==========================================================================================

//COLOR CODES: 
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15


//==========================================================================================

void clrscr(void)
{
	COORD coordScreen = { 0, 0 }; // upper left corner
	DWORD cCharsWritten;
	DWORD dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// fill with spaces
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	// cursor to upper left corner
	SetConsoleCursorPosition(hCon, coordScreen);
}
//==========================================================================================
// Position the cursor at column 'x', line 'y'
// The coodinates of upper left corner of the screen are (x,y)=(0,0)

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//==========================================================================================
// Set text color

void setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

//==========================================================================================
// Set text color & background

void setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
			BACKGROUND_RED);
}
//==========================================================================================
//PROGRAM MAIN INTERFACE

int interfaceMaker()
{
	cout << endl << endl << endl;
	setcolor(11);
	cout << "      ____ _____  _____    __   __ __    __  _____  _____ _____   __    " << endl;
	cout << "     ((    ||_// ((   ))  ((   ((  \\\\ /\\ // ((   )) ||_// ||  )  ((    " << endl;
	cout << "      \\\\__ || \\\\  \\\\_//  \\_)) \\_))  \\_/\\_/   \\\\_//  || \\\\ ||_// \\_))  " << endl << endl;
	cout << "                   _____ __ __ ____  ____  __    _____ " << endl;
	cout << "                   ||_// || ||   //    //  ||    ||==  " << endl;
	cout << "                   ||    \\\\_//  //__  //__ ||__| ||___ " << endl << endl;
	setcolor(15);
	cout << "     ===============================================================" << endl << endl;
	setcolor(11);
	cout << "  ====================================================================== \n" << endl << endl;

	setcolor(11);
	cout << "  OPTIONS\n";
	cout << "  ~~~~~~~ \n\n ";
	cout << " 1 - ";
	setcolor(15);
	cout << "Create Crosswords Puzzle" << endl;
	setcolor(11);
	cout << "  2 - ";
	setcolor(15);
	cout << "Solve Crosswords Puzzle" << endl;
	setcolor(11);
	cout << "  3 - ";
	setcolor(15);
	cout << "Exit\n\n\n\n";

	//INTERFACE CROSSWORDS BOARD

	for (int line = 1; line <= 10; line++)
	{
		cout << "       ";

		if (line == 2)
		{
			setcolor(10);
			cout << " C  R  O  S  S  W  O  R  D  S \n\n"; //writes "CROSSWORDS" on the interface board
		}

		else
		{
			if (line % 2 == 0)
			{
				for (int column = 5; column <= 14; column++)
				{
					if (column == 8)
					{
						switch (line)
						{
						case 4: setcolor(13); cout << " # "; continue;
						case 6: setcolor(13); cout << " U "; continue;
						case 8: setcolor(13); cout << " Z "; continue;  //writes a part of the word "PUZZLE" on the interface board
						case 10: setcolor(13); cout << " E "; continue;
						}
					}

					if (column == 13)
					{
						switch (line)
						{
						case 4: setcolor(12); cout << " C "; continue;
						case 6: setcolor(12); cout << " E "; continue;
						case 8: setcolor(12); cout << " T "; continue;  //writes a part of the word "CREATE" on the interface board
						case 10: setcolor(12); cout << " # "; continue;
						}
					}
					setcolor(column);
					cout << " . ";

				}
				cout << endl << endl;
			}
			else
			{
				for (int column = 14; column >= 5; column--)
				{
					if (column == 11)
					{
						switch (line)
						{
						case 5: setcolor(13); cout << " P "; continue;
						case 7: setcolor(13); cout << " Z "; continue; //writes the remaining part of the word "PUZZLE" on the interface board
						case 9: setcolor(13); cout << " L "; continue;
						}
					}

					if (column == 6)
					{
						switch (line)
						{
						case 3: setcolor(12); cout << " # "; continue;
						case 5: setcolor(12); cout << " R "; continue; //writes the remaining part of the word "CREATE" on the interface board
						case 9: setcolor(12); cout << " E "; continue;
						}
					}

					if (line == 7)
					{
						switch (column)
						{
						case 9: setcolor(14); cout << " # "; continue;
						case 8: setcolor(14); cout << " P "; continue;
						case 7: setcolor(14); cout << " L "; continue; //writes the word "PLAY" on the interface board
						case 6: setcolor(14); cout << " A "; continue;
						case 5: setcolor(14); cout << " Y "; continue;
						}
					}

					if (line == 9)
					{
						switch (column)
						{
						case 14: setcolor(11); cout << " # "; continue;
						case 13: setcolor(11); cout << " S "; continue;
						case 12: setcolor(11); cout << " O "; continue; //writes the word "SOLVE" on the interface board
						case 10: setcolor(11); cout << " V "; continue;
						case 9: setcolor(11); cout << " E "; continue;
						case 8: setcolor(11); cout << " # "; continue;
						}
					}

					setcolor(column);
					cout << " . ";
				}
				cout << endl << endl;
			}
		}
	}


	// INTERFACE "?"

	setcolor(11);

	gotoxy(45, 29);
	cout << " .----------------. \n";
	gotoxy(45, 30);
	cout << "| .--------------. | \n";
	gotoxy(45, 31);
	cout << "| |    ______    | | \n";
	gotoxy(45, 32);
	cout << "| |   / _ __ `.  | | \n";
	gotoxy(45, 33);
	cout << "| |  |_/____) |  | | \n";
	gotoxy(45, 34);
	cout << "| |    /  ___.'  | | \n";
	gotoxy(45, 35);
	cout << "| |    |_|       | | \n";
	gotoxy(45, 36);
	cout << "| |    (_)       | | \n";
	gotoxy(45, 37);
	cout << "| |              | | \n";
	gotoxy(45, 38);
	cout << "| '--------------' | \n";
	gotoxy(45, 39);
	cout << " '----------------' \n";


	gotoxy(4, 47);
	setcolor(11);
	int option; //input option chosen by the user
	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
		}

		cout << "\n  Option? ";
		cin >> option;
		cout << endl;

	} while (cin.fail() || (option != 1 && option != 2 && option != 3));


	return option;
}

//================================================================================================
//EXECUTES THE CHOSEN OPTION BY THE USER

void optionExecution(int option)
{
	bool validOption = true;
	switch (option)
	{
	case 1:
		clrscr();
		system("start CrosswordsMaker.exe");
		break;
	case 2:
		clrscr();
		system("start CrosswordsPlayer.exe");
		break;
	case 3:
		break;
	default:
		validOption = false;
	}

	if (!validOption)
	{
		clrscr();
		setcolor(13);
		cerr << endl << "  ---------------------------------------- \n";
		cerr << " | Oops! Invalid option input. Try again! | \n";
		cerr << "  ---------------------------------------- ";
		optionExecution(interfaceMaker());
	}
}

//=====================================================================================================

int main()
{
	optionExecution(interfaceMaker());
	return 0;
}
