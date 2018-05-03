#include <iostream>
#include <string>
#include <map>
#include <conio.h>

#include "Dictionary.h"
#include "Board.h"

using namespace std;

void Introduction();
void Instructions();
void Options();
string askDictionaryName();
pair<int, int> askBoardSize();
Board CreatePuzzle();
Board ResumePuzzle();
void EditBoard(Board board);

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
				cout << "\nBoard was not opened successfuly\n";
			break;
		case 2:
			board = ResumePuzzle();
			if (board.isInitialized())
				EditBoard(board);
			else
				cout << "\nBoard was not opened successfuly\n";
			break;
		default:
			cerr << "Should not be able to get here!";
		}
	}
	return 0;
}

// TODO Assure uppercase words
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

string askDictionaryName()
{
	string dictName;
	cout << "Dictionary file name? ";
	cin >> dictName;
	return dictName;
}

//=================================================================================================================================

string askBoardName()
{
	string boardName;
	cout << "Board file name? ";
	cin >> boardName;
	return boardName;
}

//=================================================================================================================================

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
		cout << "Horizontal board size? ";	
		cin >> boardSize.first;
	} while (!cin || boardSize.first <= 0 || boardSize.first > 26);

	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cout << "Vertical board size? ";
		cin >> boardSize.second;
	} while (!cin || boardSize.second <= 0 || boardSize.second > 26);

	return boardSize;
}

//=================================================================================================================================

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
		cout << "Do you wish to save the current board (Y/N) ? ";
		cin >> answer;
		answer = toupper(answer);

		if (answer == 'Y')
		{
			string fileName;
			cout << "File name? ";
			cin.ignore(10000, '\n'); //ignore remaining chars in buffer
			getline(cin, fileName);
			board.savePuzzle(fileName);
			validAnswer = true;
		}
		else if (answer == 'N')
			validAnswer = true;

	} while (!validAnswer);
}

//=================================================================================================================================

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
		cout << "\nCould not locate file with that name.\n";
		//_getch();
		return Board();
	}

	cout << endl;
	pair<int, int> boardSize = askBoardSize();
	Board board(boardSize.first, boardSize.second, dictionary);

	return board;
}

//=================================================================================================================================

Board ResumePuzzle()
{
	string boardName = askBoardName();
	Board board;
	bool boardLoaded = board.loadPuzzle(boardName); 
	
	if (!boardLoaded)
	{
		cout << "\nCould not locate file with that name.\n";
		return Board();
	}

	board.debug();
	return board;
}

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
			cout << "Position (\"LCD\" / CTRL-Z = stop) ? ";
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

			cout << "Word (< = return / - = remove / ? = help) .. ? ";
			cin >> word;

			//Convert to uppercase
			for (size_t i = 0; i < word.length(); i++)
				word.at(i) = toupper(word.at(i));

			if (word == "<") // Skip loop
			{
				validInput = true; // exit loop
				cout << endl;
			}
			else if (word == "-") // Remove word
			{
				board.removeWord(positionInput);
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
