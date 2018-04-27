#include "Board.h"
#include "Dictionary.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

/*Board::Board(int horizontalSize, int verticalSize)
{
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	board.resize(verticalSize);
	for (size_t i = 0; i < board.size(); i++)
		board.at(i).resize(horizontalSize);

	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';
}*/

//=================================================================================================================================
// Constructor with sizes and dictionary to be used

Board::Board(int horizontalSize, int verticalSize, Dictionary& dict)
{
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	board.resize(verticalSize);
	for (size_t i = 0; i < board.size(); i++)
		board.at(i).resize(horizontalSize);

	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';
	dictionary = &dict;
}

//=================================================================================================================================
// Changes current dictionary

void Board::setDictionary(Dictionary& dict)
{
	dictionary = &dict;
}

//=================================================================================================================================
// Shows current board

void Board::showBoard()
{
	for (size_t i = 0; i < board.size(); i++)
	{
		for (size_t j = 0; j < board.at(i).size(); j++)
			cout << board.at(i).at(j);
		cout << '\n';
	}
}

//=================================================================================================================================
// Inserts a word on the board. If an invalid operation, detects and outputs what the problem was.

void Board::insertWord(string word, pair<int, int> insertionPos, char direction)
{
	// word matches with already placed letters
	if (!isValid(word)) // verify word is valid
	{
		cout << "Word is not valid!";
	}
	else if (!dictionary->isInWordList(word)) // verify word belongs to the dictionary
	{
		cout << "Word does not belong to the dictionary!";
	}
	else if (!wordFitsSpace(word, insertionPos, direction)) // verify it fits the space
	{
		cout << "Word does not fit specified space!";
	}
	else if (isWordUsed(word))	// verify if word was already used
	{
		cout << "Word is already in use!";
	}
	else if (!matchesCurrentBoard(word, insertionPos, direction))
	{
		cout << "Word does not match previous inserted words!";
	}
	else
	{
		usedWords.insert(word);
		//Insert word
		
	}
}

//=================================================================================================================================
// Determines line and column indexes given text input

pair<int, int> Board::calculateInsertionCoordinates(string coordinates)
{
	pair<int, int> position;
	position.first = mapCharToNumber(coordinates.at(0));
	position.second = mapCharToNumber(coordinates.at(1));
	return position;
}

//=================================================================================================================================
// Converts line / columns letter to respective index

int Board::mapCharToNumber(char letter)
{
	char upper = toupper(letter);
	return ((int) upper - (int) 'A');
}

//=================================================================================================================================
// Verifies the given headline is valid

bool Board::isValid(string word)
{
	for (size_t i = 0; i < word.length(); i++)
	{
		if (word.at(i) < 'A' || word.at(i) > 'Z')
			return false;
	}
	return true;
}

//=================================================================================================================================
// Checks if the given word is already on the board or not

bool Board::isWordUsed(string word)
{
	set<string>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
		if (*it == word)
			return true;
	return false;
}