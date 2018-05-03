#include "Board.h"
#include "Dictionary.h"
#include "ColorMaster.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <set>

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
//=================================================================================================================================
// Constructor with sizes and dictionary to be used. Assumes board size does not exceed 26.

Board::Board(int horizontalSize, int verticalSize, Dictionary dict)
{
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	board.resize(verticalSize);
	for (size_t i = 0; i < board.size(); i++)
		board.at(i).resize(horizontalSize);

	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';
	dictionary = dict;
	initializedBoard = true;
}

//=================================================================================================================================
// Checks if the board was already initialized. The board has a bool that is updated whenever it's initialized.

bool Board::isInitialized()
{
	return initializedBoard;
}

//=================================================================================================================================
// Checks if a given position has an hash ('#')

bool Board::hasHash(pair<int,int> position)
{
	return board.at(position.first).at(position.second) == '#';
}

//=================================================================================================================================
// Changes current dictionary

inline void Board::setDictionary(Dictionary dict)
{
	dictionary = dict;
}

//=================================================================================================================================
// Shows current board

void Board::showBoard()
{
	const int WIDTH = 2;

	cout << setw(WIDTH) << " ";
	for (size_t i = 0; i < board.at(0).size(); i++)
	{
		colorMaster.setcolor(RED);
		cout << setw(WIDTH) << (char)('a' + i);
	}
	cout << endl;

	for (size_t i = 0; i < board.size(); i++)
	{
		colorMaster.setcolor(RED);
		cout << (char)('A' + i) << " ";

		for (size_t j = 0; j < board.at(i).size(); j++)
		{
			if (board.at(i).at(j) == '#')
			{
				colorMaster.setcolor(BLACK, LIGHTGRAY);
				cout << " ";
				colorMaster.setcolor(LIGHTGRAY, BLACK);
				cout << "#";
			}
			else
			{
				colorMaster.setcolor(BLACK, LIGHTGRAY);
				cout << setw(WIDTH) << board.at(i).at(j);
			}
		}
		cout << '\n';
	}
	colorMaster.setcolor(LIGHTGRAY, BLACK); //set to default
}

//=================================================================================================================================
// Verifies if a word can be inserted in a determined location, informing why not if false

bool Board::canBeInserted(string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);

	if (hasHash(insertionPosition)) // Verify it the position has an hash
	{
		cout << "You can not place a word in that location\n\n";
		return false;
	}
	else if (!isValidHeadline(word)) // Verify word is valid
	{
		cout << "Word is not valid! Please only use characters from 'A' to 'Z'\n\n";
		return false;
	}
	else if (!dictionary.isInWordList(word)) // Verify word belongs to the dictionary
	{
		cout << "Word is not present in the dictionary!\n\n";
		return false;
	}
	else if (!wordFitsSpace(word, positionInput)) // Verify it fits the space
	{
		cout << "Word does not fit the specified space!\n\n";
		return false;
	}
	else if (isWordUsed(word))	// Verify if word was already used
	{
		cout << "Word is already in use!\n\n";
		return false;
	}
	else if (!matchesCurrentBoard(word, positionInput) || !testInsertion(word,positionInput)) // Verify if the insertion can be executed while keeping the board valid
	{
		cout << "Word does not match current board!\n\n";
		return false;
	}

	// TODO Prevent words on top of each other
	return true;
}

//=================================================================================================================================
// Simulates an insertion and verifies if the resulting board is valid.

bool Board::testInsertion(string word, string positionInput)
{
	Board testBoard = *this;
	testBoard.insertWord(word, positionInput);
	return testBoard.validBoard();
}


//=================================================================================================================================
// Inserts a word on the board. Assumes it is a valid insertion.

void Board::insertWord(string word, string positionInput)
{
	//BACKUPS
	//vector<vector<char>> oldBoard = board;
	//vector<pair<string, string>> oldUsedWords = usedWords;

	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);
	usedWords.push_back(pair<string,string>(positionInput, word)); //add word to the vector

	//Insert word
	char dir = toupper(direction);
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	switch (dir)
	{
	case 'H':
		for (size_t i = 0; i < word.length(); i++)
			board.at(line).at(column + i) = word.at(i);
		break;
	case 'V':
		for (size_t i = 0; i < word.length(); i++)
			board.at(line+i).at(column) = word.at(i);
		break;
	default:
		cerr << "Invalid input!";
	}
	InsertHashes(word, positionInput);

	//if (!validBoard()) //If insertWord broke the board, restore backup
	//{
	//	cout << "Word does not match current board!\n\n";
	//	board = oldBoard;
	//	usedWords = oldUsedWords;
	//}
}

