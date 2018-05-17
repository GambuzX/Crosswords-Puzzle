/*
Class that represents a Crosswords Puzzle.
Manages the interaction between a Board, a Player and a Dictionary.
Stores a solution board and a board for the player to interact with. 
Also keeps track of the player used words, as well as a list of synonyms for each word in the solution board.
Has many methods to deal with and control the game flow.
*/

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
	Puzzle(Board &originalBoard, Dictionary &dict, Player player, std::string boardName);

	// Getters
	int getNumberOfSolutionWords();
	int getNumberOfPlayerWords();
	int calculateNumberOfCorrectWords();

	// Setters
	void setLoadedBoardNumber(std::string);
	void setPlayer(Player player);
	void setSolutionBoard(Board &board);

	// Puzzle management / interaction
	void createPlayerBoard();
	void showPlayerBoard();
	void showSolutionBoard();
	void buildClueList();
	void showClueList();
	void showDifferentSynonym(std::string);
	void insertWord(std::string word, std::string position);
	bool removeWord(std::string position);
	void showWrongAnswers();
	void showPlayerStats();
	void addWrongSubmission();
	void saveStats();

	// Validity tests
	bool hasHash(std::string position);
	bool validPositionInput(std::string);
	bool canBeInserted(std::string word, std::string position);
	bool isValidInsertionLocation(std::string position); //TODO copy changed methods from board class
	bool boardsMatch();

private:

	std::string loadedBoardNumber;
	std::vector<std::vector<char>> solutionBoard; // Board that is loaded from the file -> solution
	std::vector<std::vector<char>> playerBoard; // Board which the player tries to fill

	std::vector<std::pair<std::string, std::string>> solutionUsedWords; // all words in the solution board
	std::vector<std::pair<std::string, std::string>> playerUsedWords; // words inserted by the player playing
	std::vector<std::pair<std::string, std::string>> clueList; // list of clues for each placed word (position[upper,lower,upper], clue)

	Dictionary dictionary; //dictionary to be used
	Player currentPlayer;
	ColorMaster colorMaster;

	int horizontalSize;
	int verticalSize;	
	int numberOfSolutionWords;
	int numberOfPlayerWords = 0;

	// Methods only used internally for validating operations
	int mapCharToNumber(char letter);
	bool isValidHeadline(std::string);
	bool wordFitsSpace(std::string word, std::string position);
	bool isWordUsed(std::string word);
	bool hasWordInPosition(std::string position);
	bool matchesInterceptedPositions(std::string word, std::string position);
	bool wordInterceptsPosition(std::string targetPosition, std::string word, std::string wordPosition);
	bool existsWordInterceptingPosition(std::pair<int, int> targetCoords, char targetDir);
	std::pair<int, int> calculateInsertionCoordinates(std::string coordinates);
	std::string toUpperString(std::string word);
};


#endif