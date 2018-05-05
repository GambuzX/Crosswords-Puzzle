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
	solutionUsedWords = board.getUsedWords();
	verticalSize = board.getVerticalSize();
	horizontalSize = board.getHorizontalSize();
	dictionary = board.getDictionary();
}

//=================================================================================================================================
// Changes current board.

void Puzzle::setSolutionBoard(Board board)
{
	solutionBoard = board.getBoard();
	solutionUsedWords = board.getUsedWords();
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
	for (size_t i = 0; i < solutionUsedWords.size(); i++)
	{
		string position = solutionUsedWords.at(i).first; //Location and direction
		position.at(1) = tolower(position.at(1)); //Set column to be lowercase
		string word = solutionUsedWords.at(i).second;
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
	cout << "\nVERTICAL WORDS\n";
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
// Inserts a word in the specified position. Assumes all tests have been made.

void Puzzle::insertWord(std::string word, std::string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);
	playerUsedWords.push_back(pair<string, string>(positionInput, word)); //add word to the vector

	//Insert word
	char dir = toupper(direction);
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	switch (dir)
	{
	case 'H':
		for (size_t i = 0; i < word.length(); i++)
			playerBoard.at(line).at(column + i) = word.at(i);
		break;
	case 'V':
		for (size_t i = 0; i < word.length(); i++)
			playerBoard.at(line + i).at(column) = word.at(i);
		break;
	default:
		cerr << "Invalid input!";
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

//=================================================================================================================================
// Verifies the given word can be inserted in the specified position

bool Puzzle::canBeInserted(string word, string position)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(position);
	char direction = position.at(2);

	if (hasHash(insertionPosition)) // Verify it the position has an hash
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nYou can not place a word in that location.\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (!isValidHeadline(word)) // Verify word is valid
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is not valid! Please only use characters from 'A' to 'Z'.\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (!wordFitsSpace(word, position)) // Verify it fits the space
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord does not fit the specified space!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (isWordUsed(word))	// Verify if word was already used
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is already in use!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (!matchesInterceptedPositions(word, position)) // Verify if the insertion can be executed while keeping the board valid
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord does not match current board!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	return true;
}

//=================================================================================================================================
// Checks if a given position has an hash ('#')

bool Puzzle::hasHash(pair<int, int> position)
{
	return playerBoard.at(position.first).at(position.second) == '#';
}

//=================================================================================================================================
// Determines line and column indexes given text input

pair<int, int> Puzzle::calculateInsertionCoordinates(string coordinates)
{
	pair<int, int> position;
	position.first = mapCharToNumber(coordinates.at(0)); //line
	position.second = mapCharToNumber(coordinates.at(1)); //column
	return position;
}

//=================================================================================================================================
// Converts line / columns letter to respective index

int Puzzle::mapCharToNumber(char letter)
{
	char upper = toupper(letter);
	return ((int)upper - (int) 'A');
}

//=================================================================================================================================
// Verifies the given headline is valid

bool Puzzle::isValidHeadline(string word)
{
	for (size_t i = 0; i < word.length(); i++)
	{
		if (word.at(i) < 'A' || word.at(i) > 'Z')
			return false;
	}
	return true;
}

//=================================================================================================================================
// Checks if the word fits in the specified space

bool Puzzle::wordFitsSpace(string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));

	char dir = toupper(direction);
	int wordSize = word.length();
	int availableSpace;
	switch (dir)
	{
	case 'H':
		availableSpace = horizontalSize - insertionPosition.second;
		break;
	case 'V':
		availableSpace = verticalSize - insertionPosition.first;
		break;
	default:
		cerr << "Invalid input!";
	}
	return availableSpace >= wordSize;
}

//=================================================================================================================================
// Checks if the given word is already on the board or not

bool Puzzle::isWordUsed(string word)
{
	vector<pair<string, string>>::iterator it;
	for (it = playerUsedWords.begin(); it != playerUsedWords.end(); it++)
		if (it->second == word)
			return true;
	return false;
}

//=================================================================================================================================
// Checks if the word matches the positions in the board it will intercept
// Assumes all other conditios are met: valid, not used and fits space

bool Puzzle::matchesInterceptedPositions(string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	switch (direction)
	{
	case 'H':
		for (size_t i = 0; i < word.length(); i++) //for each position of the letter
		{
			if (playerBoard.at(line).at(column + i) != word.at(i) && playerBoard.at(line).at(column + i) != '.') // must either correspond to the word letter or to '.'
				return false;
		}
		break;
	case 'V':
		for (size_t i = 0; i < word.length(); i++)
			if (playerBoard.at(line + i).at(column) != word.at(i) && playerBoard.at(line + i).at(column) != '.')
				return false;
		break;
	default:
		cerr << "Invalid input!";
	}
	return true;
}