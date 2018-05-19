/*
	Definitions of the board class.

	AUTHOR: Mario Gil.
*/

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
// Set text color. CREDITS TO JAS

void setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

//=================================================================================================================================
// Set text color & background. CREDITS TO JAS

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
// Constructor with board sizes. Assumes board size does not exceed 26.

Board::Board(int horizontalSize, int verticalSize)
{
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	//Resize board
	board.resize(verticalSize);
	for (size_t i = 0; i < board.size(); i++)
		board.at(i).resize(horizontalSize);

	//Set all cells to be dots
	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';
}

//=================================================================================================================================
// Returns the current board

vector<vector<char>> Board::getBoard(){	return board;}

//=================================================================================================================================
// Returns the used words vector

vector<pair<string, string>> Board::getUsedWords(){	return usedWords;}

//=================================================================================================================================
// Returns board vertical size

int Board::getVerticalSize() { return verticalSize; }

//=================================================================================================================================
// Returns board horizontal size

int Board::getHorizontalSize() { return horizontalSize; }

//=================================================================================================================================
// Returns a cell of the board

char Board::getCell(int line, int column) { return board.at(line).at(column); }

//=================================================================================================================================
// Changes current board to the specified one

void Board::setBoard(vector<vector<char>> newBoard){	board = newBoard;}

//=================================================================================================================================
// Changes current used words vector to the specified one

void Board::setUsedWords(vector<pair<string, string>> newUsedWords){	usedWords = newUsedWords;}

//=================================================================================================================================
// Shows current board on the console

void Board::showBoard()
{
	const int WIDTH = 2;

	//First line of letter indexes
	cout << setw(WIDTH) << " ";
	for (size_t i = 0; i < board.at(0).size(); i++)
	{
		setcolor(RED);
		cout << setw(WIDTH) << (char)('a' + i);
	}
	cout << endl;

	for (size_t i = 0; i < board.size(); i++)
	{
		//Letter index
		setcolor(RED);
		cout << (char)('A' + i) << " ";

		//Cell content
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
}

//=================================================================================================================================
// Inserts an hash on the board.

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

	//Verify there is a word there
	if (board.at(line).at(column) == '.' || board.at(line).at(column) == '#')
	{
		setcolor(ERROR_MESSAGE);
		cout << "\nThere is no word in that location!\n";
		setcolor(DEFAULT);
		return false;
	}

	//Loop through all used words to find the one that intercepts the position on that direction
	bool foundWord = false;
	vector<pair<string, string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
	{
		string position = it->first;
		string word = it->second;
		if (givenWordInterceptsPosition(insertionPosition, direction, word, position)) // If true, word is to be removed
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
					if (!existsWordInterceptingPosition(pair<int,int>(startLine,startColumn+i),'V'))
						board.at(startLine).at(startColumn + i) = '.';
				}
				usedWords.erase(it); //iterator is pointing to the element to be removed
				removeWordHashes(word, position);
				reprintHashes();
				break;
			case 'V':
				for (int i = 0; i < (int) word.length(); i++)
				{
					if (!existsWordInterceptingPosition(pair<int, int>(startLine + i, startColumn), 'H'))
						board.at(startLine + i).at(startColumn) = '.';
				}
				usedWords.erase(it); //iterator is pointing to the element to be removed
				removeWordHashes(word, position);
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
		cout << "\nYou have not inserted any word there in the specified direction!\n";
		setcolor(DEFAULT);
		return false;
	}

	//If it is an hash
	else if (board.at(line).at(column) == '#')
	{
		board.at(line).at(column) = '.';
	}
	else
	{
		//Loop through all used words to find the one that intercepts the position on that direction
		bool foundWord = false;
		vector<pair<string, string>>::iterator it;
		for (it = usedWords.begin(); it != usedWords.end(); it++)
		{
			string position = it->first;
			string word = it->second;
			if (givenWordInterceptsPosition(insertionPosition, direction, word, position)) // If true, word is to be removed
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
						if (!existsWordInterceptingPosition(pair<int, int>(startLine, startColumn + i), 'V'))
							board.at(startLine).at(startColumn + i) = '.';
					}
					usedWords.erase(it); //iterator is pointing to the element to be removed
					removeWordHashes(word, position);
					reprintHashes();
					break;
				case 'V':
					for (int i = 0; i < (int) word.length(); i++)
					{
						if (!existsWordInterceptingPosition(pair<int, int>(startLine + i, startColumn), 'H'))
							board.at(startLine + i).at(startColumn) = '.';
					}
					usedWords.erase(it); //iterator is pointing to the element to be removed
					removeWordHashes(word, position);
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
			cout << "\nYou have not inserted any word there in the specified direction!\n";
			setcolor(DEFAULT);
			return false;
		}
	}
	return true;
}

