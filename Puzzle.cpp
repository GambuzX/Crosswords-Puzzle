/*
Definitions of the puzzle class.

AUTHOR: Mario Gil.
*/

#include "Puzzle.h"
#include "Board.h"
#include "ColorMaster.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//==========================================================================================
//COLOR CODES:

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

#define DEFAULT 15
#define QUESTION_COLOR 11
#define SYMBOL_COLOR 14
#define ERROR_MESSAGE 4
#define SUCCESS 10

//=================================================================================================================================
// Class constructor. Assigns a board and a dictionary to the Puzzle.

Puzzle::Puzzle(Board &board, Dictionary &dict)
{
	solutionBoard = board.getBoard();
	solutionUsedWords = board.getUsedWords();
	verticalSize = board.getVerticalSize();
	horizontalSize = board.getHorizontalSize();
	dictionary = dict;
	numberOfSolutionWords = (int) solutionUsedWords.size();
	numberOfPlayerWords = 0;
}

//=================================================================================================================================
// Class constructor. Assigns a board, a dictionary and a player to the Puzzle.

Puzzle::Puzzle(Board &board, Dictionary &dict, Player player)
{
	solutionBoard = board.getBoard();
	solutionUsedWords = board.getUsedWords();
	verticalSize = board.getVerticalSize();
	horizontalSize = board.getHorizontalSize();
	dictionary = dict;
	currentPlayer = player; 
	numberOfSolutionWords = (int) solutionUsedWords.size();
	numberOfPlayerWords = 0;
}

//=================================================================================================================================
// Class constructor. Assigns a board, a dictionary and a player to the Puzzle. Also stores the board number.

Puzzle::Puzzle(Board &board, Dictionary &dict, Player player, string boardNumber)
{
	solutionBoard = board.getBoard();
	solutionUsedWords = board.getUsedWords();
	verticalSize = board.getVerticalSize();
	horizontalSize = board.getHorizontalSize();
	dictionary = dict;
	currentPlayer = player;
	numberOfSolutionWords = (int) solutionUsedWords.size();
	numberOfPlayerWords = 0;
	loadedBoardNumber = boardNumber;
}

//=================================================================================================================================
// Returns number of words in the solution board.

int Puzzle::getNumberOfSolutionWords(){	return numberOfSolutionWords;}

//=================================================================================================================================
// Returns number of words in the player board.

int Puzzle::getNumberOfPlayerWords(){	return numberOfPlayerWords;}

//=================================================================================================================================
// Returns number of well placed words in the player board.

int Puzzle::calculateNumberOfCorrectWords()
{
	int counter = 0;
	for (size_t i = 0; i < playerUsedWords.size(); i++)
	{
		string position = playerUsedWords.at(i).first;
		string userWord = playerUsedWords.at(i).second;

		string solutionWord;
		for (size_t j = 0; j < solutionUsedWords.size(); j++)
		{
			if (solutionUsedWords.at(j).first == position)
				solutionWord = solutionUsedWords.at(j).second;
		}

		if (userWord == solutionWord)
			counter++;
	}
	return counter;
}

//=================================================================================================================================
// Stores the board number given a board with the name format: bXXX.txt

void Puzzle::setLoadedBoardNumber(string name) { loadedBoardNumber = name.substr(1, 3); }

//=================================================================================================================================
// Changes current player.

void Puzzle::setPlayer(Player player) { currentPlayer = player; }

//=================================================================================================================================
// Changes current solution board to the one in the Board object. Also changes solution words vector and dimensions.

void Puzzle::setSolutionBoard(Board &board)
{
	solutionBoard = board.getBoard();
	solutionUsedWords = board.getUsedWords();
	verticalSize = board.getVerticalSize();
	horizontalSize = board.getHorizontalSize();
}

//=================================================================================================================================
// Creates a board for the player to add words. Created from the original board by replacing letters with dots.

void Puzzle::createPlayerBoard()
{
	playerBoard = solutionBoard;
	for (int i = 0; i < verticalSize; i++)
		for (int j = 0; j < horizontalSize; j++)
			if (isalpha(playerBoard.at(i).at(j)))
				playerBoard.at(i).at(j) = '.';
}

