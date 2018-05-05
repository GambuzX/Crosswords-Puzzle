#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>
#include <map>

#include <Board.h>

class Puzzle
{
public:
	// Constructors
	Puzzle() {};
	Puzzle(Board originalBoard);

	void setSolutionBoard(Board board);
	void createPlayerBoard();
	void showPlayerBoard();

private:
	Board solutionBoard;

	std::vector<std::vector<char>> playerBoard;
	std::vector<std::pair<std::string, std::string>> clueList;

	ColorMaster colorMaster;

	int horizontalSize;
	int verticalSize;	
};


#endif