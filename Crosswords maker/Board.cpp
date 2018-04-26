#include "Board.h"
#include <iostream>
#include <vector>

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
