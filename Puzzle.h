#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>
#include <map>

#include <Board.h>
#include <Player.h>
#include <ColorMaster.h>

class Puzzle
{
public:
	// Constructors
	Puzzle() {};
	Puzzle(Board &originalBoard, Dictionary &dict);
	Puzzle(Board &originalBoard, Dictionary &dict, Player player);
	Puzzle(Board &originalBoard, Dictionary &dict, std::string boardName, Player player);

	//Getters
	int getNumberOfSolutionWords();
	int getNumberOfPlayerWords();
	int calculateNumberOfCorrectWords();

	void setLoadedBoardNumber(std::string);
	void setPlayer(Player player);
	void setSolutionBoard(Board board);
	void createPlayerBoard();
	void showPlayerBoard();
	void showSolutionBoard();
	void buildClueList();
	void showClueList();
	void showDifferentSynonym(std::string);
	void insertWord(std::string word, std::string position);
	void showWrongAnswers();
	void showPlayerStats();
	void addWrongSubmission();
	void saveStats();

	bool hasHash(std::string position);
	bool isValidInsertionLocation(std::string position);
	bool validPositionInput(std::string);
	bool canBeInserted(std::string word, std::string position);
	bool removeWord(std::string position);
	bool boardsMatch();

private:

	std::string loadedBoardNumber;
	std::vector<std::vector<char>> solutionBoard; //Only has the 2D vector of the board so as to be more independent of Board Class
	std::vector<std::vector<char>> playerBoard;

	std::vector<std::pair<std::string, std::string>> solutionUsedWords; // vector that stores used words as a pair (position [all uppercase], word)
	std::vector<std::pair<std::string, std::string>> playerUsedWords; // vector that stores used words as a pair (position [all uppercase], word)
	std::vector<std::pair<std::string, std::string>> clueList; // list of clues for each placed word (position[upper,lower,upper], clue)

	Dictionary dictionary; //dictionary to be used
	Player currentPlayer;
	ColorMaster colorMaster;

	int horizontalSize;
	int verticalSize;	
	int numberOfSolutionWords;
	int numberOfPlayerWords = 0;

	std::pair<int, int> calculateInsertionCoordinates(std::string coordinates);
	int mapCharToNumber(char letter);
	std::string toUpperString(std::string word);
	bool isValidHeadline(std::string);
	bool wordFitsSpace(std::string word, std::string position);
	bool isWordUsed(std::string word);
	bool matchesInterceptedPositions(std::string word, std::string position);
	bool wordInterceptsPosition(std::string targetPosition, std::string word, std::string wordPosition);
	bool adjacentSpacesEmpty(std::pair<int, int> coordinates, char direction);
};


#endif