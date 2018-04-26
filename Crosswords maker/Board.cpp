#include "Board.h"
#include "Dictionary.h"
#include <iostream>
#include <vector>
#include <map>

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

void Board::setDictionary(Dictionary& dict)
{
	dictionary = &dict;
}

//=================================================================================================================================

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
		//Insert word
	}
}

//=================================================================================================================================

pair<int, int> Board::calculateInsertionCoordinates(string coordinates)
{
	pair<int, int> position;
	position.first = mapCharToNumber(coordinates.at(0));
	position.second = mapCharToNumber(coordinates.at(1));
	return position;
}

//=================================================================================================================================

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