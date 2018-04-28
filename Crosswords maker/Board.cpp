#include "Board.h"
#include "Dictionary.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>

using namespace std;

/*Board::Board(int horizontalSize, int verticalSize)
{
	this->horizontalSize = horizontalSize;
	this->verticalSize = verticalSize;

	board.resize(verticalSize);
	for (size_t i = 0; i < board.size(); i++)
		board.at(i).resize(horizontalSize);

	for (size_t i = 0; i < board.size(); i++)
		for (size_t j = 0; j < board.at(i).size(); j++)
			board.at(i).at(j) = '.';
}*/

//=================================================================================================================================
// Constructor with sizes and dictionary to be used

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
	for (size_t i = 0; i < board.size(); i++)
	{
		for (size_t j = 0; j < board.at(i).size(); j++)
			cout << board.at(i).at(j);
		cout << '\n';
	}
}

//=================================================================================================================================
// Inserts a word on the board. If an invalid operation, detects and outputs what the problem was.

void Board::insertWord(string word, pair<int, int> insertionPos, char direction)
{
	// insertionPos = (line, column)

	if (!isValid(word)) // verify word is valid
	{
		cout << "Word is not valid!";
	}
	else if (!dictionary->isInWordList(word)) // verify word belongs to the dictionary
	{
		cout << "Word does not belong to the dictionary!";
	}
	else if (!wordFitsSpace(word, insertionPos, direction)) // verify it fits the space
	{
		cout << "Word does not fit specified space!";
	}
	else if (isWordUsed(word))	// verify if word was already used
	{
		cout << "Word is already in use!";
	}
	else if (!matchesCurrentBoard(word, insertionPos, direction))
	{
		cout << "Word does not match previous inserted words!";
	}
	else
	{
		usedWords.insert(word); //add word to the set

		//Insert word
		char dir = toupper(direction);
		int line = insertionPos.first;
		int column = insertionPos.second;
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
}

//=================================================================================================================================
// Shows the user what words he can put in the specified location

void Board::helpUser(pair<int, int> insertionPos, char direction)
{
	// insertionPos = (line, column)
	direction = toupper(direction);
	int availableSpace;
	switch (direction)
	{
	case 'H':
		availableSpace = horizontalSize - insertionPos.second;
		break;
	case 'V':
		availableSpace = verticalSize - insertionPos.first;
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
		if (!isWordUsed(currentWord) && matchesCurrentBoard(currentWord, insertionPos, direction))
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
	if (input.length() != 3)
		return false;
	
	char line = toupper(input.at(0));
	char column = toupper(input.at(1));
	char direction = toupper(input.at(2));
	char maxHorizontal = 'A' + horizontalSize - 1;
	char maxVertical = 'A' + verticalSize - 1;
	
	if (line < 'A' || line > maxVertical)
		return false;

	if (column < 'A' || column > maxHorizontal)
		return false;

	if (direction != 'H' && direction != 'V')
		return false;

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

bool Board::isValid(string word)
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
	set<string>::iterator it;
	for (it = usedWords.begin(); it != usedWords.end(); it++)
		if (*it == word)
			return true;
	return false;
}

//=================================================================================================================================
// Checks if the word fits in the specified space

bool Board::wordFitsSpace(string word, pair<int, int> insertionPos, char direction)
{
	// insertionPos = (line, column)

	char dir = toupper(direction);
	size_t wordSize = word.length();
	int availableSpace;
	switch (dir){
	case 'H':
		availableSpace = horizontalSize - insertionPos.second;
		break;
	case 'V':
		availableSpace = verticalSize - insertionPos.first;
		break;
	default:
		cerr << "Invalid input!";
	}
	return availableSpace >= wordSize;
}

//=================================================================================================================================
// Checks if the word matches the current board, i.e. the words already placed
// Assumes all other conditios are met: valid, not used and fits space

bool Board::matchesCurrentBoard(string word, pair<int, int> insertionPos, char direction)
{
	// insertionPos = (line, column)
	char dir = toupper(direction);
	int line = insertionPos.first;
	int column = insertionPos.second;
	switch (dir)
	{
	case 'H':
		for (size_t i = 0; i < word.length(); i++)
			if (board.at(line).at(column + i) != word.at(i) && board.at(line).at(column + i) != '.') // must either correspond to the word letter or to '.'
				return false;
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
// WildcardMatch
// str - Input string to match
// strWild - Match mask that may contain wildcards like ? and *
//
// A ? sign matches any character, except an empty string.
// A * sign matches any string inclusive an empty string.
// Characters are compared caseless.
//
// ADAPTED FROM:
// https://www.codeproject.com/Articles/188256/A-Simple-Wildcard-Matching-Function
bool Board::wildcardMatch(const char *str, const char *strWild)
{
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '?')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*')
		{
			// Need to do some tricks.
			// 1. The wildcard * is ignored.
			// So just an empty string matches. This is done by recursion.
			// Because we eat one character from the match string,
			// the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again,
			// with a wildcard * match. This is done by recursion.
			// Because we eat one character from the string,
			// the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}