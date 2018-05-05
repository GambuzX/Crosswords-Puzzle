#include <iostream>
#include <string>
#include <map>
#include <conio.h>
#include <random>

#include "ColorMaster.h"
#include "Dictionary.h"
#include "Board.h"
#include "Player.h"

using namespace std;

void Introduction();
void Instructions();
string askBoardName();
string askPlayerName();

ColorMaster colorMaster;

//==========================================================================================
//COLOR CODES:

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3A
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

#define DEFAULT 15
#define QUESTION_COLOR 11
#define SYMBOL_COLOR 14
#define ERROR_MESSAGE 4
#define SUCCESS 10

//==========================================================================================

int main()
{
	srand(time_t(NULL));

	Introduction();
	cout << endl;

	Instructions();
	cout << endl;

	//LOAD BOARD
	string boardName = askBoardName();
	Board board;
	bool boardLoaded = board.loadBoard(boardName);
	cout << endl;
	if (!boardLoaded)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nCould not locate file with that name.\n";
		colorMaster.setcolor(DEFAULT);
		exit(1);
	}

	//CREATE PLAYER
	cin.ignore(10000, '\n'); //needed because of remaining '\n' in the buffer
	string playerName = askPlayerName();
	Player player(playerName);

	//CREATE EDITABLE BOARD
	board.createPlayerBoard();
	board.showPlayerBoard();

	return 0;
}

//=================================================================================================================================
//TODO Ascii art with colors

void Introduction()
{
	cout << "Crosswords Puzzle Solver\n";
	cout << "=========================\n";
}

//=================================================================================================================================
//TODO Instructions

void Instructions()
{
	cout << "Instructions: \n";
	cout << "(...)\n";

	//TODO Implement these ideas
	// C for clues at anytime
	// 
}

//=================================================================================================================================
//TODO Show options

void Options()
{
	cout << "Options: \n";
	cout << " (...)\n";
}

//=================================================================================================================================
// Asks for the name of the board

string askBoardName()
{
	string boardName;
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Board file name? ";
	colorMaster.setcolor(DEFAULT);
	cin >> boardName;
	return boardName;
}

//=================================================================================================================================
// Asks for the name of the player

string askPlayerName()
{
	string playerName;
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Player name? ";
	colorMaster.setcolor(DEFAULT);
	getline(cin, playerName);
	return playerName;
}