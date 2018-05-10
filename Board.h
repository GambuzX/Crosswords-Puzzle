/*
Class that represents a Board where words can be inserted / removed.
The word board itself is represented as a 2D char vector.
Also keeps track of the words in the board in a vector of pairs of strings (position, word).
*/

#ifndef BOARD_H
#define BOARD_H

#include "Dictionary.h"
#include <vector>
#include <map>
#include <set>

class Board
{
public:
	//Constructors
	Board() {};
	Board(int horiSize, int vertSize);

	//Getters
	std::vector<std::vector<char>> getBoard();
	std::vector<std::pair<std::string, std::string>> getUsedWords();
	int getVerticalSize();
	int getHorizontalSize();
	char getCell(int line, int column);

	void setBoard(std::vector<std::vector<char>>);
	void setUsedWords(std::vector<std::pair<std::string, std::string>>);
	void showBoard();
	void insertWord(std::string word, std::string position);
	void insertHash(std::string position);
	bool removeWord(std::string position);
	bool removeWordOrHash(std::string position);
	void insertWordHashes(std::string word, std::string positionInput);
	void removeWordHashes(std::string word, std::string positionInput);
	void reprintHashes();
	void fillRemainingSpots();
	void clearBoard();

	bool validPositionInput(std::string);
	bool isWordUsed(std::string word);
	bool matchesInterceptedPositions(std::string word, std::string position);
	bool hasHash(std::pair<int, int>);
	bool wordFitsSpace(std::string word, std::string position);
	bool wordInterceptsPosition(std::string targetPosition, std::string word, std::string wordPosition);
	bool adjacentSpacesEmpty(std::pair<int, int> coordinates, char direction);

	bool saveBoard(std::string name, std::string dictName, std::vector<std::pair<std::string, std::string>> autoFormedWords);
	bool loadBoard(std::string name, std::string& dictName);

	std::pair<int, int> calculateInsertionCoordinates(std::string coordinates);

private:
	std::vector<std::vector<char>> board;
	std::vector<std::pair<std::string, std::string>> usedWords; // vector that stores used words as a pair (position, word)

	int horizontalSize = 0, verticalSize = 0;

	int mapCharToNumber(char letter);

};

#endif
