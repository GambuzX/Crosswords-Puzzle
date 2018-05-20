/*
Class that represents a Board where words can be inserted / removed.
*
Has many methods to deal with board insertions and deletions, as well as validity tests.
*
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
	// Constructors
	Board() {};
	Board(int horiSize, int vertSize);

	// Getters
	std::vector<std::vector<char>> getBoard();
	std::vector<std::pair<std::string, std::string>> getUsedWords();
	int getVerticalSize();
	int getHorizontalSize();
	char getCell(int line, int column);

	// Setters
	void setBoard(std::vector<std::vector<char>>);
	void setUsedWords(std::vector<std::pair<std::string, std::string>>);

	// Board management and interaction
	void showBoard();
	void insertWord(std::string word, std::string position);
	void insertHash(std::string position);
	bool removeWord(std::string position);
	bool removeWordOrHash(std::string position);
	void removeWordFromUsedWords(int index);
	void insertWordHashes(std::string word, std::string positionInput);
	void removeWordHashes(std::string word, std::string positionInput);
	void reprintHashes();
	void fillRemainingSpots();
	void clearBoard();

	// Utility
	//std::string determineAvailableSpaceWildcard(std::string position);
	std::pair<int, int> calculateInsertionCoordinates(std::string coordinates);

	// Validity tests
	bool validPositionInput(std::string);
	bool isWordUsed(std::string word);
	bool isOnTopOfWord(std::string insertionWord, std::string positionInput);
	bool isSameWordInDifferentPosition(std::string word, std::string position);
	bool wordFitsSpace(std::string word, std::string position);
	bool matchesInterceptedPositions(std::string word, std::string position);
	bool hasHash(std::pair<int, int>);
	bool givenWordInterceptsPosition(std::pair<int,int> targetPosition, char direction, std::string word, std::string wordPosition);
	bool existsWordInterceptingPosition(std::pair<int, int> targetCoords, char targetDir);

	// Save / Load
	bool saveBoard(std::string name, std::string dictName, std::vector<std::pair<std::string, std::string>> autoFormedWords);
	bool loadBoard(std::string name, std::string& dictName);
private:
	// The board itself
	std::vector<std::vector<char>> board;

	// Vector that stores used words as a pair (position, word)
	std::vector<std::pair<std::string, std::string>> usedWords;

	// Board dimensions
	int horizontalSize = 0, verticalSize = 0; 

	// Utility
	int mapCharToNumber(char letter);
};

#endif
