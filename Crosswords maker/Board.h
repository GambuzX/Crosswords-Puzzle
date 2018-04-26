#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board
{
public:
	//Constructors
	Board(int horizontalSize, int verticalSize);

	void showBoard();

private:
	std::vector<std::vector<char>> board;

	int horizontalSize = 0, verticalSize = 0;

};

#endif
