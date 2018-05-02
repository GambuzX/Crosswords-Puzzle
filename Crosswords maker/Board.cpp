#include "Board.h"
#include "Dictionary.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

//=================================================================================================================================
// Constructor with sizes and dictionary to be used. Assumes board size does not exceed 26.

Board::Board(int horizontalSize, int verticalSize, Dictionary& dict)
{
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	board.resize(verticalSize);
	for (size_t i = 0; i < board.size(); i++)
		board.at(i).resize(horizontalSize);

	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';
	dictionary = &dict;
}

//=================================================================================================================================
// Changes current dictionary

inline void Board::setDictionary(Dictionary& dict)
{
	dictionary = &dict;
}

//=================================================================================================================================
// Shows current board

void Board::showBoard()
{
	const int WIDTH = 2;

	cout << setw(WIDTH) << " ";
	for (size_t i = 0; i < board.at(0).size(); i++)
		cout << setw(WIDTH) << (char) ('a' + i);
	cout << endl;

	for (size_t i = 0; i < board.size(); i++)
	{
		cout << setw(WIDTH) << (char)('A' + i);
		for (size_t j = 0; j < board.at(i).size(); j++)
			cout << setw(WIDTH) << board.at(i).at(j);
		cout << '\n';
	}
}

//=================================================================================================================================
// Verifies if a word can be inserted in a determined location, informing why not if false

bool Board::canBeInserted(string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);

	if (!isValidHeadline(word)) // verify word is valid
	{
		cout << "Word is not valid! Please only use characters from 'A' to 'Z'\n\n";
		return false;
	}
	else if (!dictionary->isInWordList(word)) // verify word belongs to the dictionary
	{
		cout << "Word is not present in the dictionary!\n\n";
		return false;
	}
	else if (!wordFitsSpace(word, positionInput)) // verify it fits the space
	{
		cout << "Word does not fit the specified space!\n\n";
		return false;
	}
	else if (isWordUsed(word))	// verify if word was already used
	{
		cout << "Word is already in use!\n\n";
		return false;
	}
	else if (!matchesCurrentBoard(word, positionInput))
	{
		cout << "Word does not match current board!\n\n";
		return false;
	}

	// TODO Prevent words on top of each other
	return true;
}


//=================================================================================================================================
// Inserts a word on the board. Assumes it is a valid insertion.

void Board::insertWord(string word, string positionInput)
{
	//BACKUPS
	vector<vector<char>> oldBoard = board;
	vector<pair<string, string>> oldUsedWords = usedWords;


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

	if (!validBoard()) //If insertWord broke the board, restore backup
	{
		cout << "Word does not match current board!\n\n";
		board = oldBoard;
		usedWords = oldUsedWords;
	}
}

//=================================================================================================================================
// Removes an already placed word

void Board::removeWord(string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = calculateInsertionCoordinates(positionInput);
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	char direction = positionInput.at(2);

	if (board.at(line).at(column) == '.' || board.at(line).at(column) == '#')
	{
		cout << "\nThere is no word in that location!\n";
		return;
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
				for (int i = 0; i < word.length(); i++)
				{
					if (adjacentSpacesEmpty(pair<int,int>(startLine, startColumn + i), dir)) 
						board.at(startLine).at(startColumn + i) = '.';
				}
				usedWords.erase(it); //iterator is pointing to the element to be removed
				RemoveHashes(word, position);
				break;
			case 'V':
				for (int i = 0; i < word.length(); i++)
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
		cout << "\nThere is no word in the specified direction!\n";
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
		if ((column + word.length() - 1) < horizontalSize - 1)
			board.at(line).at(column + word.length()) = '#';
		break;
	case 'V':
		if (line > 0)
			board.at(line-1).at(column) = '#';
		if ((line + word.length() - 1) < verticalSize - 1)
			board.at(line + word.length()).at(column) = '#';
		break;
	default:
		cerr << "Invalid input!";
	}
}

//=================================================================================================================================
//

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
		if ((column + word.length() - 1) < horizontalSize - 1)
			if (board.at(line).at(column + word.length()) == '#')
				board.at(line).at(column + word.length()) = '.';
		break;
	case 'V':
		if (line > 0)
			if (board.at(line - 1).at(column) == '#')
				board.at(line - 1).at(column) = '.';
		if ((line + word.length() - 1) < verticalSize - 1)
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

	vector<string> fittingWords = dictionary->fittingWords(availableSpace);
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
					if (!dictionary->isInWordList(currentWord)) //if word does not exist
						valid = false;
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() >= 2) //only check if word size is bigger than 1
			if (!dictionary->isInWordList(currentWord)) //if word does not exist
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
					if (!dictionary->isInWordList(currentWord)) //if word does not exist
						valid = false;
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() >= 2) //only check if word size is bigger than 1
			if (!dictionary->isInWordList(currentWord)) //if word does not exist
				valid = false;
	}
		
	return valid;
}

//=================================================================================================================================

void Board::savePuzzle(string fileName)
{
	//TODO implement when certain that no other attributes will be added
	// Organize it well
	ofstream file(fileName, ios::binary);
	file.write((char *)&horizontalSize, sizeof(int));
	file.write((char *)&verticalSize, sizeof(int));

	for (int i = 0; i < verticalSize; i++) //Write all board chars
	{
		for (int j = 0; j < horizontalSize; j++)
		{
			file.write((char *)&board.at(i).at(j), sizeof(char));
		}
	}

	int vectorSize = usedWords.size();
	file.write((char *) &vectorSize, sizeof(int));
	vector<pair<string, string>>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
	{
		file.write((char *)&it->first, sizeof(string));
		file.write((char *)&it->second, sizeof(string));
	}
	file.close();
}

//=================================================================================================================================

bool Board::loadPuzzle(string fileName)
{
	ifstream file(fileName, ios::binary);
	
	if (!file.is_open())
		return;

	file.read((char *)&horizontalSize, sizeof(int));
	file.read((char *)&verticalSize, sizeof(int));

	board.resize(verticalSize);
	for (int i = 0; i < verticalSize; i++)
		board.at(i).resize(horizontalSize);

	for (int i = 0; i < verticalSize; i++) //Write all board chars
	{
		for (int j = 0; j < horizontalSize; j++)
		{
			file.read((char *) &board.at(i).at(j), sizeof(char));
		}
	}
	int vectorSize;
	file.read((char *)vectorSize, sizeof(int));
	usedWords.resize(vectorSize);

	for (int i = 0; i < vectorSize; i++)
	{
		file.read((char *)&usedWords.at(i).first, sizeof(string));
		file.read((char *)&usedWords.at(i).second, sizeof(string));
	}
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
	size_t wordSize = word.length();
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
		if ((targetCoords.second < wordCoords.second) || (targetCoords.second > wordCoords.second + word.length())) //if out of the range occupied by the word
			return false;
		break;
	case 'V':
		if (targetCoords.second != wordCoords.second) //if not on the same column, can not match
			return false;
		if ((targetCoords.first < wordCoords.first) || (targetCoords.first > wordCoords.first + word.length())) //if out of the range occupied by the word
			return false;
		break;
	default:
		cerr << "Invalid direction!";
	}
	return true;
}

//=================================================================================================================================

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