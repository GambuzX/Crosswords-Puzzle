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
	//Board(int horizontalSize, int verticalSize);
	Board(int horizontalSize, int verticalSize, Dictionary& dict);

	void setDictionary(Dictionary& dict);
	void showBoard();
	void insertWord(std::string word, std::pair<int, int> insertionPos, char direction);
	void helpUser(std::pair<int, int> insertionPos, char direction);
	bool validPositionInput(std::string); // TODO check if 3 valid letters only

	std::pair<int, int> calculateInsertionCoordinates (std::string coordinates);

private:
	std::vector<std::vector<char>> board;
	std::set<std::string> usedWords; // used to keep track of words in the board

	int horizontalSize = 0, verticalSize = 0;
	Dictionary *dictionary; //pointer to the dictionary in use

	int mapCharToNumber(char letter);
	bool isValid(std::string);
	bool isWordUsed(std::string word);
	bool wordFitsSpace(std::string word, std::pair<int, int> insertionPos, char direction);
	bool matchesCurrentBoard(std::string word, std::pair<int, int> insertionPos, char direction);
	bool wildcardMatch(const char *str, const char *strWild);
};

#endif