//=================================================================================================================================
// Removes an entry in the used words vector by index. Only erases from the vector, board is unchanged.

void Board::removeWordFromUsedWords(int index){	usedWords.erase(usedWords.begin() + index);}

//=================================================================================================================================
// Places hashes before and after word

void Board::insertWordHashes(string word, string positionInput)
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

void Board::removeWordHashes(string word, string positionInput)
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
// Reprints all hashes of inserted words, i.e., all words in the used words vector.

void Board::reprintHashes()
{
	for (size_t i = 0; i < usedWords.size(); i++)
		insertWordHashes(usedWords.at(i).second, usedWords.at(i).first);
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
// Clears the board, filling it with dots, and resets the used words vector.

void Board::clearBoard()
{
	for (int i = 0; i < verticalSize; i++)
		for (int j = 0; j < horizontalSize; j++)
			board.at(i).at(j) = '.';

	usedWords.clear();
}

//=================================================================================================================================
// Returns line and column indexes given text input

string Board::determineAvailableSpaceWildcard(string position) //TODO assure it works
{
	pair<int, int> coords = calculateInsertionCoordinates(position);
	char dir = position.at(2);
	string wildcard = "";

	if (board.at(coords.first).at(coords.second) == '#') //Nothing matches
		return wildcard;

	//Add letters and '?'
	switch (dir)
	{
	case 'H':
	{
		int currentColumn = coords.second;
		do
		{
			if (isalpha(board.at(coords.first).at(currentColumn)))
				wildcard += board.at(coords.first).at(currentColumn); //add letter
			else
				wildcard += "?"; //add question marks for every cell that is not a letter
			currentColumn++;
		} while (board.at(coords.first).at(currentColumn) != '#' && currentColumn < horizontalSize);
		break;
	}
	case 'V':
	{
		int currentLine = coords.first;
		do
		{
			if (isalpha(board.at(currentLine).at(coords.second)))
				wildcard += board.at(currentLine).at(coords.second); //add letter
			else
				wildcard += "?"; //add question marks for every cell that is not a letter
			currentLine++;
		} while (board.at(currentLine).at(coords.second) != '#' && currentLine < verticalSize);
		break;
	}
	}

	//Go through the end of the wildcard and substitute last '?' for '*'
	int currentIndex = (int)wildcard.length() - 1;
	while (wildcard.at(currentIndex) == '?')
	{
		//special case
		if (wildcard.length() == 1)
		{
			wildcard.at(currentIndex) = '*';
			break;
		}
		//if next element is '?'
		else if (wildcard.at(currentIndex - 1) == '?')
			wildcard.pop_back(); //delete last element
		//if next element is not '?'
		else
			wildcard.at(currentIndex) = '*';
		currentIndex--;
	}

	return wildcard;
}

//=================================================================================================================================
// Returns line and column indexes given text input

pair<int, int> Board::calculateInsertionCoordinates(string coordinates)
{
	pair<int, int> position;
	position.first = mapCharToNumber(coordinates.at(0)); //line
	position.second = mapCharToNumber(coordinates.at(1)); //column
	return position;
}

//=================================================================================================================================
// Verifies the user position input is valid, i.e., respects the format LCD (line, column, direction)

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
// Checks if the word to insert would be inserted on top of another one.

bool Board::isOnTopOfWord(string insertionWord, string positionInput)
{
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));
	switch (direction)
	{
	case 'H':
	{
		//For all positions occupied by the word
		for (int column = insertionPosition.second; column < insertionPosition.second + (int) insertionWord.length(); column++)
			if (!isalpha(board.at(insertionPosition.first).at(column))) //One position not being a letter is enough for it not to be on top of other word
				return false;
		break;
	}
	case 'V':
	{
		//For all positions occupied by the word
		for (int line = insertionPosition.first; line < insertionPosition.first + (int) insertionWord.length(); line++)
			if (!isalpha(board.at(line).at(insertionPosition.second))) //One position not being a letter is enough for it not to be on top of other word
				return false;
		break;
	}
	}
	return true;
}

