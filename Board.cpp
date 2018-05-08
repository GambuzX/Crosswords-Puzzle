#include "Board.h"
#include "Dictionary.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

//=================================================================================================================================
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
// Set text color

void setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

//=================================================================================================================================
// Set text color & background

void setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
			BACKGROUND_RED);
}

//=================================================================================================================================
// Constructor with sizes and dictionary to be used. Assumes board size does not exceed 26.

Board::Board(int horizontalSize, int verticalSize) //, Dictionary dict)
{
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	board.resize(verticalSize);
	for (size_t i = 0; i < board.size(); i++)
		board.at(i).resize(horizontalSize);

	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';
}

//=================================================================================================================================
// Checks if a given position has an hash ('#')

bool Board::hasHash(pair<int,int> position)
{
	return board.at(position.first).at(position.second) == '#';
}

//=================================================================================================================================
// Returns current board

vector<vector<char>> Board::getBoard()
{
	return board;
}

//=================================================================================================================================
// Returns used words vector

vector<pair<string, string>> Board::getUsedWords()
{
	return usedWords;
}

//=================================================================================================================================
// Changes current board

void Board::setBoard(vector<vector<char>> newBoard)
{
	board = newBoard;
}

//=================================================================================================================================
// Changes current used words vector

void Board::setUsedWords(vector<pair<string, string>> newUsedWords)
{
	usedWords = newUsedWords;
}

//=================================================================================================================================
// Returns vertical size

int Board::getVerticalSize()
{
	return verticalSize;
}

//=================================================================================================================================
// Returns horizontal size

int Board::getHorizontalSize()
{
	return horizontalSize;
}

//=================================================================================================================================
// Returns a cell of the board

char Board::getCell(int line, int column)
{
	return board.at(line).at(column);
}

//=================================================================================================================================
// Shows current board

void Board::showBoard()
{
	const int WIDTH = 2;

	cout << setw(WIDTH) << " ";
	for (size_t i = 0; i < board.at(0).size(); i++)
	{
		setcolor(RED);
		cout << setw(WIDTH) << (char)('a' + i);
	}
	cout << endl;

	for (size_t i = 0; i < board.size(); i++)
	{
		setcolor(RED);
		cout << (char)('A' + i) << " ";

		for (size_t j = 0; j < board.at(i).size(); j++)
		{
			if (board.at(i).at(j) == '#')
			{
				setcolor(BLACK, WHITE);
				cout << " ";
				setcolor(WHITE, BLACK);
				cout << "#";
			}
			else
			{
				setcolor(BLACK, WHITE);
				cout << setw(WIDTH) << board.at(i).at(j);
			}
		}
		setcolor(BLACK, BLACK);
		cout << ".";
		cout << '\n';
	}
	setcolor(WHITE, BLACK); //set to default
}

//=================================================================================================================================
// Inserts a word on the board. Assumes it is a valid insertion.

void Board::insertWord(string word, string positionInput)
{
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
	InsertWordHashes(word, positionInput);
}

//=================================================================================================================================
// Inserts a word on the board. Assumes it is a valid insertion.

void Board::insertHash(string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	int line = insertionPosition.first;
	int column = insertionPosition.second;

	if (board.at(line).at(column) == '#')
	{
		setcolor(ERROR_MESSAGE);
		cout << "\nThere already is an hash in that location!\n";
		setcolor(DEFAULT);
	}
	else if (isalpha(board.at(line).at(column)))
	{
		setcolor(ERROR_MESSAGE);
		cout << "\nYou cannot insert an hash over a word!\n";
		setcolor(DEFAULT);
	}
	else
	{
		board.at(line).at(column) = '#';
	}
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
		setcolor(ERROR_MESSAGE);
		cout << "\nThere is no word in that location!\n";
		setcolor(DEFAULT);
		return false;
	}

	bool foundWord = false;
	vector<pair<string, string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
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
				for (size_t i = 0; i < word.length(); i++)
				{
					if (adjacentSpacesEmpty(pair<int,int>(startLine, startColumn + i), dir)) 
						board.at(startLine).at(startColumn + i) = '.';
				}
				usedWords.erase(it); //iterator is pointing to the element to be removed
				RemoveWordHashes(word, position);
				reprintHashes();
				break;
			case 'V':
				for (size_t i = 0; i < word.length(); i++)
				{
					if (adjacentSpacesEmpty(pair<int, int>(startLine + i, startColumn), dir))
						board.at(startLine + i).at(startColumn) = '.';
				}
				usedWords.erase(it); //iterator is pointing to the element to be removed
				RemoveWordHashes(word, position);
				reprintHashes();
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
		setcolor(ERROR_MESSAGE);
		cout << "\nThere is no word in the specified direction!\n";
		setcolor(DEFAULT);
		return false;
	}
	return true;
}

//=================================================================================================================================
// Removes an already placed word or an hash. If no word removed returns false.

