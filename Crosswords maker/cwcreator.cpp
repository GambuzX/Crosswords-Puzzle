#include <iostream>
#include <string>
#include <map>
#include <conio.h>
#include "ColorMaster.h"

#include "Dictionary.h"
#include "Board.h"

using namespace std;

//TODO New mode -> Less restrictive board creation, with words not from the dictionary. Completely up to the user
//TODO Option of adding hashes
//TODO Better help function
//TODO Final board check
//TODO Board output file name according to project specifications

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

#define QUESTION_COLOR 11
#define SYMBOL_COLOR 14

//==========================================================================================

void Introduction();
void Instructions();
void Options();
string askDictionaryName();
pair<int, int> askBoardSize();
Board CreatePuzzle();
Board ResumePuzzle();
void EditBoard(Board board);

ColorMaster colorMaster;

int main()
{
	Introduction();

	cout << endl << endl;

	Instructions();
	cout << endl;

	while (true) //Program only ends by user input
	{
		int answer;
		do
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}
			cout << endl;
			Options();
			cin >> answer;
		} while (cin.fail() || answer < 0 || answer > 2);

		//SELECT OPTION
		Board board;
		switch (answer)
		{
		case 0:
			exit(0);
			break;
		case 1:
			board = CreatePuzzle();
			if (board.isInitialized())
				EditBoard(board);
			else
			{
				colorMaster.setcolor(RED);
				cout << "\nBoard was not opened successfuly\n";
				colorMaster.setcolor(WHITE);
			}
			break;
		case 2:
			board = ResumePuzzle();
			if (board.isInitialized())
				EditBoard(board);
			else
			{
				colorMaster.setcolor(RED);
				cout << "\nBoard was not opened successfuly\n";
				colorMaster.setcolor(WHITE);
			}
			break;
		default:
			cerr << "Should not be able to get here!";
		}
	}
	return 0;
}

// TODO Use _getch() where needed

//=================================================================================================================================
//TODO Ascii art with colors

void Introduction()
{
	cout << "Crosswords Puzzle Creator\n";
	cout << "=========================\n";
}

//=================================================================================================================================
//TODO Instructions

void Instructions()
{
	cout << "Instructions: \n";
	cout << "(...)\n";
}

//=================================================================================================================================
//TODO Show options

void Options()
{
	cout << "Options: \n";
	cout << " (...)\n";
}

//=================================================================================================================================
// Asks for the name of the dictionary

string askDictionaryName()
{
	string dictName;
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Dictionary file name? ";
	colorMaster.setcolor(WHITE);
	cin >> dictName;
	return dictName;
}

//=================================================================================================================================
// Asks for the name of the board

string askBoardName()
{
	string boardName;
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Board file name? ";
	colorMaster.setcolor(WHITE);
	cin >> boardName;
	return boardName;
}

//=================================================================================================================================
// Asks for the size of the board

pair<int,int> askBoardSize()
{
	pair<int, int> boardSize;
	cout << "Max size for each direction is 26.\n";

	do {
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		colorMaster.setcolor(QUESTION_COLOR);
		cout << "Horizontal board size? ";	
		colorMaster.setcolor(WHITE);
		cin >> boardSize.first;
	} while (!cin || boardSize.first <= 0 || boardSize.first > 26);

	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		colorMaster.setcolor(QUESTION_COLOR);
		cout << "Vertical board size? ";
		colorMaster.setcolor(WHITE);
		cin >> boardSize.second;
	} while (!cin || boardSize.second <= 0 || boardSize.second > 26);

	return boardSize;
}

//=================================================================================================================================
// Asks if the user wishes to save the current board

void askToSaveBoard(Board board)
{
	bool validAnswer = false;
	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
		}
		char answer;
		colorMaster.setcolor(QUESTION_COLOR);
		cout << "Do you wish to save the current board (Y/N) ? ";
		colorMaster.setcolor(WHITE);
		cin >> answer;
		answer = toupper(answer);

		if (answer == 'Y')
		{
			string fileName;
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "File name? ";
			colorMaster.setcolor(WHITE);
			cin.ignore(10000, '\n'); //ignore remaining chars in buffer
			getline(cin, fileName);
			board.saveBoard(fileName);
			cout << "\nBoard was saved successfully.\n";
			validAnswer = true;
		}
		else if (answer == 'N')
			validAnswer = true;

	} while (!validAnswer);
}

//=================================================================================================================================
// Creates a new puzzle from scratch

Board CreatePuzzle()
{
	cout << " ------------------------------------\n";
	cout << "           CREATE PUZZLE             \n";
	cout << " ------------------------------------\n";

	string dictName = askDictionaryName();
	Dictionary dictionary(dictName);
	bool dictionaryOpened = dictionary.ProcessDictionary();
	if (!dictionaryOpened)
	{
		colorMaster.setcolor(RED);
		cout << "\nCould not locate file with that name.\n";
		colorMaster.setcolor(WHITE);
		//_getch();
		return Board();
	}

	cout << endl;
	pair<int, int> boardSize = askBoardSize();
	Board board(boardSize.first, boardSize.second, dictionary);

	return board;
}

//=================================================================================================================================
// Resumes an already existing puzzle from a file

Board ResumePuzzle()
{
	cout << endl;
	string boardName = askBoardName();
	Board board;
	bool boardLoaded = board.loadBoard(boardName); 

	if (!boardLoaded)
	{
		colorMaster.setcolor(RED);
		cout << "\nCould not locate file with that name.\n";
		colorMaster.setcolor(WHITE);
		return Board();
	}

	return board;
}

//=================================================================================================================================
// Allows to make changes to an existing board

void EditBoard(Board board)
{
	cout << endl;
	board.showBoard();
	cout << endl;

	// Input loop
	bool stopCreating = false;
	while (!stopCreating)
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
			colorMaster.setcolor(WHITE);
			cout << "(\"LCD\" / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "CTRL - Z";
			colorMaster.setcolor(WHITE);
			cout << " = stop) ";
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "? ";
			colorMaster.setcolor(WHITE);
			cin >> positionInput;

			if (cin.fail())
			{
				if (cin.eof())
				{
					stopCreating = true;
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
				if (board.validPositionInput(positionInput))
					validPositionInput = true;
			}
		} while (!validPositionInput); //loop until valid input

		if (stopCreating) //exit loop if CTRL-Z
		{
			askToSaveBoard(board);
			break;
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
			colorMaster.setcolor(WHITE);
			cout << " (";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "<";
			colorMaster.setcolor(WHITE);
			cout << " = return / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "-";
			colorMaster.setcolor(WHITE);
			cout << " = remove / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "?";
			colorMaster.setcolor(WHITE);
			cout << " = help)";
			colorMaster.setcolor(QUESTION_COLOR);
			cout << " ? ";
			colorMaster.setcolor(WHITE);
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
				bool wordRemoved = board.removeWord(positionInput);
				if (wordRemoved)
					validInput = true; // exit loop
				cout << endl;
			}
			else if (word == "?") // Ask for help
			{
				board.helpUser(positionInput);
				cout << endl;
			}
			else // default
				if (board.canBeInserted(word, positionInput)) //Check validity and output error messages if necessary
				{
					board.insertWord(word, positionInput);
					validInput = true;
				}
		} while (!validInput); //loop until valid input

		cout << endl;
		board.showBoard();
		cout << endl;
	}
}
