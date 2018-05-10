/*
Starting point for the Crosswords Creator program.
Allows the user to start (or resume) the creation of a board which can be used in a crosswords game.
Built by the interaction between user, Board and Dictionary classes.
Has functions to deal with the UI, user interaction and the interaction between board and dictionary classes.
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#include "ColorMaster.h"
#include "Dictionary.h"
#include "Board.h"

using namespace std;

//TODO random mode
//TODO clear screen
//TODO random complete	


//TODO varrer tabuleiro e procurar palavras automaticamente formadas

//TODO indicate saved file name
//TODO Credits to me only
//TODO Clean up code
//TODO Clear all warnings

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

void Introduction();
void FullInstructions();
void PositionInstructions();
void WordInstructions();
void Options();

char YesNoQuestion(string question);

string askDictionaryName();
string askBoardNumber();
string determineBoardName();

pair<int, int> askBoardSize();

Dictionary CreateDictionary(bool &success);
Dictionary CreateDictionary(string dictName, bool &success);

Board CreateBoard();
Board ResumeBoard(string &dictName, bool &operationSuccess);

bool askToSaveBoard(Board &board, string dictName);
bool canBeInserted(Board &board, Dictionary &dictionary, string word, string positionInput);
bool isBoardValid(Board &board, Dictionary &dictionary);
bool isBoardValid(Board &board, Dictionary &dictionary, string word, string position);
bool testInsertion(Board &board, Dictionary &dictionary, string word, string positionInput);
bool randomInsertWord(Board &board, Dictionary &dictionary, string position);

void helpUser(Board &board, Dictionary &dictionary, string positionInput);
void EditBoard(Board &board, Dictionary &dict);

bool isValidInsertion(Board &board, Dictionary &dictionary, string word, string positionInput);
Board generateRandomBoard(Dictionary &dictionary, int insertionAttempts);

ColorMaster colorMaster;

//=================================================================================================================================

int main()
{
	srand(time_t(NULL));

	Introduction();
	cout << endl;

	char answer = YesNoQuestion("Display instructions (Y/N)? ");
	if (answer == 'Y')
		FullInstructions();

	while (true) //Program only ends by user input
	{
		//SELECT OPTION
		int answer;
		do
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}
			cout << endl;
			Options();
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "\nOption ? ";
			colorMaster.setcolor(DEFAULT);
			cin >> answer;
		} while (cin.fail() || (answer != 0 && answer != 1 && answer != 2 && answer != 3));

		cout << endl;
		Board board;
		Dictionary dictionary;
		switch (answer)
		{
		case 0:
			exit(0);
			break;
		case 1:
		{
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << " ====================================\n";
			cout << " |          CREATE PUZZLE           |\n";
			cout << " ====================================\n\n";
			colorMaster.setcolor(DEFAULT);

			bool success; //true if successfully created dictionary
			dictionary = CreateDictionary(success);
			if (!success)
			{
				colorMaster.setcolor(ERROR_MESSAGE);
				cout << "\nDictionary was not opened successfully.\n";
				colorMaster.setcolor(DEFAULT);
				break;
			}
			else
			{
				colorMaster.setcolor(SUCCESS);
				cout << "\nDictionary was opened successfully.\n";
				colorMaster.setcolor(DEFAULT);
			}

			cout << endl;
			board = CreateBoard();
			EditBoard(board, dictionary);

			break;
		}
		case 2:
		{
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << " ====================================\n";
			cout << " |          RESUME PUZZLE           |\n";
			cout << " ====================================\n";
			colorMaster.setcolor(DEFAULT);

			bool dictSuccess; //true if successfully created dictionary
			bool boardSuccess;
			string dictName;
			board = ResumeBoard(dictName, boardSuccess);

			if (!boardSuccess)
			{
				colorMaster.setcolor(ERROR_MESSAGE);
				cout << "\nBoard was not opened successfully.\n";
				colorMaster.setcolor(DEFAULT);
				break;
			}
			else
			{
				colorMaster.setcolor(SUCCESS);
				cout << "\nBoard was opened successfully.\n";
				colorMaster.setcolor(DEFAULT);
			}

			dictionary = CreateDictionary(dictName, dictSuccess);

			if (!dictSuccess)
			{
				colorMaster.setcolor(ERROR_MESSAGE);
				cout << "\nDictionary was not opened successfully.\n";
				colorMaster.setcolor(DEFAULT);
				break;
			}
			else
			{
				colorMaster.setcolor(SUCCESS);
				cout << "\nDictionary was opened successfully.\n";
				colorMaster.setcolor(DEFAULT);
			}

			EditBoard(board, dictionary);
			break;
		}
		case 3: 
		{
			const int NUMBER_INSERTION_ATTEMPTS = 50;

			colorMaster.setcolor(SYMBOL_COLOR);
			cout << " ====================================\n";
			cout << " |         RANDOM GENERATOR         |\n";
			cout << " ====================================\n\n";
			colorMaster.setcolor(DEFAULT);

			//Open dictionary
			bool success; //true if successfully created dictionary
			dictionary = CreateDictionary(success);
			if (!success)
			{
				colorMaster.setcolor(ERROR_MESSAGE);
				cout << "\nDictionary was not opened successfully.\n";
				colorMaster.setcolor(DEFAULT);
				break;
			}
			else
			{
				colorMaster.setcolor(SUCCESS);
				cout << "\nDictionary was opened successfully.\n";
				colorMaster.setcolor(DEFAULT);
			}
			cout << endl;

			//Generate board
			board = generateRandomBoard(dictionary, NUMBER_INSERTION_ATTEMPTS); //TODO more efficient algorithm

			//Show generated board
			cout << endl;
			board.showBoard();
			cout << endl;

			//Save board
			askToSaveBoard(board, dictionary.getName());
			break;
		}
		default:
			cerr << "Should not be able to get here!";
		}
	}
	return 0;
}

//=================================================================================================================================
// Prints and introduction for the program

void Introduction()
{
	cout << endl << " ";
	colorMaster.setcolor(BLACK,WHITE);
	cout << "  ____                                         _        ____                _             \n";
	colorMaster.setcolor(WHITE, BLACK); cout << " "; colorMaster.setcolor(BLACK, WHITE);
	cout << " / ___|_ __ ___  ___ _____      _____  _ __ __| |___   / ___|_ __ ___  __ _| |_ ___  _ __ \n";
	colorMaster.setcolor(WHITE, BLACK); cout << " "; colorMaster.setcolor(BLACK, WHITE);
	cout << "| |   | '__/ _ \\/ __/ __\\ \\ /\\ / / _ \\| '__/ _` / __| | |   | '__/ _ \\/ _` | __/ _ \\| '__|\n";
	colorMaster.setcolor(WHITE, BLACK); cout << " "; colorMaster.setcolor(BLACK, WHITE);
	cout << "| |___| | | (_) \\__ \\__ \\\\ V  V / (_) | | | (_| \\__ \\ | |___| | |  __/ (_| | || (_) | |   \n";
	colorMaster.setcolor(WHITE, BLACK); cout << " "; colorMaster.setcolor(BLACK, WHITE);
	cout << " \\____|_|  \\___/|___/___/ \\_/\\_/ \\___/|_|  \\__,_|___/  \\____|_|  \\___|\\__,_|\\__\\___/|_|   \n";
	colorMaster.setcolor(WHITE, BLACK);
}

//=================================================================================================================================
// Prints the program instructions

void FullInstructions()
{
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nINSTRUCTIONS\n\n";
	colorMaster.setcolor(WHITE, BLACK);

	cout << "Max size for each direction is 26.\n";

	colorMaster.setcolor(SYMBOL_COLOR); 
	cout << "Dots";
	colorMaster.setcolor(DEFAULT);
	cout << " (.) represent places where you ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "can";
	colorMaster.setcolor(DEFAULT);
	cout << " insert words\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Hashes";
	colorMaster.setcolor(DEFAULT);
	cout << " (#) represent places where you ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "can not";
	colorMaster.setcolor(DEFAULT);
	cout << " insert words.\n";

	colorMaster.setcolor(QUESTION_COLOR);
	cout << "\nPosition Question\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Input: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "LCD";
	colorMaster.setcolor(DEFAULT);
	cout << " (line, column, direction).\n";
	cout << "Can either be lower or uppercase, but ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "must respect this order!";
	colorMaster.setcolor(DEFAULT);
	cout << " Direction is 'H' or 'V'.\n";

	cout << "Other options: \n";
	cout << "- "; 	colorMaster.setcolor(SYMBOL_COLOR); 	cout << "I";	colorMaster.setcolor(DEFAULT);	cout << " to display simplified instructions for this question.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);		cout << "B";	colorMaster.setcolor(DEFAULT);	cout << " to display the current board.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);		cout << "R";	colorMaster.setcolor(DEFAULT);	cout << " to randomly complete the board.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);		cout << "CTRL-Z";	colorMaster.setcolor(DEFAULT);	cout << " to stop creating the board.\n";

	colorMaster.setcolor(QUESTION_COLOR);
	cout << "\nWord Question\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Input: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Word to be inserted in the board\n";
	colorMaster.setcolor(DEFAULT);

	cout << "Other options: \n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "I";	colorMaster.setcolor(DEFAULT);	cout << " to display simplified instructions for this question.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "B";	colorMaster.setcolor(DEFAULT);	cout << " to display the current board.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "-";	colorMaster.setcolor(DEFAULT);	cout << " to remove a previously placed word.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "?";	colorMaster.setcolor(DEFAULT);	cout << " for a list of words that can be placed starting on the specified position.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "R";	colorMaster.setcolor(DEFAULT);	cout << " to randomly insert a valid word from the dictionary.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "<";	colorMaster.setcolor(DEFAULT);	cout << " to return to the Position question.\n";

	cout << "\nWhen board creation is stopped by entering CTRL-Z, you will be prompted to ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "save the board";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n";

	cout << "If you choose to save the board, you will also be asked if the board is ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "finished";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n";
	cout << "If the board is finished, any ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "remaining cells will be filled with #";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n";

	colorMaster.setcolor(QUESTION_COLOR);
	cout << "\nPress any key to continue";
	colorMaster.setcolor(DEFAULT);
	_getch();
	cout << endl;
}

//=================================================================================================================================
// Prints simplified instructions for the Position? question

void PositionInstructions()
{
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nPOSITION INSTRUCTIONS\n\n";
	colorMaster.setcolor(WHITE, BLACK);

	cout << "Input: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "LCD";
	colorMaster.setcolor(DEFAULT);
	cout << " (line, column, direction).\n";
	cout << "Can either be lower or uppercase, but ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "must respect this order!";
	colorMaster.setcolor(DEFAULT);
	cout << " Direction is 'H' or 'V'.\n";

	cout << "Other options: \n";
	cout << "- "; 	colorMaster.setcolor(SYMBOL_COLOR); 	cout << "I";	colorMaster.setcolor(DEFAULT);	cout << " to display simplified instructions for this question.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);		cout << "B";	colorMaster.setcolor(DEFAULT);	cout << " to display the current board.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);		cout << "R";	colorMaster.setcolor(DEFAULT);	cout << " to randomly complete the board.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);		cout << "CTRL-Z";	colorMaster.setcolor(DEFAULT);	cout << " to stop creating the board.\n";
}

//=================================================================================================================================
// Prints simplified instructions for the Word? question

void WordInstructions()
{
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nWORD INSTRUCTIONS\n\n";
	colorMaster.setcolor(WHITE, BLACK);

	cout << "Input: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Word to be inserted in the board\n";
	colorMaster.setcolor(DEFAULT);

	cout << "Other options: \n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "I";	colorMaster.setcolor(DEFAULT);	cout << " to display simplified instructions for this question.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "B";	colorMaster.setcolor(DEFAULT);	cout << " to display the current board.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "-";	colorMaster.setcolor(DEFAULT);	cout << " to remove a previously placed word.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "?";	colorMaster.setcolor(DEFAULT);	cout << " for a list of words that can be placed starting on the specified position.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "R";	colorMaster.setcolor(DEFAULT);	cout << " to randomly insert a valid word from the dictionary.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "<";	colorMaster.setcolor(DEFAULT);	cout << " to return to the Position question.\n";
}

//=================================================================================================================================
// Shows the options

void Options()
{
	colorMaster.setcolor(BLACK, WHITE);
	cout << "OPTIONS\n";
	colorMaster.setcolor(WHITE, BLACK);

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "1";
	colorMaster.setcolor(DEFAULT);
	cout << " - Create board\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "2";
	colorMaster.setcolor(DEFAULT);
	cout << " - Resume board\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "3";
	colorMaster.setcolor(DEFAULT);
	cout << " - Randomly generate puzzle\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "0";
	colorMaster.setcolor(DEFAULT);
	cout << " - Exit\n";
}

//=================================================================================================================================
//Asks a Yes / No question and returns the answer

char YesNoQuestion(string question)
{
	char answer;
	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
		}
		colorMaster.setcolor(QUESTION_COLOR);
		cout << question;
		colorMaster.setcolor(DEFAULT);
		cin >> answer;
		answer = toupper(answer);
	} while (answer != 'Y' && answer != 'N');
	return answer;
}

//=================================================================================================================================
// Asks for the name of the dictionary

string askDictionaryName()
{
	string dictName;
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Dictionary file name? ";
	colorMaster.setcolor(DEFAULT);
	cin >> dictName;
	return dictName;
}

//=================================================================================================================================
// Asks for the number of the board

string askBoardNumber()
{
	string boardNumber;
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Board number? ";
	colorMaster.setcolor(DEFAULT);
	cin >> boardNumber;
	return boardNumber;
}

//=================================================================================================================================
// Asks for the size of the board

pair<int,int> askBoardSize()
{
	pair<int, int> boardSize;

	do {
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		colorMaster.setcolor(QUESTION_COLOR);
		cout << "Horizontal board size? ";	
		colorMaster.setcolor(DEFAULT);
		cin >> boardSize.first;
	} while (!cin || boardSize.first <= 0 || boardSize.first > 26); //board size is limited

	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		colorMaster.setcolor(QUESTION_COLOR);
		cout << "Vertical board size? ";
		colorMaster.setcolor(DEFAULT);
		cin >> boardSize.second;
	} while (!cin || boardSize.second <= 0 || boardSize.second > 26);

	return boardSize;
}

//=================================================================================================================================
// Asks if the user wishes to save the current board. Boolean to indicate operation success or not

bool askToSaveBoard(Board &board, string dictName)
{
	bool success = true;
	char answer = YesNoQuestion("Save the current board (Y/N) ? ");

	if (answer == 'Y')
	{
		//Check if board is finished or not
		char answer2 = YesNoQuestion("Autocomplete missing cells (Y/N) ? ");
		if (answer2 == 'Y')
			board.fillRemainingSpots();

		//Save file
		string fileName = determineBoardName();
		success = board.saveBoard(fileName, dictName);
		if (success)
		{
			colorMaster.setcolor(SUCCESS);
			cout << "\nBoard was saved successfully as " << fileName << ".\n";
			colorMaster.setcolor(DEFAULT);
		}
		else
		{
			colorMaster.setcolor(ERROR_MESSAGE);
			cout << "\nThe final board is not valid.\n";
			colorMaster.setcolor(DEFAULT);
		}
	}
	return success;
}

//=================================================================================================================================
// Returns the name of the board to be saved, based on already existing files

string determineBoardName()
{
	string boardName = "";
	int counter = 1;

	while (boardName == "")
	{
		string tempName;
		if (counter < 10)
			tempName = "B00" + to_string(counter) + ".txt";
		else if (counter < 100)
			tempName = "B0" + to_string(counter) + ".txt";
		else if (counter < 1000)
			tempName = "B" + to_string(counter) + ".txt";
		else
			tempName = "BOverflow.txt";

		ifstream file(tempName); //opens files until it finds one that does not exist
		if (!file.is_open())
			boardName = tempName;
		counter++;
	}
	return boardName;
}

//=================================================================================================================================
// Creates a new dictionary, asking the user the name. Returns it and changes the argument boolean to indicate success or not

Dictionary CreateDictionary(bool &success)
{
	string dictName = askDictionaryName();
	Dictionary dictionary(dictName);
	bool dictionaryOpened = dictionary.ProcessDictionary();
	if (!dictionaryOpened)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nCould not locate file with that name.\n";
		colorMaster.setcolor(DEFAULT);
		success = false;
		return Dictionary();
	}
	success = true;
	return dictionary;
}

//=================================================================================================================================
// Creates a new dictionary with specified name. Returns it and changes the argument boolean to indicate success or not

Dictionary CreateDictionary(string dictName, bool &success)
{
	Dictionary dictionary(dictName);
	bool dictionaryOpened = dictionary.ProcessDictionary();
	if (!dictionaryOpened)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nCould not locate dictionary with that name.\n";
		colorMaster.setcolor(DEFAULT);
		success = false;
		return Dictionary();
	}
	success = true;
	return dictionary;
}

//=================================================================================================================================
// Creates a new board from scratch

Board CreateBoard()
{
	pair<int, int> boardSize = askBoardSize();
	Board board(boardSize.first, boardSize.second);
	return board;
}

//=================================================================================================================================
// Resumes an already existing board from a file

Board ResumeBoard(string &dictionaryName, bool &operationSuccess)
{
	cout << endl;
	string boardNumber = askBoardNumber();
	Board board;
	string dictName;
	bool boardLoaded = board.loadBoard(boardNumber, dictName);

	if (!boardLoaded)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nCould not locate board file.\n";
		colorMaster.setcolor(DEFAULT);
		operationSuccess = false;
		return Board();
	}

	operationSuccess = true;
	dictionaryName = dictName;
	return board;
}

//=================================================================================================================================
// Verifies if a word can be inserted in a determined location, informing why not if false

bool canBeInserted(Board &board, Dictionary &dictionary, string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = board.calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);

	if (board.hasHash(insertionPosition)) // Verify it the position has an hash
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nYou can not place a word in that location.\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (!dictionary.isValidHeadline(word)) // Verify word is a valid headline
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is not valid! Please only use characters from 'A' to 'Z' or the ones specified in the instructions.\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (!dictionary.isInWordList(word)) // Verify word belongs to the dictionary
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is not present in the dictionary!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (!board.wordFitsSpace(word, positionInput)) // Verify it fits the space
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord does not fit the specified space!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (board.isWordUsed(word)) // Verify if word was already used
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is already in use!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	else if (!board.matchesInterceptedPositions(word, positionInput) || !testInsertion(board, dictionary, word, positionInput)) // Verify if the insertion can be executed while keeping the board valid
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord does not match current board!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	return true;
}

//=================================================================================================================================
// Verifies if all words in the board in both directions are valid according to the dictionary

bool isBoardValid(Board &board, Dictionary &dictionary)
{
	bool valid = true;

	//HORIZONTAL
	for (int line = 0; line < board.getVerticalSize(); line++)
	{
		string currentWord = "";
		for (int column = 0; column < board.getHorizontalSize(); column++)
		{
			if (isalpha(board.getCell(line, column)))
			{
				currentWord += board.getCell(line, column);
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
	for (int column = 0; column < board.getHorizontalSize(); column++)
	{
		string currentWord = "";
		for (int line = 0; line < board.getVerticalSize(); line++)
		{
			if (isalpha(board.getCell(line, column)))
			{
				currentWord += board.getCell(line, column);
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
// Verifies if all words in the board in both directions are valid according to the dictionary
// However, is limited to the positions the word occupies

bool isBoardValid(Board &board, Dictionary &dictionary, string word, string position)
{
	pair<int, int> coords = board.calculateInsertionCoordinates(position);
	char dir = position.at(2);
	int start, end;

	string currentWord;
	switch (dir)
	{
	case 'H':
		start = coords.second;
		end = start + (int)word.length() - 1;

		//VERTICAL
		for (int column = start; column <= end; column++)
		{
			currentWord = "";
			for (int line = 0; line < board.getVerticalSize(); line++)
			{
				if (isalpha(board.getCell(line, column)))
				{
					currentWord += board.getCell(line, column);
				}
				else
				{
					if (currentWord.length() >= 2) //only check if word size is bigger than 1
						if (!dictionary.isInWordList(currentWord)) //if word does not exist
							return false;
					currentWord = ""; //reset word
				}
			}
			if (currentWord.length() >= 2) //only check if word size is bigger than 1
				if (!dictionary.isInWordList(currentWord)) //if word does not exist
					return false;
		}

		//HORIZONTAL
		currentWord = "";
		for (int column = 0; column < board.getHorizontalSize(); column++)
		{
			if (isalpha(board.getCell(coords.first, column)))
			{
				currentWord += board.getCell(coords.first, column);
			}
			else
			{
				if (currentWord.length() >= 2) //only check if word size is bigger than 1
					if (!dictionary.isInWordList(currentWord)) //if word does not exist
						return false;
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() >= 2) //only check if word size is bigger than 1
			if (!dictionary.isInWordList(currentWord)) //if word does not exist
				return false;
		break;

	case 'V':
		//HORIZONTAL
		start = coords.first;
		end = start + (int)word.length() - 1;

		for (int line = start; line <= end; line++)
		{
			currentWord = "";
			for (int column = 0; column < board.getHorizontalSize(); column++)
			{
				if (isalpha(board.getCell(line, column)))
				{
					currentWord += board.getCell(line, column);
				}
				else
				{
					if (currentWord.length() >= 2) //only check if word size is bigger than 1
						if (!dictionary.isInWordList(currentWord)) //if word does not exist
							return false;
					currentWord = ""; //reset word
				}
			}
			if (currentWord.length() >= 2) //only check if word size is bigger than 1
				if (!dictionary.isInWordList(currentWord)) //if word does not exist
					return false;
		}

		//VERTICAL
		currentWord = "";
		for (int line = 0; line < board.getVerticalSize(); line++)
		{
			if (isalpha(board.getCell(line, coords.second)))
			{
				currentWord += board.getCell(line, coords.second);
			}
			else
			{
				if (currentWord.length() >= 2) //only check if word size is bigger than 1
					if (!dictionary.isInWordList(currentWord)) //if word does not exist
						return false;
				currentWord = ""; //reset word
			}
		}

		if (currentWord.length() >= 2) //only check if word size is bigger than 1
			if (!dictionary.isInWordList(currentWord)) //if word does not exist
				return false;
		break;
	}
	return true;
}

//=================================================================================================================================
// Simulates an insertion and verifies if the resulting board is valid according to the dictionary

bool testInsertion(Board &board, Dictionary &dictionary, string word, string positionInput)
{
	//Backup
	std::vector<std::vector<char>> oldboard = board.getBoard();
	std::vector<std::pair<std::string, std::string>> oldUsedWords = board.getUsedWords();

	//Insert word a verify if the board is valid
	board.insertWord(word, positionInput);
	bool valid = isBoardValid(board, dictionary, word, positionInput);

	//Restore backup
	board.setBoard(oldboard);
	board.setUsedWords(oldUsedWords);

	return valid;
}

//=================================================================================================================================
// Helps the user by showing which words can be placed on specified location

void helpUser(Board &board, Dictionary &dictionary, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = board.calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));

	//Calculate available space
	int availableSpace;
	switch (direction)
	{
	case 'H':
		availableSpace = board.getHorizontalSize() - insertionPosition.second;
		break;
	case 'V':
		availableSpace = board.getVerticalSize() - insertionPosition.first;
		break;
	default:
		cerr << "Invalid input!";
	}

	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nWords that fit there:\n";
	colorMaster.setcolor(WHITE, BLACK);

	//Gets the words that may fit there from the dictionary
	vector<string> fittingWords = dictionary.fittingWords(availableSpace);

	//Displays the words that are valid insertions in the board
	int counter = 0;
	const int WORDS_PER_LINE = 6;
	const int WORDS_WIDTH = 18;
	for (size_t i = 0; i < fittingWords.size(); i++)
	{
		string currentWord = fittingWords.at(i);
		if (!board.isWordUsed(currentWord) && board.matchesInterceptedPositions(currentWord, positionInput) && testInsertion(board, dictionary, currentWord, positionInput))
		{
			if (counter % WORDS_PER_LINE == 0) cout << endl;
			cout << setw(WORDS_WIDTH) << currentWord;
			counter++;
		}
	}
}

//=================================================================================================================================
// Randomly inserts a valid word from the dictionary in the specified position.

bool randomInsertWord(Board &board, Dictionary &dictionary, string position)
{
	pair<int, int> coords = board.calculateInsertionCoordinates(position);
	int line = coords.first;
	int column = coords.second;
	char direction = position.at(2);

	//Calculate available space
	int availableSpace;
	switch (direction)
	{
	case 'H':
		availableSpace = board.getHorizontalSize() - column;
		break;
	case 'V':
		availableSpace = board.getVerticalSize() - line;
		break;
	default:
		cerr << "Invalid input!";
	}

	//Gets the words that fit the space
	vector<string> fittingWords = dictionary.fittingWords(availableSpace);

	//Filters the words that may actually be inserted
	vector<string> validWords;
	for (int j = 0; j < fittingWords.size(); j++)
	{
		if (isValidInsertion(board, dictionary, fittingWords.at(j), position)) //Checks if word can be inserted
			validWords.push_back(fittingWords.at(j));
	}

	if (validWords.size() == 0) //if no words can be inserted, skip iteration
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nNo word in the dictionary can be inserted there.\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	//Perform insertion
	int wordIndex = rand() % validWords.size();
	board.insertWord(validWords.at(wordIndex), position);
	board.insertWordHashes(validWords.at(wordIndex), position);
	return true;
}

//=================================================================================================================================
// Randomly completes a board by inserting random words in random positions. Does not guarantee full board!

bool randomCompleteBoard(Board &board, Dictionary &dictionary, int insertionAttempts)
{
	cout << "\nInserting random words...\n";

	for (int i = 0; i < insertionAttempts; i++)
	{
		//Generate random position
		int line = rand() % board.getVerticalSize();
		int column = rand() % board.getHorizontalSize();
		int dir = rand() % 2;
		char direction = (dir == 0 ? 'H' : 'V');
		char c_position[] = { (char) 'A' + line , (char) 'A' + column, direction, '\0' };
		string position(c_position);

		//Calculate available space
		int availableSpace;
		switch (direction)
		{
		case 'H':
			availableSpace = board.getHorizontalSize() - column;
			break;
		case 'V':
			availableSpace = board.getVerticalSize() - line;
			break;
		default:
			cerr << "Invalid input!";
		}

		//Gets the words that fit the space
		vector<string> fittingWords = dictionary.fittingWords(availableSpace);

		//Filters the words that may actually be inserted
		vector<string> validWords;
		for (int j = 0; j < fittingWords.size(); j++)
		{
			if (isValidInsertion(board, dictionary, fittingWords.at(j), position)) //Checks if word can be inserted
				validWords.push_back(fittingWords.at(j));
		}

		if (validWords.size() == 0) //if no words can be inserted, skip iteration
			continue;

		//Perform insertion
		int wordIndex = rand() % validWords.size();
		board.insertWord(validWords.at(wordIndex), position);
		board.insertWordHashes(validWords.at(wordIndex), position);
	}
}

//=================================================================================================================================
// Allows to make changes to an existing board.

void EditBoard(Board &board, Dictionary &dict)
{
	cout << endl;
	board.showBoard();
	cout << endl;

	// Input loop
	bool stopCreating = false;
	while (!stopCreating)
	{
		string positionInput, word;
		bool skipLoop = false;

		////////////////////////////////
		//      ASK FOR POSITION      //
		////////////////////////////////

		bool validPositionInput = false;
		do
		{
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "Board position? ";
			colorMaster.setcolor(DEFAULT);
			cin >> positionInput;

			if (cin.fail())
			{
				if (cin.eof())
				{
					stopCreating = true;
					cin.clear(); // Clears cin so as to enable next inputs
					break; //exits this loop
				}
				else
				{
					cin.clear();
					cin.ignore(10000, '\n');
				}
			}
			else //if good input
			{
				//Convert to uppercase
				for (size_t i = 0; i < positionInput.length(); i++)
					positionInput.at(i) = toupper(positionInput.at(i));

				if (positionInput == "I")
				{
					PositionInstructions();
					cout << endl;
				}
				else if (positionInput == "B")
				{
					cout << endl;
					board.showBoard();
					cout << endl;
				}
				else if (positionInput == "R")
				{
					cout << endl;
					board.showBoard();
					cout << endl;
				}
				//Check validity
				else if (board.validPositionInput(positionInput))
				{
					pair<int, int> coordinates = board.calculateInsertionCoordinates(positionInput);
					if (board.getCell(coordinates.first, coordinates.second) == '#')
					{
						colorMaster.setcolor(ERROR_MESSAGE);
						cout << "\nYou cannot insert any word there.\n\n";
						colorMaster.setcolor(DEFAULT);
					}
					else
					{
						validPositionInput = true;
					}
				}
			}
		} while (!validPositionInput); //loop until valid input

		if (stopCreating) //exit loop if CTRL-Z
		{
			if (isBoardValid(board, dict)) //FINAL CHECK
			{
				bool successfulSave = askToSaveBoard(board, dict.getName());
				if (!successfulSave) //if there was a problem saving board, continue with the loop
					continue;
				break; //if successful save of the board, end loop
			}
			else
			{
				colorMaster.setcolor(ERROR_MESSAGE);
				cout << "\nBoard has invalid words and cannot be saved.\n\n";
				colorMaster.setcolor(DEFAULT);
				break;
			}
		}

		////////////////////////////////
		//        ASK FOR WORD        //
		////////////////////////////////

		bool validInput = false;
		do
		{
			if (skipLoop)
				break;

			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "Word to insert? ";
			colorMaster.setcolor(DEFAULT);
			cin >> word;

			//Convert to uppercase
			for (size_t i = 0; i < word.length(); i++)
				word.at(i) = toupper(word.at(i));

			if (word == "<") // Skip loop
			{
				validInput = true; // Exit loop
				cout << endl;
			}
			else if (word == "-") // Remove word
			{
				bool wordRemoved = board.removeWord(positionInput);
				if (wordRemoved)
					validInput = true; // exit loop
				cout << endl;
			}
			else if (word == "?") // Ask for help
			{
				helpUser(board, dict, positionInput);
				cout << endl;
			}
			else if (word == "I") // Ask for instructions
			{
				WordInstructions();
				cout << endl;
			}
			else if (word == "B") // Show board
			{
				cout << endl;
				board.showBoard();
				cout << endl;
			}
			else if (word == "R") // Show board
			{
				bool wordInserted = randomInsertWord(board, dict, positionInput);
				if (wordInserted)
					validInput = true;
				cout << endl;
			}
			else // normal word insertion
				if (canBeInserted(board, dict, word, positionInput)) //Check validity and output error messages if necessary
				{
					board.insertWord(word, positionInput);
					board.insertWordHashes(word, positionInput);
					validInput = true;
				}
		} while (!validInput); //loop until valid input

		cout << endl;
		board.showBoard();
		cout << endl;
	}
}

//=================================================================================================================================
//														RANDOM GENERATOR
//=================================================================================================================================

//=================================================================================================================================
// Verifies if a word can be inserted in a determined location, returning a bool. No messages are displayed.

bool isValidInsertion(Board &board, Dictionary &dictionary, string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = board.calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);

	if (board.hasHash(insertionPosition)) // Verify it the position has an hash
		return false;
	else if (board.isWordUsed(word)) // Verify if word was already used
		return false;
	else if (!board.matchesInterceptedPositions(word, positionInput) || !testInsertion(board, dictionary, word, positionInput)) // Verify if the insertion can be executed while keeping the board valid
		return false;
	return true;
}

//=================================================================================================================================
// Asks for the dimensions of a board and randomly fills it, returning it

Board generateRandomBoard(Dictionary &dictionary, int insertionAttempts)
{
	pair<int, int> boardSize = askBoardSize();
	Board board(boardSize.first, boardSize.second);

	board.clearBoard();

	cout << "\nGenerating random puzzle...\n";

	for (int i = 0; i < insertionAttempts; i++)
	{
		//Generate random position
		int line = rand() % board.getVerticalSize();
		int column = rand() % board.getHorizontalSize();
		int dir = rand() % 2;
		char direction = (dir == 0 ? 'H' : 'V');
		char c_position[] = { (char) 'A' + line , (char) 'A' + column, direction, '\0' };
		string position(c_position);

		//Calculate available space
		int availableSpace;
		switch (direction)
		{
		case 'H':
			availableSpace = board.getHorizontalSize() - column;
			break;
		case 'V':
			availableSpace = board.getVerticalSize() - line;
			break;
		default:
			cerr << "Invalid input!";
		}

		//Gets the words that fit the space
		vector<string> fittingWords = dictionary.fittingWords(availableSpace); 

		//Filters the words that may actually be inserted
		vector<string> validWords;
		for (int j = 0; j < fittingWords.size(); j++)
		{
			if (isValidInsertion(board, dictionary, fittingWords.at(j), position)) //Checks if word can be inserted
				validWords.push_back(fittingWords.at(j));
		}

		if (validWords.size() == 0) //if no words can be inserted, skip iteration
			continue;

		//Perform insertion
		int wordIndex = rand() % validWords.size();
		board.insertWord(validWords.at(wordIndex), position);
		board.insertWordHashes(validWords.at(wordIndex), position);
	}
	return board;
}