bool Board::removeWordOrHash(string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	char direction = positionInput.at(2);

	if (board.at(line).at(column) == '.')
	{
		setcolor(ERROR_MESSAGE);
		cout << "\nThere is nothing to remove in that location!\n";
		setcolor(DEFAULT);
		return false;
	}
	else if (board.at(line).at(column) == '#')
	{
		board.at(line).at(column) = '.';
	}
	else
	{
		bool foundWord = false;
		vector<pair<string, string>>::iterator it;
		for (it = usedWords.begin(); it != usedWords.end(); it++)
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
					for (size_t i = 0; i < word.length(); i++)
					{
						if (adjacentSpacesEmpty(pair<int, int>(startLine, startColumn + i), dir))
							board.at(startLine).at(startColumn + i) = '.';
					}
					usedWords.erase(it); //iterator is pointing to the element to be removed
					RemoveWordHashes(word, position);
					reprintHashes();
					break;
				case 'V':
					for (size_t i = 0; i < word.length(); i++)
					{
						if (adjacentSpacesEmpty(pair<int, int>(startLine + i, startColumn), dir))
							board.at(startLine + i).at(startColumn) = '.';
					}
					usedWords.erase(it); //iterator is pointing to the element to be removed
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
			setcolor(ERROR_MESSAGE);
			cout << "\nThere is no word in the specified direction!\n";
			setcolor(DEFAULT);
			return false;
		}
	}
	return true;
}
//=================================================================================================================================
// Places hashes before and after word

void Board::InsertWordHashes(string word, string positionInput)
{
	pair<int, int> coords = calculateInsertionCoordinates(positionInput);
	int line = coords.first;
	int column = coords.second;
	char dir = positionInput.at(2);

	switch (dir)
	{
	case 'H':
		if (column > 0)
			if (!isalpha(board.at(line).at(column-1))) //only change if it is not a letter
				board.at(line).at(column - 1) = '#';
		if ((column + (int) word.length() - 1) < horizontalSize - 1)
			if (!isalpha(board.at(line).at(column + word.length())))
				board.at(line).at(column + word.length()) = '#';
		break;
	case 'V':
		if (line > 0)
			if (!isalpha(board.at(line - 1).at(column))) //only change if it is not a letter
				board.at(line-1).at(column) = '#';
		if ((line + (int) word.length() - 1) < verticalSize - 1)
			if (!isalpha(board.at(line + word.length()).at(column)))
				board.at(line + word.length()).at(column) = '#';
		break;
	default:
		cerr << "Invalid input!";
	}
}

//=================================================================================================================================
// Removes hashes associated with the word from the given position

void Board::RemoveWordHashes(string word, string positionInput)
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
// Reprints all hashes of inserted words

void Board::reprintHashes()
{
	for (size_t i = 0; i < usedWords.size(); i++)
		InsertWordHashes(usedWords.at(i).second, usedWords.at(i).first);
}

//=================================================================================================================================
// Substitutes all dots in the board for hashes

void Board::fillRemainingSpots()
{
	for (int i = 0; i < verticalSize; i++)
		for (int j = 0; j < horizontalSize; j++)
			if (board.at(i).at(j) == '.')
				board.at(i).at(j) = '#';
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
		setcolor(ERROR_MESSAGE);
		cout << "\nInvalid input!\n\n";
		setcolor(DEFAULT);
		return false;
	}
	else
		return true;
}

//=================================================================================================================================
// Saves the board to a file. Returns a boolean indicating whether or not the operation was successfull

bool Board::saveBoard(string fileName, string dictName)
{
	//if (!isBoardValid()) //FINAL CHECK
		//return false;

	//TODO ADD FINAL CHECK

	// Organize it well
	ofstream file(fileName);
	
	file << dictName << endl << endl;

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
	return true;
}

//=================================================================================================================================
// Loads a board from a file

bool Board::loadBoard(string fileName, string& dictName)
{
	ifstream file(fileName);
	
	if (!file.is_open())
		return false;

	//SETTING DICTIONARY
	getline(file, dictName);

	string line;
	getline(file, line); // empty line

	//SETTING HORIZONTAL AND VERTICAL SIZES
	int verticalSize = 0;
	int horizontalSize = 0;
	bool foundDot = false; //Bool to check whether or not the board was finished
	getline(file, line);
	while (line != "")
	{
		horizontalSize = (line.length() + 1) / 2; //has one extra space for each char
		if (line.find('.') != string::npos)
			foundDot = true;
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
	if (!foundDot)
		fillRemainingSpots();
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
// Converts line / columns letter to respective index.

int Board::mapCharToNumber(char letter)
{
	char upper = toupper(letter);
	return ((int) upper - (int) 'A');
}

//=================================================================================================================================
// Checks if the given word is already on the board or not.

bool Board::isWordUsed(string word)
{
	vector<pair<string,string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
		if (it->second == word)
			return true;
	return false;
}

//=================================================================================================================================
// Checks if the word fits in the specified space. 

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
// Checks if the word matches the positions in the board it will intercept.
// Assumes all other conditios are met: valid, not used and fits space.

bool Board::matchesInterceptedPositions(string word, string positionInput)
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

	switch (direction)
	{
	case 'H':
		if (line == 0) //special case: only check downwards
		{
			if (isalpha(board.at(line + 1).at(column)))
				return false;
		}
		else if (line == verticalSize - 1) //special case: only check upwards
		{
			if (isalpha(board.at(line - 1).at(column)))
				return false;
		}
		else
		{
			if (isalpha(board.at(line + 1).at(column)) || isalpha(board.at(line - 1).at(column))) // check both up and down
				return false;
		}
		break;
	case 'V':
		if (column == 0) //special case: only check right
		{
			if (isalpha(board.at(line).at(column + 1)))
				return false;
		}
		else if (column == (horizontalSize - 1)) //special case: only check left
		{
			if (isalpha(board.at(line).at(column - 1)))
				return false;
		}
		else
		{
			if (isalpha(board.at(line).at(column + 1)) || isalpha(board.at(line).at(column - 1))) // check both right and left
				return false;
		}
		break;
	default:
		cerr << "Invalid direction!";
	}
	return true;
}