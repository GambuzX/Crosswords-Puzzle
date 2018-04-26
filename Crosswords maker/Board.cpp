#include "Board.h"
#include <iostream>
#include <vector>
#include <map>

using namespace std;

Board::Board(int horizontalSize, int verticalSize)
{
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	board.resize(verticalSize);
	for (size_t i = 0; i < board.size(); i++)
		board.at(i).resize(horizontalSize);

	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';
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