//=================================================================================================================================
// Shows an "empty board", only white and black cells, in the console.

void Puzzle::showPlayerBoard()
{
	const int WIDTH = 2;

	cout << setw(WIDTH) << " ";
	for (size_t i = 0; i < playerBoard.at(0).size(); i++)
	{
		colorMaster.setcolor(RED);
		cout << setw(WIDTH) << (char)('a' + i);
	}
	cout << endl;

	for (size_t i = 0; i < playerBoard.size(); i++)
	{
		colorMaster.setcolor(RED);
		cout << (char)('A' + i) << " ";

		for (size_t j = 0; j < playerBoard.at(i).size(); j++)
		{
			if (playerBoard.at(i).at(j) == '#')
			{
				colorMaster.setcolor(WHITE, BLACK);
				cout << "  "; //empty black space
			}
			else if (playerBoard.at(i).at(j) == '.')
			{
				colorMaster.setcolor(BLACK, WHITE);
				cout << "  "; //empty white space
			}
			else
			{
				colorMaster.setcolor(BLACK, WHITE);
				cout << setw(WIDTH) << playerBoard.at(i).at(j); // show user input letters
			}
		}
		cout << '\n';
	}
	colorMaster.setcolor(WHITE, BLACK); //set to default
}

//==================================================================================================================================
// Shows the solution board with the words inserted by the player in the console.

void Puzzle::showSolutionBoard()
{
	const int WIDTH = 2;

	cout << setw(WIDTH) << " ";
	for (size_t i = 0; i < solutionBoard.at(0).size(); i++)
	{
		colorMaster.setcolor(RED);
		cout << setw(WIDTH) << (char)('a' + i);
	}
	cout << endl;

	for (size_t i = 0; i < solutionBoard.size(); i++)
	{
		colorMaster.setcolor(RED);
		cout << (char)('A' + i) << " ";

		for (size_t j = 0; j < solutionBoard.at(i).size(); j++)
		{
			if (solutionBoard.at(i).at(j) == '#')
			{
				colorMaster.setcolor(WHITE, BLACK);
				cout << "  ";
			}
			else
			{
				colorMaster.setcolor(BLACK, WHITE);
				cout << setw(WIDTH) << solutionBoard.at(i).at(j);
			}
		}
		cout << '\n';
	}
	colorMaster.setcolor(WHITE, BLACK); //set to default
}

//=================================================================================================================================
// Builds an initial list of clues selected randomly from the list of available clues for each word.

void Puzzle::buildClueList()
{
	clueList.clear(); //Reset previous clue list
	for (size_t i = 0; i < solutionUsedWords.size(); i++)
	{
		string position = solutionUsedWords.at(i).first; //Location and direction
		position.at(1) = tolower(position.at(1)); //Set column to be lowercase
		string word = solutionUsedWords.at(i).second;
		string clue = dictionary.GetWordSynonym(word); //Random synonym.
		clueList.push_back(pair<string, string>(position, clue));
	}
}

//=================================================================================================================================
// Shows the built list of clues to the user. Assumes that it has been build already.

void Puzzle::showClueList()
{
	//HORIZONTAL
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nHORIZONTAL WORDS CLUES\n";
	colorMaster.setcolor(WHITE, BLACK);
	for (size_t i = 0; i < clueList.size(); i++)
	{
		if (clueList.at(i).first.at(2) == 'H')
		{
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << clueList.at(i).first.substr(0, 2);
			colorMaster.setcolor(DEFAULT); 
			cout << " - " << clueList.at(i).second << endl;
		}
	}

	cout << endl;

	//VERTICAL
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nVERTICAL WORDS CLUES\n";
	colorMaster.setcolor(WHITE, BLACK);
	for (size_t i = 0; i < clueList.size(); i++)
	{
		if (clueList.at(i).first.at(2) == 'V')
		{
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << clueList.at(i).first.substr(0, 2);
			colorMaster.setcolor(DEFAULT); 
			cout << " - " << clueList.at(i).second << endl;
		}
	}
}

//=================================================================================================================================
// Shows a different synonym for the specified word than the one in the initial hints.