//=================================================================================================================================
// Checks if the given word is already on the board or not, in another position.

bool Board::isSameWordInDifferentPosition(string word, string position)
{
	vector<pair<string, string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
		if (it->first != position && it->second == word)
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
	int wordSize = (int) word.length();
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
// Checks if a given position has an hash ('#')

bool Board::hasHash(pair<int, int> position) { return board.at(position.first).at(position.second) == '#'; }

//=================================================================================================================================
// Checks if a given word in the board intercepts determined coordinates in the board.

bool Board::givenWordInterceptsPosition(pair<int, int> targetCoords, char targetDir, string word, string wordPosition)
{
	// position = (line, column)
	pair<int, int> wordCoords = calculateInsertionCoordinates(wordPosition);
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
// Checks if any word in the board intercepts determined coordinates in the board.

bool Board::existsWordInterceptingPosition(pair<int, int> targetCoords, char targetDir)
{
	// position = (line, column)
	for (size_t i = 0; i < usedWords.size(); i++)
	{
		pair<int, int> wordCoords = calculateInsertionCoordinates(usedWords.at(i).first);
		char wordDir = usedWords.at(i).first.at(2);

		bool intercepts = true;

		if (targetDir != wordDir) //must be on the same direction
			intercepts = false;

		switch (targetDir)
		{
		case 'H':
			if (targetCoords.first != wordCoords.first) //if not on the same line, can not match
				intercepts = false;
			if ((targetCoords.second < wordCoords.second) || (targetCoords.second > wordCoords.second + (int)usedWords.at(i).second.length())) //if out of the range occupied by the word
				intercepts = false;
			break;
		case 'V':
			if (targetCoords.second != wordCoords.second) //if not on the same column, can not match
				intercepts = false;
			if ((targetCoords.first < wordCoords.first) || (targetCoords.first > wordCoords.first + (int)usedWords.at(i).second.length())) //if out of the range occupied by the word
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
// Saves the board to a file. Returns a boolean indicating whether or not the operation was successfull

bool Board::saveBoard(string fileName, string dictName, vector<pair<string, string>> boardWords)
{
	// Clear board letters and reset used words vector
	for (int i = 0; i < verticalSize; i++)
		for (int j = 0; j < horizontalSize; j++)
			if (isalpha(board.at(i).at(j)))
				board.at(i).at(j) = '.';
	usedWords.clear();


	// Add all the words in the vector to the used words vector
	for (size_t i = 0; i < boardWords.size(); i++)
	{
		insertWord(boardWords.at(i).second, boardWords.at(i).first);
		insertWordHashes(boardWords.at(i).second, boardWords.at(i).first);
	}

	// Organize it well
	ofstream file(fileName);

	// Write dictionary name
	file << dictName << endl << endl;

	// Write board
	for (int i = 0; i < verticalSize; i++)
	{
		for (int j = 0; j < horizontalSize; j++)
		{
			file << board.at(i).at(j) << " ";
		}
		file << endl;
	}
	file << endl;

	//Write used words
	vector<pair<string, string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
	{
		file << it->first << " " << it->second << endl;
	}

	file.close();
	return true;
}

//=================================================================================================================================
// Loads a board from a file and changes the dictName argument for the name of the dictionary that was used.

bool Board::loadBoard(string boardNumber, string& dictName)
{
	// Determine file name from the board number
	string fileName;
	if (boardNumber.length() == 1)
		fileName = "b00" + boardNumber + ".txt";
	else if (boardNumber.length() == 2)
		fileName = "b0" + boardNumber + ".txt";
	else if (boardNumber.length() == 3)
		fileName = "b" + boardNumber + ".txt";
	else
		return false;

	//Open file
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
		horizontalSize = ((int)line.length() + 1) / 2; //has one extra space for each char
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
		insertWordHashes(word, position);
	}

	//If board was completed
	if (!foundDot)
		fillRemainingSpots();
	return true;
}

//=================================================================================================================================
// Converts line / columns letter to respective index.

int Board::mapCharToNumber(char letter)
{
	char upper = toupper(letter);
	return ((int)upper - (int) 'A');
}