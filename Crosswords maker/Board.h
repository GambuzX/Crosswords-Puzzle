#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>

class Board
{
public:
	//Constructors
	Board(int horizontalSize, int verticalSize);

	void showBoard();

	std::pair<int, int> calculateInsertionCoordinates (std::string coordinates);

private:
	std::vector<std::vector<char>> board;

	int horizontalSize = 0, verticalSize = 0;

	int mapCharToNumber(char letter);
};

#endif