void Puzzle::showDifferentSynonym(std::string position)
{
	//Find word in that positions
	string word;
	for (size_t i = 0; i < solutionUsedWords.size(); i++)
		if (toUpperString(solutionUsedWords.at(i).first) == toUpperString(position))
		{
			word = solutionUsedWords.at(i).second;
			break;
		}

	//Find already used synonym
	string usedSynonym;
	for (size_t i = 0; i < clueList.size(); i++)
		if (toUpperString(clueList.at(i).first) == toUpperString(position))
		{
			usedSynonym = clueList.at(i).second;
			break;
		}

	//If there is only one synonym, no other can be found
	if (dictionary.getWordSynonyms(word).size() <= 1)
	{
		cout << "\nThis synonym is unique.\n";
		colorMaster.setcolor(DEFAULT);
		cout << "Hint: ";
		colorMaster.setcolor(SYMBOL_COLOR);
		cout << usedSynonym;
		cout << endl;
		colorMaster.setcolor(DEFAULT);
	}
	//If there are more than one synonyms
	else
	{
		string newSynonym = dictionary.GetWordSynonym(word);
		while (toUpperString(newSynonym) == toUpperString(usedSynonym)) //Find different synonym
			newSynonym = dictionary.GetWordSynonym(word);

		colorMaster.setcolor(DEFAULT);
		cout << "\nNew Hint: ";
		colorMaster.setcolor(SYMBOL_COLOR);
		cout << newSynonym;
		cout << endl;
		colorMaster.setcolor(DEFAULT);

		currentPlayer.incrementNumberOfClues();
	}
}

//=================================================================================================================================
// Inserts a word in the specified position. Assumes all tests have been made.

void Puzzle::insertWord(string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);
	playerUsedWords.push_back(pair<string, string>(positionInput, toUpperString(word))); //add word to the vector

	//Insert word
	char dir = toupper(direction);
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	switch (dir)
	{
	case 'H':
		for (size_t i = 0; i < word.length(); i++)
			playerBoard.at(line).at(column + i) = word.at(i);
		break;
	case 'V':
		for (size_t i = 0; i < word.length(); i++)
			playerBoard.at(line + i).at(column) = word.at(i);
		break;
	default:
		cerr << "Invalid input!";
	}
	numberOfPlayerWords++;
}

//=================================================================================================================================
// Removes an already placed word. If word removed returns true, else returns false.

bool Puzzle::removeWord(std::string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	char direction = positionInput.at(2);

	if (playerBoard.at(line).at(column) == '.' || playerBoard.at(line).at(column) == '#')
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nThere is no word in that location!\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	bool foundWord = false;
	vector<pair<string, string>>::iterator it;
	for (it = playerUsedWords.begin(); it != playerUsedWords.end(); it++)
	{
		string position = it->first;
		string word = it->second;
		if (wordInterceptsPosition(positionInput, word, position)) // If true, word is to be removed
		{
			pair<int, int> wordPos = calculateInsertionCoordinates(position);
			int startLine = wordPos.first;
			int startColumn = wordPos.second;
			char dir = position.at(2);

			switch (dir)
			{
			case 'H':
				for (int i = 0; i < (int) word.length(); i++)
				{
					//If cell is not used by any other word, change it to dot ('.')
					if (!existsWordInterceptingPosition(pair<int, int>(startLine, startColumn + i), 'V'))
						playerBoard.at(startLine).at(startColumn + i) = '.';
				}
				playerUsedWords.erase(it); //iterator is pointing to the element to be removed
				break;
			case 'V':
				for (int i = 0; i < (int) word.length(); i++)
				{
					if (!existsWordInterceptingPosition(pair<int, int>(startLine + i, startColumn), 'H'))
						playerBoard.at(startLine + i).at(startColumn) = '.';
				}
				playerUsedWords.erase(it); //iterator is pointing to the element to be removed
				break;
			default:
				cerr << "Invalid direction!";
			}
			foundWord = true;
			break; //can only remove one word, stops after it
		}
	}
	if (!foundWord)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nThere is no word in the specified direction!\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	numberOfPlayerWords--;
	return true;
}

//=================================================================================================================================
// Shows the user which words he misplaced

