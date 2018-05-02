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
	Board(int horiSize, int vertSize, Dictionary& dict);

	inline void setDictionary(Dictionary& dict);
	void showBoard();
	bool canBeInserted(std::string word, std::string position);
	void insertWord(std::string word, std::string position);
	void removeWord(std::string position);
	// TODO Method to automatically insert hashes in places where no more words fit
	void InsertHashes(); 
	void helpUser(std::string position);
	bool validPositionInput(std::string);
	bool validBoard();
	void savePuzzle(std::string name);
	void loadPuzzle(std::string name);

private:
	std::vector<std::vector<char>> board;
	std::vector<std::pair<std::string, std::string>> usedWords; // vector that stores used words as a pair (position, word)

	int horizontalSize = 0, verticalSize = 0;
	Dictionary *dictionary; //pointer to the dictionary in use


	std::pair<int, int> calculateInsertionCoordinates(std::string coordinates);
	int mapCharToNumber(char letter);
	int calculateBound(std::pair<int, int> coords, char direction);

	bool isValidHeadline(std::string);
	bool isWordUsed(std::string word);
	bool wordFitsSpace(std::string word, std::string position);
	bool matchesCurrentBoard(std::string word, std::string position);
	bool wordInterceptsPosition(std::string targetPosition, std::string word, std::string wordPosition);
	bool adjacentSpacesEmpty(std::pair<int,int> coordinates, char direction);
};

#endif
