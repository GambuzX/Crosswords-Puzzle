#include <iostream>
#include <string>
#include <map>
#include <conio.h>
#include <random>

#include "ColorMaster.h"
#include "Dictionary.h"
#include "Board.h"
#include "Player.h"
#include "Puzzle.h"

using namespace std;

void Introduction();
void Instructions();
string askBoardName();
string askPlayerName();
void solveCurrentPuzzle(Puzzle&);

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

	
	bool finishedProgram = false;
	do
	{
		//LOAD BOARD
		string boardName = askBoardName();
		Board *board = new Board();
		bool boardLoaded = board->loadBoard(boardName);
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

		//CREATE PUZZLE TO BE SOLVED
		Puzzle puzzle(*board);
		delete board; //board is copied to the puzzle instance, duplicate is not needed
		puzzle.createPlayerBoard();
		puzzle.buildClueList();

		solveCurrentPuzzle(puzzle);

		//Ask if player wants to solve other puzzle or not

	} while (!finishedProgram);
	
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

//=================================================================================================================================
// Asks the user to solve the current puzzle.

void solveCurrentPuzzle(Puzzle &puzzle)
{
	puzzle.showPlayerBoard();
	puzzle.showClueList();
	cout << endl;

	bool finishedPuzzle = false;
	do
	{
		string positionInput, word;

		////////////////////////////////
		//      ASK FOR POSITION      //
		////////////////////////////////

		bool validPositionInput = false;
		do
		{
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "Position ";
			colorMaster.setcolor(DEFAULT);
			cout << "(\"LCD\" / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "CTRL - Z";
			colorMaster.setcolor(DEFAULT);
			cout << " = stop) ";
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "? ";
			colorMaster.setcolor(DEFAULT);
			cin >> positionInput;

			if (cin.fail())
			{
				if (cin.eof())
				{
					finishedPuzzle = true;
					cin.clear(); // Clears cin so as to enable next inputs
					break; //exits this loop
				}
				else
				{
					cin.clear();
					cin.ignore(10000, '\n');
				}
			}
			else //if good input
			{
				//Convert to uppercase
				for (size_t i = 0; i < positionInput.length(); i++)
					positionInput.at(i) = toupper(positionInput.at(i));

				//Check validity
				if (puzzle.validPositionInput(positionInput))
					validPositionInput = true;
			}
		} while (!validPositionInput); //loop until valid input

		if (finishedPuzzle) //If CTRL-Z, confirm if the player wishes to leave
		{
			bool leave = false;
			char answer;
			do
			{
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(10000, '\n');
				}
				colorMaster.setcolor(QUESTION_COLOR);
				cout << "\nIf you leave all your progress will be lost. Proceed (Y/N)? ";
				colorMaster.setcolor(DEFAULT);
				cin >> answer;
				cout << endl;
				answer = toupper(answer);
				if (answer == 'Y')
					leave = true;
			} while (answer != 'Y' && answer != 'N');

			if (leave)
				break;
			else
				continue;
		}

		////////////////////////////////
		//        ASK FOR WORD        //
		////////////////////////////////

		bool validInput = false;
		bool skipInsertion = false;
		do
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}
			colorMaster.setcolor(QUESTION_COLOR); //TODO button for instructions instead of displaying all things
			cout << "Word";
			colorMaster.setcolor(DEFAULT);
			cout << " (";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "<";
			colorMaster.setcolor(DEFAULT);
			cout << " = return / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "-";
			colorMaster.setcolor(DEFAULT);
			cout << " = remove / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "?";
			colorMaster.setcolor(DEFAULT);
			cout << " = help)";
			colorMaster.setcolor(QUESTION_COLOR);
			cout << " ? ";
			colorMaster.setcolor(DEFAULT);
			cin >> word;

			//Convert to uppercase
			for (size_t i = 0; i < word.length(); i++)
				word.at(i) = toupper(word.at(i));

			if (word == "<") // Skip loop
			{
				validInput = true; // Exit loop
				cout << endl;
			}
			else if (word == "-") // Remove word
			{
				bool wordRemoved = puzzle.removeWord(positionInput);
				if (wordRemoved)
					validInput = true; // exit loop
				cout << endl;
			}
			else if (word == "?") // Ask for help
			{
				puzzle.showDifferentSynonym(positionInput);
				cout << endl;
			}
			else // default
			{
				if (puzzle.canBeInserted(word, positionInput)) //Check validity and output error messages if necessary 
				{
					puzzle.insertWord(word, positionInput);
					validInput = true;
				}
			}
		} while (!validInput); //loop until valid input

		cout << endl;
		puzzle.showPlayerBoard();
		cout << endl;
	} while (!finishedPuzzle);
}