//=================================================================================================================================
// Removes an already placed word. If no word removed returns false.

bool Board::removeWord(string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	char direction = positionInput.at(2);

	if (board.at(line).at(column) == '.' || board.at(line).at(column) == '#')
	{
		cout << "\nThere is no word in that location!\n";
		return false;
	}

	bool foundWord = false;
	vector<pair<string, string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
	{
		string position = it->first;
		string word = it->second;
		if (wordInterceptsPosition(positionInput, word, position))
		{
			pair<int, int> wordPos = calculateInsertionCoordinates(position);
			int startLine = wordPos.first;
			int startColumn = wordPos.second;
			char dir = position.at(2);

			switch (dir)
			{
			case 'H':
				for (size_t i = 0; i < word.length(); i++)
				{
					if (adjacentSpacesEmpty(pair<int,int>(startLine, startColumn + i), dir)) 
						board.at(startLine).at(startColumn + i) = '.';
				}
				usedWords.erase(it); //iterator is pointing to the element to be removed
				RemoveHashes(word, position);
				break;
			case 'V':
				for (size_t i = 0; i < word.length(); i++)
				{
					if (adjacentSpacesEmpty(pair<int, int>(startLine + i, startColumn), dir))
						board.at(startLine + i).at(startColumn) = '.';
				}
				usedWords.erase(it); //iterator is pointing to the element to be removed
				RemoveHashes(word, position);
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
		cout << "\nThere is no word in the specified direction!\n";
		return false;
	}
	return true;
}

//=================================================================================================================================
// Places hashes before and after word

void Board::InsertHashes(string word, string positionInput)
{
	pair<int, int> coords = calculateInsertionCoordinates(positionInput);
	int line = coords.first;
	int column = coords.second;
	char dir = positionInput.at(2);

	switch (dir)
	{
	case 'H':
		if (column > 0)
			board.at(line).at(column - 1) = '#';
		if ((column + (int) word.length() - 1) < horizontalSize - 1)
			board.at(line).at(column + word.length()) = '#';
		break;
	case 'V':
		if (line > 0)
			board.at(line-1).at(column) = '#';
		if ((line + (int) word.length() - 1) < verticalSize - 1)
			board.at(line + word.length()).at(column) = '#';
		break;
	default:
		cerr << "Invalid input!";
	}
}

//=================================================================================================================================
// Removes hashes associated with the word from the given position

void Board::RemoveHashes(string word, string positionInput)
{
	pair<int, int> coords = calculateInsertionCoordinates(positionInput);
	int line = coords.first;
	int column = coords.second;
	char dir = positionInput.at(2);

	switch (dir)
	{
	case 'H':
		if (column > 0)
			if (board.at(line).at(column - 1) == '#')
				board.at(line).at(column - 1) = '.';
		if ((column + (int) word.length() - 1) < horizontalSize - 1)
			if (board.at(line).at(column + word.length()) == '#')
				board.at(line).at(column + word.length()) = '.';
		break;
	case 'V':
		if (line > 0)
			if (board.at(line - 1).at(column) == '#')
				board.at(line - 1).at(column) = '.';
		if ((line + (int) word.length() - 1) < verticalSize - 1)
			if (board.at(line + word.length()).at(column) == '#')
				board.at(line + word.length()).at(column) = '.';
		break;
	default:
		cerr << "Invalid input!";
	}
}

//=================================================================================================================================
// Shows the user what words he can put in the specified location

void Board::helpUser(string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));
	int availableSpace;
	switch (direction)
	{
	case 'H':
		availableSpace = horizontalSize - insertionPosition.second;
		break;
	case 'V':
		availableSpace = verticalSize - insertionPosition.first;
		break;
	default:
		cerr << "Invalid input!";
	}

	cout << "Words you can fit there:\n";
	cout << "________________________\n";

	vector<string> fittingWords = dictionary.fittingWords(availableSpace);
	int counter = 0;
	const int WORDS_PER_LINE = 6;
	const int WORDS_WIDTH = 18;
	for (size_t i = 0; i < fittingWords.size(); i++)
	{
		string currentWord = fittingWords.at(i);
		if (!isWordUsed(currentWord) && matchesCurrentBoard(currentWord, positionInput))
		{
			if (counter % WORDS_PER_LINE == 0) cout << endl;
			cout << setw(WORDS_WIDTH) << currentWord;
			counter++;
		}
	}
}

