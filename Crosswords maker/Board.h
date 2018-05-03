#ifndef BOARD_H
#define BOARD_H

#include "Dictionary.h"
#include "ColorMaster.h"
#include <vector>
#include <map>
#include <set>

class Board
{
public:
	//Constructors
	//Board(int horizontalSize, int verticalSize);
	Board() {};
	Board(int horiSize, int vertSize, Dictionary dict);

	inline void setDictionary(Dictionary dict);

	void showBoard();
	void insertWord(std::string word, std::string position);
	bool removeWord(std::string position);
	void InsertHashes(std::string word, std::string positionInput);
	void RemoveHashes(std::string word, std::string positionInput);
	void helpUser(std::string position);
	//void helpUserComplete(std::string position);

	bool isInitialized();
	bool hasHash(std::pair<int,int>);
	bool canBeInserted(std::string word, std::string position);
	bool testInsertion(std::string word, std::string position);
	bool validPositionInput(std::string);
	bool validBoard();

	void saveBoard(std::string name);
	bool loadBoard(std::string name);

private:
	std::vector<std::vector<char>> board;
	std::vector<std::pair<std::string, std::string>> usedWords; // vector that stores used words as a pair (position, word)

	bool initializedBoard = false;
	int horizontalSize = 0, verticalSize = 0;

	Dictionary dictionary; //dictionary to be used
	ColorMaster colorMaster;

	std::pair<int, int> calculateInsertionCoordinates(std::string coordinates);
	int mapCharToNumber(char letter);

	bool isValidHeadline(std::string);
	bool isWordUsed(std::string word);
	bool wordFitsSpace(std::string word, std::string position);
	bool matchesCurrentBoard(std::string word, std::string position); //TODO Change name
	bool wordInterceptsPosition(std::string targetPosition, std::string word, std::string wordPosition);
	bool adjacentSpacesEmpty(std::pair<int,int> coordinates, char direction);
};

#endif
