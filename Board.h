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
	bool removeWord(std::string position);
	void InsertHashes(std::string word, std::string positionInput);
	void RemoveHashes(std::string word, std::string positionInput);
	void reprintHashes();
	void fillRemainingSpots();
	void helpUser(std::string position, std::vector<std::string> fittingWords);

	bool isInitialized();
	bool validPositionInput(std::string);
	bool isWordUsed(std::string word);
	bool matchesInterceptedPositions(std::string word, std::string position);
	bool hasHash(std::pair<int, int>);
	bool wordFitsSpace(std::string word, std::string position);
	bool wordInterceptsPosition(std::string targetPosition, std::string word, std::string wordPosition);
	bool adjacentSpacesEmpty(std::pair<int, int> coordinates, char direction);

	bool saveBoard(std::string name, std::string dictName);
	bool loadBoard(std::string name, std::string& dictName);

	std::pair<int, int> calculateInsertionCoordinates(std::string coordinates);

private:
	std::vector<std::vector<char>> board;
	std::vector<std::pair<std::string, std::string>> usedWords; // vector that stores used words as a pair (position, word)

	bool initializedBoard = false;
	int horizontalSize = 0, verticalSize = 0;

	int mapCharToNumber(char letter);

};

#endif