//=================================================================================================================================
// Verifies the user position input is valid

bool Board::validPositionInput(string input)
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
		cout << "Invalid input!\n\n";
		return false;
	}
	else
		return true;
}

//=================================================================================================================================
// Verifies if all words in the board in both directions are valid

bool Board::validBoard()
{
	bool valid = true;

	//HORIZONTAL
	for (int line = 0; line < verticalSize; line++)
	{
		string currentWord = "";
		for (int column = 0; column < horizontalSize; column++)
		{
			if (isalpha(board.at(line).at(column)))
			{
				currentWord += board.at(line).at(column);
			}
			else
			{
				if (currentWord.length() >= 2) //only check if word size is bigger than 1
					if (!dictionary.isInWordList(currentWord)) //if word does not exist
						valid = false;
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() >= 2) //only check if word size is bigger than 1
			if (!dictionary.isInWordList(currentWord)) //if word does not exist
				valid = false;
	}

	//VERTICAL
	for (int column = 0; column < horizontalSize; column++)
	{
		string currentWord = "";
		for (int line = 0; line < verticalSize; line++)
		{
			if (isalpha(board.at(line).at(column)))
			{
				currentWord += board.at(line).at(column);
			}
			else
			{
				if (currentWord.length() >= 2) //only check if word size is bigger than 1
					if (!dictionary.isInWordList(currentWord)) //if word does not exist
						valid = false;
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() >= 2) //only check if word size is bigger than 1
			if (!dictionary.isInWordList(currentWord)) //if word does not exist
				valid = false;
	}
		
	return valid;
}

//=================================================================================================================================
// Saves the board to a file

void Board::saveBoard(string fileName)
{
	//TODO implement when certain that no other attributes will be added
	// Organize it well
	ofstream file(fileName);
	
	file << dictionary.getName() << endl << endl;

	for (int i = 0; i < verticalSize; i++)
	{
		for (int j = 0; j < horizontalSize; j++)
		{
			file << board.at(i).at(j) << " ";
		}
		file << endl;
	}
	file << endl;

	vector<pair<string, string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
	{
		file << it->first << " " << it->second << endl;
	}

	file.close();
}

//=================================================================================================================================
// Loads a board from a file

bool Board::loadBoard(string fileName)
{
	ifstream file(fileName);
	
	if (!file.is_open())
		return false;

	//SETTING DICTIONARY
	string dictName;
	getline(file, dictName);
	Dictionary dict(dictName);
	bool dictionaryOpened = dict.ProcessDictionary();
	if (!dictionaryOpened)
	{
		cout << "\nDictionary file was not found!\n";
		return false;
	}
	setDictionary(dict);

	string line;
	getline(file, line); // empty line

	//SETTING HORIZONTAL AND VERTICAL SIZES
	int verticalSize = 0;
	int horizontalSize = 0;
	getline(file, line);
	while (line != "") //TODO Verify this works
	{
		horizontalSize = (line.length() + 1) / 2; //has one extra space for each char
		verticalSize++;
		getline(file, line);
	}
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	//RESIZE BOARD
	board.resize(verticalSize);
	for (int i = 0; i < verticalSize; i++)
		board.at(i).resize(horizontalSize);

	//INITIALIZE WITH DOTS
	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';

	//INSERT WORDS THAT WERE ON THE BOARD
	usedWords.clear();
	while (getline(file, line))
	{
		string position = line.substr(0, 3);
		string word = line.substr(4); //from index 4 to the end
		insertWord(word, position);
	}	
	initializedBoard = true;
	return true;
}

//=================================================================================================================================
// Determines line and column indexes given text input

pair<int, int> Board::calculateInsertionCoordinates(string coordinates)
{
	pair<int, int> position;
	position.first = mapCharToNumber(coordinates.at(0)); //line
	position.second = mapCharToNumber(coordinates.at(1)); //column
	return position;
}

//=================================================================================================================================
// Converts line / columns letter to respective index

int Board::mapCharToNumber(char letter)
{
	char upper = toupper(letter);
	return ((int) upper - (int) 'A');
}

//=================================================================================================================================
// Verifies the given headline is valid

bool Board::isValidHeadline(string word)
{
	for (size_t i = 0; i < word.length(); i++)
	{
		if (word.at(i) < 'A' || word.at(i) > 'Z')
			return false;
	}
	return true;
}

//=================================================================================================================================
// Checks if the given word is already on the board or not

bool Board::isWordUsed(string word)
{
	vector<pair<string,string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
		if (it->second == word)
			return true;
	return false;
}

//=================================================================================================================================
// Checks if the word fits in the specified space

bool Board::wordFitsSpace(string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));

	char dir = toupper(direction);
	int wordSize = word.length();
	int availableSpace;
	switch (dir){
	case 'H':
		availableSpace = horizontalSize - insertionPosition.second;
		break;
	case 'V':
		availableSpace = verticalSize - insertionPosition.first;
		break;
	default:
		cerr << "Invalid input!";
	}
	return availableSpace >= wordSize;
}

