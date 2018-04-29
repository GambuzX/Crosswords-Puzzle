#include <iostream>
#include <string>
#include <map>

#include "Dictionary.h"
#include "Board.h"

using namespace std;

string askDictionaryName();
pair<int, int> askBoardSize();

int main()
{
	Introduction();
	cout << endl << endl;

	Instructions();
	cout << endl << endl;

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
			Options();
			cin >> answer;
		} while (cin.fail() || answer < 0 || answer > 2);

		switch (answer)
		{
		case 0:
			exit(0);
			break;
		case 1:
			CreatePuzzle();
			break;
		case 2:
			//Resume puzzle
			break;
		default:
			cerr << "Should not be able to get here!";
		}
	}
	return 0;
}

// TODO Assure uppercase words
// TODO Make sure max size is 26 (because of letters)

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

pair<int,int> askBoardSize()
{
	pair<int, int> boardSize;

	cout << "Horizontal board size? ";	
	cin >> boardSize.first;
	while (!cin)
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Horizontal board size? ";
		cin >> boardSize.first;
	}

	cout << "Vertical board size? ";
	cin >> boardSize.second;
	while (!cin)
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Vertical board size? ";
		cin >> boardSize.second;
	}
	return boardSize;
}

//=================================================================================================================================

void CreatePuzzle()
{
	cout << " ------------------------------------\n";
	cout << "           CREATE PUZZLE             \n";
	cout << " ------------------------------------\n";

	string dictName = askDictionaryName();
	Dictionary dictionary(dictName);
	dictionary.ProcessDictionary();

	pair<int, int> boardSize = askBoardSize();
	Board board(boardSize.first, boardSize.second, dictionary);

	board.showBoard();

	bool stopCreating = false;
	while (!stopCreating)
	{
		string position, word;

		bool validPositionInput = false;
		do
		{
			if (cin.fail())
			{
				if (cin.eof())
					stopCreating = true;
				else
				{
					cin.clear();
					cin.ignore(10000, '\n');
				}
			}

			cout << "Position (\"LCD / CTRL-Z = stop) ?";
			cin >> position;
			if (board.validPositionInput(position))
				validPositionInput = true;
		} while (!validPositionInput);
		
		//Convert to uppercase
		for (size_t i = 0; i < position.length(); i++) 
			position.at(i) = toupper(position.at(i));

		do
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}

			cout << "Word ( - = remove / ? = help) .. ?";
			cin >> word;
		} while (!cin);

		//Convert to uppercase
		for (size_t i = 0; i < word.length(); i++)
			word.at(i) = toupper(word.at(i));

	}
}