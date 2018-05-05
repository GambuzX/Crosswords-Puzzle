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
	void buildClueList();

private:
	std::vector<std::vector<char>> solutionBoard;
	std::vector<std::vector<char>> playerBoard;
	std::vector<std::pair<std::string, std::string>> usedWords; // vector that stores used words as a pair (position, word)
	std::vector<std::pair<std::string, std::string>> clueList; // list of clues for each placed word

	Dictionary dictionary; //dictionary to be used
	ColorMaster colorMaster;

	int horizontalSize;
	int verticalSize;	
};


#endif