void Puzzle::showWrongAnswers()
{
	vector<pair<string, string>> horizontalErrors;
	vector<pair<string,string>> verticalErrors;

	for (size_t i = 0; i < playerUsedWords.size(); i++)
	{
		string position = playerUsedWords.at(i).first;
		string userWord = playerUsedWords.at(i).second;

		string solutionWord;
		for (size_t j = 0; j < solutionUsedWords.size(); j++)
		{
			if (solutionUsedWords.at(j).first == position)
				solutionWord = solutionUsedWords.at(j).second;
		}

		bool correctAnswer = (userWord == solutionWord);
		if (!correctAnswer)
		{
			if (position.at(2) == 'H')
				horizontalErrors.push_back(pair<string, string>(position, userWord));
			else if (position.at(2) == 'V')
				verticalErrors.push_back(pair<string, string>(position, userWord));
		}
	}

	//HORIZONTAL
	if ((int)horizontalErrors.size() > 0)
	{
		colorMaster.setcolor(BLACK, WHITE);
		cout << "\nHORIZONTAL\n";
		colorMaster.setcolor(WHITE, BLACK);
		for (size_t i = 0; i < horizontalErrors.size(); i++)
		{
			string position = horizontalErrors.at(i).first;
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << position.at(0) << (char) tolower(position.at(1));
			colorMaster.setcolor(DEFAULT); 
			cout << " - " << horizontalErrors.at(i).second << endl;
		}
	}

	//VERTICAL
	if ((int)verticalErrors.size() > 0)
	{
		colorMaster.setcolor(BLACK, WHITE);
		cout << "\nVERTICAL\n";
		colorMaster.setcolor(WHITE, BLACK);
		for (size_t i = 0; i < verticalErrors.size(); i++)
		{
			string position = verticalErrors.at(i).first;
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << position.at(0) << (char) tolower(position.at(1));
			colorMaster.setcolor(DEFAULT); 
			cout << " - " << verticalErrors.at(i).second << endl;
		}
	}
}

//=================================================================================================================================
// Displays player stats: name, time to solve, hints asked and wrong submissions.

void Puzzle::showPlayerStats()
{
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nSTATS\n";
	colorMaster.setcolor(WHITE, BLACK);

	cout << "Player name: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << currentPlayer.getName();
	colorMaster.setcolor(DEFAULT);
	cout << endl;

	cout << "Time to solve: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << currentPlayer.calculateTimeSpent() << " seconds";
	colorMaster.setcolor(DEFAULT);
	cout << endl;

	cout << "Number of hints: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << currentPlayer.getNumberOfClues();
	colorMaster.setcolor(DEFAULT);
	cout << endl;

	cout << "Number of wrong submissions: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << currentPlayer.getNumberOfWrongSubmissions();
	colorMaster.setcolor(DEFAULT);
	cout << endl;
}

//=================================================================================================================================
// Increments wrong submission counter of the player.

void Puzzle::addWrongSubmission(){	currentPlayer.incrementWrongSubmissions();}

//=================================================================================================================================
// Saves the player stats to a file on the format bXXX_p.txt. The bool indicates whether the player won or not.

void Puzzle::saveStats(bool finishedGame)
{
	string fileName = "b" + loadedBoardNumber + "_p.txt";
	ofstream file(fileName, ios::app);

	file << "Player name: " << currentPlayer.getName() << "; Time: " << currentPlayer.calculateTimeSpent();
	file << "s; Hints: " << currentPlayer.getNumberOfClues() << "; Wrong submissions: " << currentPlayer.getNumberOfWrongSubmissions();
	file << "; Finished: ";
	if (finishedGame)
		file << "Yes";
	else
		file << "No";
	file << endl;
	file.close();
}

//=================================================================================================================================
// Verifies it the specified position has an hash.

bool Puzzle::hasHash(std::string position)
{
	pair<int, int> coords = calculateInsertionCoordinates(position);
	return playerBoard.at(coords.first).at(coords.second) == '#';
}

//=================================================================================================================================
// Verifies the user position input is valid.