//=================================================================================================================================
// Checks if the word matches the current board, i.e. the words already placed
// Assumes all other conditios are met: valid, not used and fits space

bool Board::matchesCurrentBoard(string word, string positionInput)
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
			if (board.at(line).at(column+i) != word.at(i) && board.at(line).at(column + i) != '.') // must either correspond to the word letter or to '.'
				return false;			
		}
		break;
	case 'V':
		for (size_t i = 0; i < word.length(); i++)
			if (board.at(line + i).at(column) != word.at(i) && board.at(line + i).at(column) != '.')
				return false;
		break;
	default:
		cerr << "Invalid input!";
	}
	return true;

	//TODO Check it matches from all sides, not just positions I want to place
}

//=================================================================================================================================
// Checks if a word in the board intercepts determined coordinates in the board

bool Board::wordInterceptsPosition(string targetPosition, string word, string wordPosition)
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
		if ((targetCoords.second < wordCoords.second) || (targetCoords.second > wordCoords.second + (int) word.length())) //if out of the range occupied by the word
			return false;
		break;
	case 'V':
		if (targetCoords.second != wordCoords.second) //if not on the same column, can not match
			return false;
		if ((targetCoords.first < wordCoords.first) || (targetCoords.first > wordCoords.first + (int) word.length())) //if out of the range occupied by the word
			return false;
		break;
	default:
		cerr << "Invalid direction!";
	}
	return true;
}

//=================================================================================================================================
// Checks if the adjacent spaces of a given position on a direction are empty

bool Board::adjacentSpacesEmpty(pair<int, int> coordinates, char direction)
{
	int line = coordinates.first;
	int column = coordinates.second;

	bool empty = false;
	switch (direction)
	{
	case 'H':
		if (line == 0) //special case: only check downwards
		{
			if (board.at(line + 1).at(column) == '.' || board.at(line + 1).at(column) == '#')
				empty = true;
			else
				empty = false;
		}
		else if (line == verticalSize-1) //special case: only check upwards
		{
			if (board.at(line - 1).at(column) == '.' || board.at(line - 1).at(column) == '#')
				empty = true;
			else
				empty = false;
		}
		else
		{
			if ((board.at(line + 1).at(column) == '.' || board.at(line + 1).at(column) == '#') && (board.at(line - 1).at(column) == '.' || board.at(line - 1).at(column) == '#')) // check both up and down
				empty = true;
			else
				empty = false;
		}
		break;
	case 'V':
		if (column == 0) //special case: only check right
		{
			if (board.at(line).at(column + 1) == '.' || board.at(line).at(column + 1) == '#')
				empty = true;
			else
				empty = false;
		}
		else if (column == (horizontalSize - 1)) //special case: only check left
		{
			if (board.at(line).at(column - 1) == '.' || board.at(line).at(column - 1) == '#')
				empty = true;
			else
				empty = false;
		}
		else
		{
			if ((board.at(line).at(column + 1) == '.' || board.at(line).at(column + 1) == '#') && (board.at(line).at(column - 1) == '.' || board.at(line).at(column - 1) == '#')) // check both right and left
				empty = true;
			else
				empty = false;
		}
		break;
	default:
		cerr << "Invalid direction!";
	}
	return empty;
}