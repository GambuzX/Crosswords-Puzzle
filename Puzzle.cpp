#include "Puzzle.h"
#include "Board.h"
#include "ColorMaster.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

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

#define DEFAULT 15
#define QUESTION_COLOR 11
#define SYMBOL_COLOR 14
#define ERROR_MESSAGE 4
#define SUCCESS 10

//=================================================================================================================================
// Class constructor. Assigns a board to the Puzzle.

Puzzle::Puzzle(Board board)
{
	solutionBoard = board.getBoard();
	usedWords = board.getUsedWords();
	verticalSize = board.getVerticalSize();
	horizontalSize = board.getHorizontalSize();
	dictionary = board.getDictionary();
}

//=================================================================================================================================
// Changes current board.

void Puzzle::setSolutionBoard(Board board)
{
	solutionBoard = board.getBoard();
	usedWords = board.getUsedWords();
	verticalSize = board.getVerticalSize();
	horizontalSize = board.getHorizontalSize();
	dictionary = board.getDictionary();
}

//=================================================================================================================================
// Creates a dummy board for the player to add words. Created from the original board by replacing letters with dots.

void Puzzle::createPlayerBoard()
{
	playerBoard = solutionBoard;
	for (int i = 0; i < verticalSize; i++)
		for (int j = 0; j < horizontalSize; j++)
			if (isalpha(playerBoard.at(i).at(j)))
				playerBoard.at(i).at(j) = '.';
}

//=================================================================================================================================
// Shows an "empty board", only white and black cells.

void Puzzle::showPlayerBoard()
{
	const int WIDTH = 2;

	cout << setw(WIDTH) << " ";
	for (size_t i = 0; i < playerBoard.at(0).size(); i++)
	{
		colorMaster.setcolor(RED);
		cout << setw(WIDTH) << (char)('a' + i);
	}
	cout << endl;

	for (size_t i = 0; i < playerBoard.size(); i++)
	{
		colorMaster.setcolor(RED);
		cout << (char)('A' + i) << " ";

		for (size_t j = 0; j < playerBoard.at(i).size(); j++)
		{
			if (playerBoard.at(i).at(j) == '#')
			{
				colorMaster.setcolor(WHITE, BLACK);
				cout << "  "; //empty black space
			}
			else if (playerBoard.at(i).at(j) == '.')
			{
				colorMaster.setcolor(BLACK, WHITE);
				cout << "  "; //empty white space
			}
			else
			{
				colorMaster.setcolor(BLACK, WHITE);
				cout << setw(WIDTH) << playerBoard.at(i).at(j); // show user input letters
			}
		}
		cout << '\n';
	}
	colorMaster.setcolor(WHITE, BLACK); //set to default
}

//=================================================================================================================================
// Builds an initial list of clues.

void Puzzle::buildClueList()
{
	for (size_t i = 0; i < usedWords.size(); i++)
	{
		string position = usedWords.at(i).first; //Location and direction
		position.at(1) = tolower(position.at(1)); //Set column to be lowercase
		string word = usedWords.at(i).second;
		string clue = dictionary.GetWordSynonym(word);
		clueList.push_back(pair<string, string>(position, clue));
	}
}

//=================================================================================================================================
// Shows the list of clues to the user.

void Puzzle::showClueList()
{
	//TODO add colors

	//HORIZONTAL
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nHORIZONTAL WORDS\n";
	colorMaster.setcolor(WHITE, BLACK);
	for (size_t i = 0; i < clueList.size(); i++)
	{
		if (clueList.at(i).first.at(2) == 'H')
		{
			cout << clueList.at(i).first.substr(0, 2) << " - " << clueList.at(i).second << endl;
		}
	}

	cout << endl;

	//VERTICAL
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\VERTICAL WORDS\n";
	colorMaster.setcolor(WHITE, BLACK);
	for (size_t i = 0; i < clueList.size(); i++)
	{
		if (clueList.at(i).first.at(2) == 'V')
		{
			cout << clueList.at(i).first.substr(0, 2) << " - " << clueList.at(i).second << endl;
		}
	}
}

//=================================================================================================================================
// Verifies the user position input is valid

bool Puzzle::validPositionInput(string input)
{
	bool valid = true;

	if (input.length() != 3)
		valid = false;
	else
	{
		char line = toupper(input.at(0));
		char column = toupper(input.at(1));
		char direction = toupper(input.at(2));
		char maxHorizontal = 'A' + horizontalSize - 1;
		char maxVertical = 'A' + verticalSize - 1;

		if (line < 'A' || line > maxVertical)
			valid = false;

		if (column < 'A' || column > maxHorizontal)
			valid = false;

		if (direction != 'H' && direction != 'V')
			valid = false;
	}

	if (!valid)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nInvalid input!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else
		return true;
}