bool Puzzle::validPositionInput(string input)
{
	bool valid = true;

	if (input.length() != 3)
		valid = false;
	else
	{
		char line = toupper(input.at(0));
		char column = toupper(input.at(1));
		char direction = toupper(input.at(2));
		char maxHorizontal = 'A' + horizontalSize - 1;
		char maxVertical = 'A' + verticalSize - 1;

		if (line < 'A' || line > maxVertical)
			valid = false;

		if (column < 'A' || column > maxHorizontal)
			valid = false;

		if (direction != 'H' && direction != 'V')
			valid = false;
	}

	if (!valid)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nInvalid input!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else
		return true;
}

//=================================================================================================================================
// Verifies the given word can be inserted in the specified position.

bool Puzzle::canBeInserted(string word, string position)
{
	// Verify it the position has an hash
	if (!isValidInsertionLocation(position))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nYou can not place a word in that location.\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify if there already has been inserted a word there
	else if (hasWordInPosition(position))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nYou have already inserted a word there!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify word is valid
	else if (!isValidHeadline(word))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is not valid! Please only use characters from 'A' to 'Z'.\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify it fits the space
	else if (!wordFitsSpace(word, position))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord does not fit the specified space!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify if word was already used
	else if (isWordUsed(word))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is already in use!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify if the insertion can be executed while keeping the board valid
	else if (!matchesInterceptedPositions(word, position))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord does not match current board!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	return true;
}

//=================================================================================================================================
// Checks if a given position is a position where there is a word beggining.

bool Puzzle::isValidInsertionLocation(string positionInput)
{
	pair<int, int> position = calculateInsertionCoordinates(positionInput);
	if (playerBoard.at(position.first).at(position.second) == '#') return false;

	vector<pair<string, string>>::iterator it;
	for (it = solutionUsedWords.begin(); it != solutionUsedWords.end(); it++) //Go through all of the solution inserted words
		if (toUpperString(it->first) == toUpperString(positionInput))
			return true;
	return false;
}

//=================================================================================================================================
// Checks if the player board is equal to the solution board, i.e., if the player has won.

bool Puzzle::boardsMatch() { return playerBoard == solutionBoard; }

//=================================================================================================================================
// Converts line / columns letter to respective index.

int Puzzle::mapCharToNumber(char letter)
{
	char upper = toupper(letter);
	return ((int)upper - (int) 'A');
}

//=================================================================================================================================
// Verifies the given headline is valid.

bool Puzzle::isValidHeadline(string word)
{
	for (size_t i = 0; i < word.length(); i++)
	{
		if (word.at(i) < 'A' || word.at(i) > 'Z')
			return false;
	}
	return true;
}

//=================================================================================================================================
// Checks if the word fits in the specified space, i.e., if it occupies the entire space.

bool Puzzle::wordFitsSpace(string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));

	char dir = toupper(direction);
	int wordSize = (int) word.length();
	int availableSpace;
	switch (dir)
	{
	case 'H':
		for (int column = insertionPosition.second; column < horizontalSize; column++)
		{
			if (playerBoard.at(insertionPosition.first).at(column) == '#')
			{
				availableSpace = column - insertionPosition.second;
				break;
			}
			else if (column == horizontalSize - 1) //if on last board position and is not an hash
			{
				availableSpace = horizontalSize - insertionPosition.second;
				break;
			}
		}
		break;
	case 'V':
		for (int line = insertionPosition.first; line < verticalSize; line++)
		{
			if (playerBoard.at(line).at(insertionPosition.second) == '#')
			{
				availableSpace = line - insertionPosition.first;
				break;
			}
			else if (line == verticalSize - 1) //if on last board position and is not an hash
			{
				availableSpace = verticalSize - insertionPosition.first;
				break;
			}
		}
		break;
	default:
		cerr << "Invalid input!";
	}
	return availableSpace == wordSize;
}

//=================================================================================================================================
// Checks if the given word is already on the player board or not.

bool Puzzle::isWordUsed(string word)
{
	vector<pair<string, string>>::iterator it;
	for (it = playerUsedWords.begin(); it != playerUsedWords.end(); it++)
		if (it->second == word)
			return true;
	return false;
}

//=================================================================================================================================
// Checks if the user has already inserted a word in the specified position

bool Puzzle::hasWordInPosition(string position)
{
	vector<pair<string, string>>::iterator it;
	for (it = playerUsedWords.begin(); it != playerUsedWords.end(); it++)
		if (it->first == position)
			return true;
	return false;
}

//=================================================================================================================================
// Checks if the word matches the positions in the board it will intercept
// Assumes all other conditios are met: valid, not used and fits space

bool Puzzle::matchesInterceptedPositions(string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));
	int line = insertionPosition.first;
	int column = insertionPosition.second;

	switch (direction)
	{
	case 'H':
		for (size_t i = 0; i < word.length(); i++) //for each position of the letter
		{
			if (playerBoard.at(line).at(column + i) != word.at(i) && playerBoard.at(line).at(column + i) != '.') // must either correspond to the word letter or to '.'
				return false;
		}
		break;
	case 'V':
		for (size_t i = 0; i < word.length(); i++)
			if (playerBoard.at(line + i).at(column) != word.at(i) && playerBoard.at(line + i).at(column) != '.')
				return false;
		break;
	default:
		cerr << "Invalid input!";
	}
	return true;
}

//=================================================================================================================================
// Checks if a word in the board intercepts determined coordinates in the board.

bool Puzzle::wordInterceptsPosition(string targetPosition, string word, string wordPosition)
{
	// position = (line, column)
	pair<int, int> targetCoords = calculateInsertionCoordinates(targetPosition);
	pair<int, int> wordCoords = calculateInsertionCoordinates(wordPosition);
	char targetDir = targetPosition.at(2);
	char wordDir = wordPosition.at(2);

	if (targetDir != wordDir) //must be on the same direction
		return false;

	switch (targetDir)
	{
	case 'H':
		if (targetCoords.first != wordCoords.first) //if not on the same line, can not match
			return false;
		if ((targetCoords.second < wordCoords.second) || (targetCoords.second > wordCoords.second + (int)word.length())) //if out of the range occupied by the word
			return false;
		break;
	case 'V':
		if (targetCoords.second != wordCoords.second) //if not on the same column, can not match
			return false;
		if ((targetCoords.first < wordCoords.first) || (targetCoords.first > wordCoords.first + (int)word.length())) //if out of the range occupied by the word
			return false;
		break;
	default:
		cerr << "Invalid direction!";
	}
	return true;
}

//=================================================================================================================================
// Checks if any word in the player board intercepts determined coordinates in the board.

bool Puzzle::existsWordInterceptingPosition(pair<int, int> targetCoords, char targetDir)
{
	// position = (line, column)
	for (size_t i = 0; i < playerUsedWords.size(); i++)
	{
		pair<int, int> wordCoords = calculateInsertionCoordinates(playerUsedWords.at(i).first);
		char wordDir = playerUsedWords.at(i).first.at(2);

		bool intercepts = true;

		if (targetDir != wordDir) //must be on the same direction
			intercepts = false;

		switch (targetDir)
		{
		case 'H':
			if (targetCoords.first != wordCoords.first) //if not on the same line, can not match
				intercepts = false;
			if ((targetCoords.second < wordCoords.second) || (targetCoords.second > wordCoords.second + (int)playerUsedWords.at(i).second.length())) //if out of the range occupied by the word
				intercepts = false;
			break;
		case 'V':
			if (targetCoords.second != wordCoords.second) //if not on the same column, can not match
				intercepts = false;
			if ((targetCoords.first < wordCoords.first) || (targetCoords.first > wordCoords.first + (int)playerUsedWords.at(i).second.length())) //if out of the range occupied by the word
				intercepts = false;
			break;
		default:
			cerr << "Invalid direction!";
		}

		if (intercepts)
			return true;
	}
	return false;
}

//=================================================================================================================================
// Determines line and column indexes given text input

pair<int, int> Puzzle::calculateInsertionCoordinates(string coordinates)
{
	pair<int, int> position;
	position.first = mapCharToNumber(coordinates.at(0)); //line
	position.second = mapCharToNumber(coordinates.at(1)); //column
	return position;
}

//=================================================================================================================================
// Converts a string to uppercase.

string Puzzle::toUpperString(string word)
{
	string upper = word;
	for (size_t i = 0; i < upper.length(); i++)
		upper.at(i) = toupper(upper.at(i));
	return upper;
}