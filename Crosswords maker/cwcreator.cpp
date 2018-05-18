/*
Starting point for the Crosswords Creator program.
Allows the user to start (or resume) the creation of a board which can be used in a crosswords game.
Built by the interaction between user, Board and Dictionary classes.
Has functions to deal with the UI, user interaction and the interaction between board and dictionary classes.

AUTHOR: GambuzX
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

//TODO test another randomizer. Maybe dont need to fill all spots

//TODO Think about connecting programs in a single one

//TODO Credits to me only
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

//Enumeration of possible modes to edit the board
enum EditMode
{
	strict, //Does many tests before allowing insertion, not allowing the board to ever be invalid while editing
	trustUser, //Gives the user more freedom and only checks full validity in the end
	limitless, //Gives the user even more freedom. Does not insert or remove hashes, user has all the power! -> Not implemented
	invalid //Invalid edit mode
};

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
EditMode askEditMode();

Dictionary CreateDictionary(bool &success);
Dictionary CreateDictionary(string dictName, bool &success);

Board CreateBoard();
Board ResumeBoard(string &dictName, bool &operationSuccess);

bool askToSaveBoard(Board &board, Dictionary &dict);
bool canBeInserted(Board &board, Dictionary &dictionary, string word, string positionInput, EditMode editmode);
bool isBoardValid(Board &board, Dictionary &dictionary);
bool isBoardValid(Board &board, Dictionary &dictionary, string word, string position);
bool hasTwoLetterWordsRepeated(Board &board, Dictionary &dictionary);
bool testInsertion(Board &board, Dictionary &dictionary, string word, string positionInput);
bool testRemoval(Board &board, Dictionary &dictionary, string positionInput);
bool randomInsertWord(Board &board, Dictionary &dictionary, string position);

vector <pair<string, string>> listAllBoardWords(Board &board, Dictionary &dictionary, vector<pair<string, string>> &invWords);

pair<string, string> newRiddle();

void helpUser(Board &board, Dictionary &dictionary, string positionInput, EditMode editMode);
void randomCompleteBoard(Board &board, Dictionary &dictionary, int insertionAttempts);
void bruteForceInsertion(Board &board, Dictionary &dictionary, bool complete, int verticalStep, int horizontalStep);
void EditBoard(Board &board, Dictionary &dict, EditMode editMode);

bool wordBelongsToUsedWords(vector<pair<string, string>> usedWords, string word);
bool wordRepeatedInDifferentPosition(vector<pair<string, string>> usedWords, string word, string position);
bool isValidInsertion(Board &board, Dictionary &dictionary, string word, string positionInput);
bool isValidInsertionPlus(Board &board, Dictionary &dictionary, string word, string positionInput);
Board generateRandomBoard(Dictionary &dictionary);

ColorMaster colorMaster;

//=================================================================================================================================
// Entry point for the program

int main()
{
	srand((unsigned int) time(NULL));

	Introduction();
	cout << endl;

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
		} while (cin.fail() || (answer != 0 && answer != 1 && answer != 2 && answer != 3 && answer != 9));

		//INTERPRET OPTION
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

			//OPEN DICTIONARY
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

			//CREATE BOARD AND EDIT IT
			board = CreateBoard();
			EditMode editMode = askEditMode();
			EditBoard(board, dictionary, editMode);

			break;
		}
		case 2:
		{
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << " ====================================\n";
			cout << " |          RESUME PUZZLE           |\n";
			cout << " ====================================\n";
			colorMaster.setcolor(DEFAULT);

			//OPEN BOARD FILE
			bool dictSuccess; //true if successfully created dictionary
			bool boardSuccess;
			string dictName;
			board = ResumeBoard(dictName, boardSuccess); //Returns the resumed board and changes argument variables to the values in the opened file
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

			//OPEN DICTIONARY
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

			//EDIT BOARD
			EditMode editMode = askEditMode();
			EditBoard(board, dictionary, editMode);
			break;
		}
		case 3: 
		{
			const int NUMBER_INSERTION_ATTEMPTS = 12;

			colorMaster.setcolor(SYMBOL_COLOR);
			cout << " ====================================\n";
			cout << " |         RANDOM GENERATOR         |\n";
			cout << " ====================================\n\n";
			colorMaster.setcolor(DEFAULT);

			//OPEN DICTIONARY
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

			//GENERATE BOARD
			//board = generateRandomBoard(dictionary); //Adds some words to the board
			board = CreateBoard(); //Start with an empty board
			bruteForceInsertion(board, dictionary, false, 1,1); //Fill the missing board places
			EditMode editMode = askEditMode();
			EditBoard(board, dictionary, editMode); //Allows editing
			break;
		}
		case 9:
		{
			FullInstructions();
			break; 
		}
		default:
			cerr << "Should not be able to get here!";
		}
	}
	return 0;
}

//=================================================================================================================================
// Prints a introduction logo for the program

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
// Prints the full program instructions and specifications

void FullInstructions()
{
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nINSTRUCTIONS\n\n";
	colorMaster.setcolor(WHITE, BLACK);

	//OPTIONS INFORMATION
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Options Information\n";
	colorMaster.setcolor(DEFAULT);

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "1 - Create new board.\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Allows you to create a new board starting from scratch and edit it.\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "2 - Resume existing board.\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Loads an already existing file containing a board generated by this program and allows to continue editing it.\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "3 - Randomly generate puzzle.\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Creates a new puzzle and performs some random word insertions, not completely filling it.\n";
	cout << "You will have the option to perform a more thorough word insertion.\nNote that depending on the board size this operation may take a while.\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "9 - Instructions.\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Displays full program instructions.\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "0 - Exit\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Exits the program.\n";

	//EDIT MODES
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "\nEdit Modes\n";
	colorMaster.setcolor(DEFAULT);
	cout << "When editing the board, you will have 2 options, ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "strict";
	colorMaster.setcolor(DEFAULT);
	cout << " and ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "free";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Strict";
	colorMaster.setcolor(DEFAULT);
	cout << " mode does not allow insertions that would invalidate the board. The board is always valid, even while editing.\n";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Free";
	colorMaster.setcolor(DEFAULT);
	cout << " mode allows any insertion respecting the positions occupied. A full validity verification is only made in the end.\n";
	
	//BOARD SPECIFICATIONS
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "\nBoard Specifications\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Max board size in each direction is 26.\n";
	colorMaster.setcolor(SYMBOL_COLOR); 
	cout << "Dots";
	colorMaster.setcolor(DEFAULT);
	cout << " (.) represent places where you ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "can";
	colorMaster.setcolor(DEFAULT);
	cout << " insert words.\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Hashes";
	colorMaster.setcolor(DEFAULT);
	cout << " (#) represent places where you ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "can not";
	colorMaster.setcolor(DEFAULT);
	cout << " insert words.\n";

	//POSITION QUESTION
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

	//WORD QUESTION
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
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "-";	colorMaster.setcolor(DEFAULT);	cout << " to remove a previously placed word (or hash if in free mode).\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "?";	colorMaster.setcolor(DEFAULT);	cout << " for a list of words that can be placed starting on the specified position.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "+";	colorMaster.setcolor(DEFAULT);	cout << " to verify if an automatically formed word is valid.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "#";	colorMaster.setcolor(DEFAULT);	cout << " to insert an hash.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "R";	colorMaster.setcolor(DEFAULT);	cout << " to randomly insert a valid word from the dictionary.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "<";	colorMaster.setcolor(DEFAULT);	cout << " to return to the Position question.\n";

	//SAVING BOARD
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "\nSaving Board\n";
	colorMaster.setcolor(DEFAULT);
	cout << "When board creation is stopped by entering CTRL-Z, if the board is valid you will be prompted to ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "save the board";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n";

	cout << "If you choose to save the board and indicate the board is ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "finished";
	colorMaster.setcolor(DEFAULT);
	cout << ", any ";
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
	cout << "- "; 	colorMaster.setcolor(SYMBOL_COLOR); 	cout << "I";	colorMaster.setcolor(DEFAULT);	cout << " to display these instructions.\n";
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
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "I";	colorMaster.setcolor(DEFAULT);	cout << " to display these instructions.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "B";	colorMaster.setcolor(DEFAULT);	cout << " to display the current board.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "-";	colorMaster.setcolor(DEFAULT);	cout << " to remove a previously placed word (or hash if in free mode).\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "?";	colorMaster.setcolor(DEFAULT);	cout << " for a list of words that can be placed starting on the specified position.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "+";	colorMaster.setcolor(DEFAULT);	cout << " to verify if an automatically formed word is valid.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "#";	colorMaster.setcolor(DEFAULT);	cout << " to insert an hash.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "R";	colorMaster.setcolor(DEFAULT);	cout << " to randomly insert a valid word from the dictionary.\n";
	cout << "- ";	colorMaster.setcolor(SYMBOL_COLOR);	cout << "<";	colorMaster.setcolor(DEFAULT);	cout << " to return to the Position question.\n";
}

//=================================================================================================================================
// Shows the available options

void Options()
{
	colorMaster.setcolor(BLACK, WHITE);
	cout << "OPTIONS\n";
	colorMaster.setcolor(WHITE, BLACK);

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "1";
	colorMaster.setcolor(DEFAULT);
	cout << " - Create new board.\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "2";
	colorMaster.setcolor(DEFAULT);
	cout << " - Resume existing board.\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "3";
	colorMaster.setcolor(DEFAULT);
	cout << " - Randomly generate puzzle.\n";

	cout << endl;

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "9";
	colorMaster.setcolor(DEFAULT);
	cout << " - Instructions.\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "0";
	colorMaster.setcolor(DEFAULT);
	cout << " - Exit\n";
}

//=================================================================================================================================
//Asks a Yes / No question and returns the answer (Y/N)

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
// Asks for the size of the board, returning a pair (horizontal, vertical)

pair<int,int> askBoardSize()
{
	pair<int, int> boardSize;

	//HORIZONTAL
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

	//VERTICAL
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

//====================================================================================================.=============================
// Asks if the user wishes to save the current board. Returns boolean to indicate operation success or not

bool askToSaveBoard(Board &board, Dictionary &dict)
{
	char answer = YesNoQuestion("Save the current board (Y/N) ? ");
	if (answer == 'Y')
	{
		//Check if board is finished or not
		char answer2 = YesNoQuestion("Autocomplete missing cells (Y/N) ? ");
		if (answer2 == 'Y')
			board.fillRemainingSpots();

		//Performs a search for all the words in board, also indicating which ones are wrongly placed
		vector<pair<string, string>> invalidWords;
		vector<pair<string, string>> boardWords = listAllBoardWords(board, dict, invalidWords);

		if (invalidWords.size() == 0)
		{
			//Save file
			string fileName = determineBoardName();
			board.saveBoard(fileName, dict.getName(), boardWords);
			colorMaster.setcolor(SUCCESS);
			cout << "\nBoard was saved successfully as " << fileName << ".\n";
			colorMaster.setcolor(DEFAULT);
		}
		else
		{
			//Display error message and erroneous words
			colorMaster.setcolor(ERROR_MESSAGE);
			cout << "\nThe final board is not valid. The following words are repeated:";
			for (size_t i = 0; i < invalidWords.size(); i++)
				cout << endl << invalidWords.at(i).first << " - " << invalidWords.at(i).second;
			colorMaster.setcolor(DEFAULT);
			cout << endl;
			return false;
		}
	}
	return true;
}

//=================================================================================================================================
// Asks the user in what mode the board should be edited.

EditMode askEditMode()
{
	EditMode editMode;
	int answer;
	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
		}
		colorMaster.setcolor(QUESTION_COLOR);
		cout << "\nDo you wish to edit the board in Strict (0) or Free (1) mode? ";
		colorMaster.setcolor(DEFAULT);
		cin >> answer;
	} while (answer != 0 && answer != 1);
	
	switch (answer)
	{
	case 0:
		editMode = EditMode::strict;
		break;
	case 1:
		editMode = EditMode::trustUser;
		break;
	default:
		editMode = EditMode::invalid;
	}
	return editMode;
}

//=================================================================================================================================
// Returns the name of the board to be saved, based on already existing files.

string determineBoardName()
{
	string boardName = "";
	int counter = 1; //will be incremented for each board file that already exists

	while (boardName == "")
	{
		//assign correct file name
		string tempName;
		if (counter < 10)
			tempName = "b00" + to_string(counter) + ".txt";
		else if (counter < 100)
			tempName = "b0" + to_string(counter) + ".txt";
		else if (counter < 1000)
			tempName = "b" + to_string(counter) + ".txt";
		else
			tempName = "bOverflow.txt";

		ifstream file(tempName); //opens files until it finds one that does not exist
		if (!file.is_open())
			boardName = tempName;
		counter++;
	}
	return boardName;
}

//=================================================================================================================================
// Creates a new dictionary, asking the user the name. Returns it and changes the parameter boolean to indicate success or not.

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
// Creates a new dictionary with specified name. Returns it and changes the argument boolean to indicate success or not.

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
// Creates a new board from scratch.

Board CreateBoard()
{
	pair<int, int> boardSize = askBoardSize();
	Board board(boardSize.first, boardSize.second);
	return board;
}

//=================================================================================================================================
// Resumes an already existing board from a file. Changes parameter values.

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
// Verifies if a word can be inserted in a determined location, informing why not if false.
// Last test varies depending on edit mode.

bool canBeInserted(Board &board, Dictionary &dictionary, string word, string positionInput, EditMode editMode)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = board.calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);

	// Verify it the position has an hash
	if (board.hasHash(insertionPosition))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nYou can not place a word in that location.\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify word is a valid headline
	else if (!dictionary.isValidHeadline(word))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is not valid! Please only use characters from 'A' to 'Z' or the ones specified in the instructions.\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify word belongs to the dictionary
	else if (!dictionary.isInWordList(word))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is not present in the dictionary!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify it fits the space
	else if (!board.wordFitsSpace(word, positionInput))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord does not fit the specified space!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify if word was already used
	else if (board.isWordUsed(word))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is already in use!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	// Verify if word would be inserted on top of another
	else if (board.isOnTopOfWord(word, positionInput))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nYou can not insert a word on top of another!\n\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	//Last test depends on edit mode
	if (editMode == EditMode::strict)
	{
		// Verify if the insertion can be executed while keeping the board valid
		if (!board.matchesInterceptedPositions(word, positionInput) || !testInsertion(board, dictionary, word, positionInput))
		{
			colorMaster.setcolor(ERROR_MESSAGE);
			cout << "\nWord does not match current board!\n\n";
			colorMaster.setcolor(DEFAULT);
			return false;
		}
	}
	else if (editMode == EditMode::trustUser)
	{
		// Verify if the insertion can be executed while keeping the board valid
		if (!board.matchesInterceptedPositions(word, positionInput))
		{
			colorMaster.setcolor(ERROR_MESSAGE);
			cout << "\nWord does not match current board!\n\n";
			colorMaster.setcolor(DEFAULT);
			return false;
		}
	}

	return true;
}

//=================================================================================================================================
// Verifies if a given position on a given direction has a new word that was formed automatically, with the option of inserting it if valid.

bool checkAndAddAutoFormedWord(Board &board, Dictionary &dictionary, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = board.calculateInsertionCoordinates(positionInput);
	int line = insertionPosition.first;
	int column = insertionPosition.second;
	char direction = positionInput.at(2);

	//Check if cell is an alphabetic character
	if (!isalpha(board.getCell(line, column)))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nThere is no word in that location.\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	//Verify if any already existing word intercepts that position
	else if (board.existsWordInterceptingPosition(insertionPosition, direction))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nThere already exists a word in that position and direction.\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}
	
	//Get the word in that position and direction
	string newWord = "";
	switch (direction)
	{
	case 'H':
	{
		int currentColumn = column;
		do
		{
			newWord += board.getCell(line, currentColumn);
			currentColumn++;
		} while (currentColumn < board.getHorizontalSize() && isalpha(board.getCell(line, currentColumn)));
		break;
	}
	case 'V':
	{
		int currentLine = line;
		do
		{
			newWord += board.getCell(currentLine, column);
			currentLine++;
		} while (currentLine < board.getVerticalSize() && isalpha(board.getCell(currentLine, column)));
		break;
	}
	}

	//Verify if word is in the dictionary
	if (!dictionary.isInWordList(newWord))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is not present in the dictionary!\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	//Verify if word was already used
	else if (board.isWordUsed(newWord))
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nWord is already in use!\n";
		colorMaster.setcolor(DEFAULT);
		return false;
	}

	//If it reaches here, word is new and valid
	colorMaster.setcolor(SUCCESS);
	cout << "\nWord is valid for insertion.\n\n";
	colorMaster.setcolor(DEFAULT);

	//Ask for insertion
	char answer = YesNoQuestion("Insert word (Y/N)? ");
	if (answer == 'Y')
	{
		//Remove previous words in occupied positions and direction
		vector<pair<string, string>> usedWords = board.getUsedWords();
		switch (direction)
		{
		case 'H':
		{
			for (size_t j = column; j < column + newWord.length(); j++)
				for (size_t g = 0; g < usedWords.size(); g++)
					if (board.givenWordInterceptsPosition(pair<int, int>(line, (int) j), direction, usedWords.at(g).second, usedWords.at(g).first))
					{
						board.removeWordFromUsedWords((int) g);
						break;
					}
			break;
		}
		case 'V':
		{
			for (size_t j = line; j < line + newWord.length(); j++)
				for (size_t g = 0; g < usedWords.size(); g++)
					if (board.givenWordInterceptsPosition(pair<int, int>((int) j, column), direction, usedWords.at(g).second, usedWords.at(g).first))
					{
						board.removeWordFromUsedWords((int) g);
						usedWords.erase(usedWords.begin() + g);
						break;
					}
			break;
		}
		}

		//Insert word and hashes
		board.insertWord(newWord, positionInput);
		board.insertWordHashes(newWord, positionInput);
		cout << "\nWord was inserted. Previous words on occupied positions were removed.\n";
	}
	else
		cout << "\nWord was not inserted.\n";
	return true;
}

//=================================================================================================================================
// Verifies if all words in the board in both directions are valid according to the dictionary and if there are no duplicates.
//
// The idea is as follows: - While we keep finding consecutive letters, concatenate them in a string.
//						   - When a character that is not a letter is found, verify if previously formed string is valid.
//						   - Repeat for all lines and columns in both directions.

bool isBoardValid(Board &board, Dictionary &dictionary)
{
	vector<string> foundWords;

	//HORIZONTAL
	for (int line = 0; line < board.getVerticalSize(); line++)
	{
		string currentWord = ""; //start with an empty string
		for (int column = 0; column < board.getHorizontalSize(); column++)
		{
			if (isalpha(board.getCell(line, column)))
			{
				currentWord += board.getCell(line, column); //concatenate consecutive letters
			}
			else
			{
				if (currentWord.length() >= 2) //only check if word size is bigger than 1
					if (!dictionary.isInWordList(currentWord) || find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end()) //if word does not exist or is already in the foundWords vector
						return false;
					else // if a valid word
						foundWords.push_back(currentWord);
				currentWord = ""; //reset word and keep searching
			}
		}
		//Last cell verification
		if (currentWord.length() >= 2)
			if (!dictionary.isInWordList(currentWord) || find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end())
				return false;
			else
				foundWords.push_back(currentWord);
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
				if (currentWord.length() >= 2)
					if (!dictionary.isInWordList(currentWord) || find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end())
						return false;
					else
						foundWords.push_back(currentWord);
				currentWord = "";
			}
		}
		if (currentWord.length() >= 2)
			if (!dictionary.isInWordList(currentWord) || find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end())
				return false;
			else
				foundWords.push_back(currentWord);
	}

	return true;
}

//=================================================================================================================================
// Verifies if all words in the board in both directions are valid according to the dictionary and if there are no duplicates.
// However, is limited to the lines and columns the word to be inserted intercepts.
//
// The idea is as follows: - While we keep finding consecutive letters, concatenate them in a string.
//						   - When a character that is not a letter is found, verify if previously formed string is valid.
//						   - Repeat for all lines and columns that the word to be inserted intercepts.

bool isBoardValid(Board &board, Dictionary &dictionary, string word, string position)
{
	pair<int, int> coords = board.calculateInsertionCoordinates(position);
	char dir = position.at(2);
	int start, end;

	vector<pair<string, string>> usedWords = board.getUsedWords();
	string currentWord;
	switch (dir)
	{
	case 'H':
		start = coords.second;
		end = start + (int)word.length() - 1;

		//VERTICAL
		for (int column = start; column <= end; column++)
		{
			currentWord = ""; //start with an empty string
			for (int line = 0; line < board.getVerticalSize(); line++)
			{
				if (isalpha(board.getCell(line, column)))
				{
					currentWord += board.getCell(line, column); //concatenate consecutive letters
				}
				else
				{
					if (currentWord.length() >= 2) //only check if word size is bigger than 1
					{
						char c_position[] = { 'A' + (char)(line - currentWord.length()), 'A' + (char)column, 'V', '\0' };
						string position(c_position);

						if (!dictionary.isInWordList(currentWord)) //if word does not exist
							return false;

						//Check for duplicates in different positions
						for (size_t i = 0; i < usedWords.size(); i++)
							if (usedWords.at(i).second == currentWord && usedWords.at(i).first != position)
								return false;
					}
					currentWord = ""; //reset word
				}
			}
			if (currentWord.length() >= 2) //only check if word size is bigger than 1
			{
				char c_position[] = { 'A' + (char)(board.getVerticalSize() - currentWord.length()), 'A' + (char)column, 'V', '\0' };
				string position(c_position);

				if (!dictionary.isInWordList(currentWord)) //if word does not exist
					return false;

				//Check for duplicates in different positions
				for (size_t i = 0; i < usedWords.size(); i++)
					if (usedWords.at(i).second == currentWord && usedWords.at(i).first != position)
						return false;
			}
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
				{
					char c_position[] = { 'A' + (char)coords.first, 'A' + (char)(column - currentWord.length()), 'H', '\0' };
					string position(c_position);

					if (!dictionary.isInWordList(currentWord)) //if word does not exist
						return false;

					//Check for duplicates in different positions
					for (size_t i = 0; i < usedWords.size(); i++)
						if (usedWords.at(i).second == currentWord && usedWords.at(i).first != position)
							return false;
				}
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() >= 2) //only check if word size is bigger than 1
		{
			char c_position[] = { 'A' + (char)coords.first, 'A' + (char)(board.getHorizontalSize() - currentWord.length()), 'H', '\0' };
			string position(c_position);

			if (!dictionary.isInWordList(currentWord)) //if word does not exist
				return false;

			//Check for duplicates in different positions
			for (size_t i = 0; i < usedWords.size(); i++)
				if (usedWords.at(i).second == currentWord && usedWords.at(i).first != position)
					return false;
		}
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
					{
						char c_position[] = { 'A' + (char)line, 'A' + (char)(column - currentWord.length()), 'H', '\0' };
						string position(c_position);

						if (!dictionary.isInWordList(currentWord)) //if word does not exist
							return false;

						//Check for duplicates in different positions
						for (size_t i = 0; i < usedWords.size(); i++)
							if (usedWords.at(i).second == currentWord && usedWords.at(i).first != position)
								return false;
					}
					currentWord = ""; //reset word
				}
			}
			if (currentWord.length() >= 2) //only check if word size is bigger than 1
			{
				char c_position[] = { 'A' + (char)line, 'A' + (char)(board.getHorizontalSize() - currentWord.length()), 'H', '\0' };
				string position(c_position);

				if (!dictionary.isInWordList(currentWord)) //if word does not exist
					return false;

				//Check for duplicates in different positions
				for (size_t i = 0; i < usedWords.size(); i++)
					if (usedWords.at(i).second == currentWord && usedWords.at(i).first != position)
						return false;
			}
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
				{
					char c_position[] = { 'A' + (char)(line - currentWord.length()), 'A' + (char)coords.second, 'V', '\0' };
					string position(c_position);

					if (!dictionary.isInWordList(currentWord)) //if word does not exist
						return false;

					//Check for duplicates in different positions
					for (size_t i = 0; i < usedWords.size(); i++)
						if (usedWords.at(i).second == currentWord && usedWords.at(i).first != position)
							return false;
				}
				currentWord = ""; //reset word
			}
		}

		if (currentWord.length() >= 2) //only check if word size is bigger than 1
		{
			char c_position[] = { 'A' + (char)(board.getVerticalSize() - currentWord.length()), 'A' + (char)coords.second, 'V', '\0' };
			string position(c_position);

			if (!dictionary.isInWordList(currentWord)) //if word does not exist
				return false;

			//Check for duplicates in different positions
			for (size_t i = 0; i < usedWords.size(); i++)
				if (usedWords.at(i).second == currentWord && usedWords.at(i).first != position)
					return false;
		}
		break;
	}
	return true;
}

//=================================================================================================================================
// Verifies if there are 2 letter words repeated in the board. Assumes words found are valid.

bool hasTwoLetterWordsRepeated(Board &board, Dictionary &dictionary)
{
	vector<string> foundWords;

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
				if (currentWord.length() == 2) //only check if word size is 2
					if (find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end())
						return true;
					else
						foundWords.push_back(currentWord);
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() == 2) //only check if word size is 2
			if (find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end())
				return true;
			else
				foundWords.push_back(currentWord);
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
				if (currentWord.length() == 2) //only check if word size is 2
					if (find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end())
						return true;
					else
						foundWords.push_back(currentWord);
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() == 2) //only check if word size is 2
			if (find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end())
				return true;
			else
				foundWords.push_back(currentWord);
	}

	return false;
}

//=================================================================================================================================
// Simulates an insertion and verifies if the resulting board is valid according to the dictionary.

bool testInsertion(Board &board, Dictionary &dictionary, string word, string positionInput)
{
	//Backup
	std::vector<std::vector<char>> oldboard = board.getBoard();
	std::vector<std::pair<std::string, std::string>> oldUsedWords = board.getUsedWords();

	//Insert word and verify if the board is valid
	board.insertWord(word, positionInput);
	bool test1 = isBoardValid(board, dictionary, word, positionInput);
	bool test2 = !hasTwoLetterWordsRepeated(board, dictionary);

	//Restore backup
	board.setBoard(oldboard);
	board.setUsedWords(oldUsedWords);

	return test1 && test2; //Only returns true if both tests are valid
}

//=================================================================================================================================
// Simulates a word removal and verifies if the resulting board is valid according to the dictionary.

bool testRemoval(Board &board, Dictionary &dictionary, string positionInput)
{
	//Backup
	std::vector<std::vector<char>> oldboard = board.getBoard();
	std::vector<std::pair<std::string, std::string>> oldUsedWords = board.getUsedWords();

	//Remove word and verify if the board is valid
	board.removeWord(positionInput);
	bool valid = isBoardValid(board, dictionary);

	//Restore backup
	board.setBoard(oldboard);
	board.setUsedWords(oldUsedWords);

	return valid;
}

//=================================================================================================================================
// Helps the user by showing which words can be placed on specified location. Behaviour changes with edit mode.
// EditMode = strict -> Has everything in account. Words shown are will keep the board valid.
// EditMode = trustUser -> Only has in account the occupied cells and not adjacent ones.

void helpUser(Board &board, Dictionary &dictionary, string positionInput, EditMode editMode)
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
	//Behaviour varies depending on edit mode
	switch (editMode)
	{
	case EditMode::strict:
	{
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
		break;
	}
	case EditMode::trustUser:
	{
		int counter = 0;
		const int WORDS_PER_LINE = 6;
		const int WORDS_WIDTH = 18;
		for (size_t i = 0; i < fittingWords.size(); i++)
		{
			string currentWord = fittingWords.at(i);
			if (!board.isWordUsed(currentWord) && board.matchesInterceptedPositions(currentWord, positionInput))
			{
				if (counter % WORDS_PER_LINE == 0) cout << endl;
				cout << setw(WORDS_WIDTH) << currentWord;
				counter++;
			}
		}
		break;
	}
	}
}

//=================================================================================================================================
// Lists all the words in the board and returns a vector with them. Also signals which words are invalid, i.e., repeated.

vector <pair<string, string>> listAllBoardWords(Board &board, Dictionary &dictionary, vector<pair<string, string>> &invWords)
{
	vector<pair<string, string>> boardWords;
	vector<pair<string, string>> invalidWords;

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
				{
					char pos[] = { 'A' + (char)line, 'A' + (char)(column - currentWord.length()), 'H', '\0' };
					string position(pos);
					if (!wordBelongsToUsedWords(boardWords, currentWord)) //if word is not used
					{
						boardWords.push_back(pair<string, string>(position, currentWord));
					}
					else if (wordRepeatedInDifferentPosition(boardWords, currentWord, position)) //if it is used, but in a different position -> invalid
					{
						invalidWords.push_back(pair<string, string>(position, currentWord));
					}
				}
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() >= 2)
		{
			char pos[] = { 'A' + (char)line, 'A' + (char)(board.getHorizontalSize() - currentWord.length()) , 'H', '\0' };
			string position(pos);
			if (!wordBelongsToUsedWords(boardWords, currentWord))
			{
				boardWords.push_back(pair<string, string>(position, currentWord));
			}
			else if (wordRepeatedInDifferentPosition(boardWords, currentWord, position))
			{
				invalidWords.push_back(pair<string, string>(position, currentWord));
			}
		}
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
				{
					char pos[] = { 'A' + (char)(line - currentWord.length()), 'A' + (char)column, 'V', '\0' };
					string position(pos);
					if (!wordBelongsToUsedWords(boardWords, currentWord))
					{
						boardWords.push_back(pair<string, string>(position, currentWord));
					}
					else if (wordRepeatedInDifferentPosition(boardWords, currentWord, position))
					{
						invalidWords.push_back(pair<string, string>(position, currentWord));
					}
				}
				currentWord = ""; //reset word
			}
		}
		if (currentWord.length() >= 2) //only check if word size is bigger than 1
		{
			char pos[] = { 'A' + (char)(board.getVerticalSize() - currentWord.length()), 'A' + (char)column, 'V', '\0' };
			string position(pos);
			if (!wordBelongsToUsedWords(boardWords, currentWord))
			{
				boardWords.push_back(pair<string, string>(position, currentWord));
			}
			else if (wordRepeatedInDifferentPosition(boardWords, currentWord, position))
			{
				invalidWords.push_back(pair<string, string>(position, currentWord));
			}
		}
	}
	invWords = invalidWords;
	return boardWords;
}

//=================================================================================================================================
// Indicates if a word belongs to a vector of pairs of strings (position, word).

bool wordBelongsToUsedWords(vector<pair<string, string>> usedWords, string word)
{
	for (size_t i = 0; i < usedWords.size(); i++)
		if (usedWords.at(i).second == word)
			return true;
	return false;
}

//=================================================================================================================================
// Indicates if a word is repeated in a vector, i.e., if it appears more than once in different positions.

bool wordRepeatedInDifferentPosition(vector<pair<string, string>> usedWords, string word, string position)
{
	for (size_t i = 0; i < usedWords.size(); i++)
		if (usedWords.at(i).first != position && usedWords.at(i).second == word)
			return true;
	return false;
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
	for (size_t j = 0; j < fittingWords.size(); j++)
	{
		if (isValidInsertion(board, dictionary, fittingWords.at(j), position)) //Checks if word can be inserted
			validWords.push_back(fittingWords.at(j));
	}

	//If no words can be inserted, skip insertion
	if (validWords.size() == 0)
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

void randomCompleteBoard(Board &board, Dictionary &dictionary, int insertionAttempts) //TODO check this
{
	//Verifies if the board is already complete or not. 
	bool hasDot = false;
	for (int i = 0; i < board.getVerticalSize(); i++)
		for (int j = 0; j < board.getHorizontalSize(); j++)
			if (board.getCell(i, j) == '.')
				hasDot = true;

	//If board complete, leave.
	if (!hasDot)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nThe board is already complete.\n";
		colorMaster.setcolor(DEFAULT);
		return ;
	}

	cout << "\nInserting random words...\n";

	vector<string> headlines = dictionary.getHeadlines();
	for (int i = 0; i < insertionAttempts; i++)
	{
		//Generate random position
		int line = rand() % board.getVerticalSize();
		int column = rand() % board.getHorizontalSize();
		int dir = rand() % 2;
		char direction = (dir == 0 ? 'H' : 'V');
		char c_position[] = { 'A' + (char) line , 'A' + (char) column, direction, '\0' };
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

		if (availableSpace <= 2) //skip if limited space. Does not insert 2 letter words to avoid invalid boards.
			continue;

		//Try to insert some random words to increase efficiency
		const int RANDOM_TRIES = 12;
		bool insertedWord = false;
		for (int j = 0; j < RANDOM_TRIES; j++)
		{
			int randomN = rand() % headlines.size();

			if (isValidInsertionPlus(board, dictionary, headlines.at(randomN), position))
			{
				board.insertWord(headlines.at(randomN), position);
				board.insertWordHashes(headlines.at(randomN), position);
				insertedWord = true;
				break;
			}
		}
		if (insertedWord) //if successfully inserted a random word, go to next iteration
			continue;

		//Gets the words that fit the space
		vector<string> fittingWords = dictionary.fittingWords(availableSpace);

		//Filters the words that may actually be inserted
		vector<string> validWords;
		for (size_t j = 0; j < fittingWords.size(); j++)
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
// Brute force word insertion. Goes through the board, advancing cells according to the step parameters until the end.
// Parameter 'complete' decides between speed vs completeness. If complete = true, will take longer, but board will be full.
//
// To reduce time consumption, only inserts if position has a dot and, after inserting, skips one column.
//
// Also tries to insert a predetermined number of random words before going throught the entire dictionary.

void bruteForceInsertion(Board &board, Dictionary &dictionary, bool complete, int verticalStep = 1, int horizontalStep = 1)
{
	int intervalBetweenDots = (complete ? 1 : 10);
	int currentColor = 1; //Colors go from 1 to 15, excluding black
	int counter = 1;
	bool riddle = true;
	pair<string, string> currentRiddle;
	vector<string> headlines = dictionary.getHeadlines(); //all words from wordList
	for (int line = 0; line < board.getVerticalSize(); line+= verticalStep) //Go through the board cells
	{
		for (int column = 0; column < board.getHorizontalSize(); column+= horizontalStep) //Skip 2 columns at a time
		{
			//UI DISPLAY
			if (counter % intervalBetweenDots == 0)
			{
				colorMaster.setcolor(currentColor);
				cout << " . ";
				colorMaster.setcolor(DEFAULT);
				currentColor++;
				if (currentColor >= 15)
				{
					currentColor = 1;
					cout << endl;
					if (riddle)
					{
						currentRiddle = newRiddle();
						colorMaster.setcolor(QUESTION_COLOR);
						cout << currentRiddle.first;
						colorMaster.setcolor(DEFAULT);
						riddle = !riddle;
					}
					else
					{
						colorMaster.setcolor(SYMBOL_COLOR);
						cout << currentRiddle.second;
						colorMaster.setcolor(DEFAULT);
						riddle = !riddle;
					}
					cout << endl;
				}
			}
			counter++;

			//WORD INSERTION
			if (board.getCell(line, column) == '.')
			{
				//========================
				//    First direction   //
				//========================

				int dir = rand() % 2;
				char direction = (dir == 0 ? 'H' : 'V');
				char c_position[] = { 'A' + (char)line , 'A' + (char)column, direction, '\0' };
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

				if (availableSpace > 2) //does not insert 2 letter words to avoid repeated letters
				{
					//Gets the words that fit the space
					vector<string> fittingWords = dictionary.fittingWords(availableSpace);

					const int DECISION_BOUNDARY = 50; //TODO test values
					if (fittingWords.size() > DECISION_BOUNDARY)
					{
						//Try to insert some random words to increase efficiency
						const int RANDOM_TRIES = 12;
						bool insertedWord = false;
						for (int j = 0; j < RANDOM_TRIES; j++)
						{
							int randomN = rand() % fittingWords.size();

							if (isValidInsertionPlus(board, dictionary, fittingWords.at(randomN), position))
							{
								board.insertWord(fittingWords.at(randomN), position);
								board.insertWordHashes(fittingWords.at(randomN), position);
								insertedWord = true;
								break;
							}
						}
						if (insertedWord && complete) //if successfully inserted a random word, go to next iteration
							continue;
					}

					if (complete) //Only if user really wants a full board. Will take longer
					{
						//Filters the words that may actually be inserted
						vector<string> validWords;
						for (size_t j = 0; j < fittingWords.size(); j++)
						{
							if (isValidInsertion(board, dictionary, fittingWords.at(j), position)) //Checks if word can be inserted
								validWords.push_back(fittingWords.at(j));
						}

						if (validWords.size() != 0) //if no words can be inserted, skip iteration
						{
							//Perform insertion
							int wordIndex = rand() % validWords.size();
							board.insertWord(validWords.at(wordIndex), position);
							board.insertWordHashes(validWords.at(wordIndex), position);
						}
					}
				}

				//========================
				//   Second direction   //
				//========================

				direction = (direction == 'V'? 'H' : 'V');
				char c_position2[] = { 'A' + (char)line , 'A' + (char)column, direction, '\0' };
				position = string(c_position2);

				//Calculate available space
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

				if (availableSpace <= 2)
					continue;

				//Gets the words that fit the space
				vector<string> fittingWords = dictionary.fittingWords(availableSpace);

				const int DECISION_BOUNDARY = 50; //TODO test values
				if (fittingWords.size() > DECISION_BOUNDARY)
				{
					//Try to insert some random words to increase efficiency
					const int RANDOM_TRIES = 12;
					bool insertedWord = false;
					for (int j = 0; j < RANDOM_TRIES; j++)
					{
						int randomN = rand() % fittingWords.size();

						if (isValidInsertionPlus(board, dictionary, fittingWords.at(randomN), position))
						{
							board.insertWord(fittingWords.at(randomN), position);
							board.insertWordHashes(fittingWords.at(randomN), position);
							insertedWord = true;
							break;
						}
					}
					if (insertedWord) //if successfully inserted a random word, go to next iteration
						continue;
				}

				if (complete) //Only if user really wants a full board. Will take longer
				{
					//Filters the words that may actually be inserted
					vector<string> validWords;
					for (size_t j = 0; j < fittingWords.size(); j++)
					{
						if (isValidInsertion(board, dictionary, fittingWords.at(j), position)) //Checks if word can be inserted
							validWords.push_back(fittingWords.at(j));
					}

					if (validWords.size() != 0) //if no words can be inserted, skip iteration
					{
						//Perform insertion
						int wordIndex = rand() % validWords.size();
						board.insertWord(validWords.at(wordIndex), position);
						board.insertWordHashes(validWords.at(wordIndex), position);
					}
				}
			}
		}
	}
}

//=================================================================================================================================
// Returns a random pair constituted by riddle and answer. 

pair<string, string> newRiddle()
{
	vector<pair<string, string>> riddleList = { 
	{"What kind of room has no doors or windows ?", "A mushroom."}, 
	{"What kind of tree can you carry in your hand?", "A palm."}, 
	{"Which word in the dictionary is spelled incorrectly?", "Incorrectly"},
	{"How many of each species did Moses take on the ark with him?", "None, Moses wasn't on the ark Noah was."},
	{"Imagine you are in a dark room. How do you get out?", "Stop imagining."},
	{"What invention lets you look right through a wall?", "The window."},
	{"What is at the end of a rainbow?", "The letter W."},
	{"The eight of us go forth not back to protect our king from a foes attack. Who are we?", "Chesspawns."},
	{"Why can't a man living in the USA be buried in Canada?", "Why should a living man be buried?"},
	{"Why do Chinese men eat more rice than Japanese men do?", "There are more Chinese men than Japanese man."},
	{"When will water stop running down hill?", "When it reaches the bottom."},
	{"I have keys but no locks, I have a space but no room, You can enter, but can't go outside. What am I?", "A keyboard."},
	{"Which month has 28 days?", "All of them, of course."},
	{"What is so delicate that saying its name breaks it?", "Silence."},
	{"What is next in this sequence? JFMAMJJASON . . .", "The letter \"D\". The sequence contains the first letter of each month."},
	{"Suproliglicatiouspenuvaliancia - how do you spell it?", "I T (it)."},
	{"What is full of holes, but can still hold a lot of water?", "A sponge."},
	{"What belongs to you but others use it more than you do?", "Your name."},
	{"People buy me to eat, but never eat me. What am I?", "A plate."},
	{"Poor people have it. Rich people need it. If you eat it you die. What is it?", "Nothing."},
	{"What begins with T ends with T and has T in it?", "A teapot."},
	{"What word begins and ends with an E but only has one letter?", "Envelope."},
	{"What is something you will never see again?", "Yesterday :'("},
	{"What are two things you cannot have for breakfast?", "Lunch and dinner."},
	{"Which football player wears the biggest helmet?", "The one with the biggest head."},
	{"Why couldn't the sailors play cards?", "The captain was standing on the deck."},
	{"What do you call a man who does not have all his fingers on one hand?", "Normal – You have fingers on both hands!"}};

	return riddleList.at(rand() % riddleList.size()); //return random pair
}


//=================================================================================================================================
// Allows to make changes to an existing board.

void EditBoard(Board &board, Dictionary &dict, EditMode editMode)
{
	if (editMode == EditMode::invalid)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nInvalid edit mode.\n\n";
		colorMaster.setcolor(DEFAULT);
		return;
	}

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

				//OPTIONS
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
					const int NUMBER_OF_INSERTION_ATTEMPTS = 12;
					randomCompleteBoard(board, dict, NUMBER_OF_INSERTION_ATTEMPTS);
					bruteForceInsertion(board, dict, true);
					validPositionInput = true; //leave loop
					skipLoop = true;
				}
				else if (board.validPositionInput(positionInput)) //Check validity					
				{
					pair<int, int> coordinates = board.calculateInsertionCoordinates(positionInput);
					if (board.getCell(coordinates.first, coordinates.second) == '#' && editMode == EditMode::strict)
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

		if (stopCreating) //if CTRL-Z
		{
			if (isBoardValid(board, dict)) //FINAL CHECK
			{
				bool successfulSave = askToSaveBoard(board, dict);
				if (!successfulSave) //if there was a problem saving board, continue with the loop
				{
					skipLoop = true;
					stopCreating = false;
				} 
				else 
					break; //if successful save of the board, end loop
			}
			else
			{
				colorMaster.setcolor(ERROR_MESSAGE);
				cout << "\nBoard has invalid words and cannot be saved.\n\n";
				colorMaster.setcolor(DEFAULT);
				skipLoop = true;

				char answer = YesNoQuestion("Leave (Y/N)? ");
				if (answer == 'N')
					stopCreating = false; //continue editing
			}
		}

		////////////////////////////////
		//        ASK FOR WORD        //
		////////////////////////////////

		bool validInput = false;
		do
		{
			if (skipLoop) //skip to next iteration
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

			//OPTIONS
			if (word == "<") //Return to previous question
			{
				validInput = true;
				cout << endl;
			}
			else if (word == "-") // Remove word
			{
				switch (editMode)
				{
				case EditMode::strict:
					board.removeWord(positionInput);
					validInput = true; // exit loop
					cout << endl;
					break;
				case EditMode::trustUser:
					board.removeWordOrHash(positionInput);
					validInput = true; // exit loop
					cout << endl;
					break;
				}
			}
			else if (word == "?") // Ask for help
			{
				helpUser(board, dict, positionInput, editMode); //varies depending on edit mode
				cout << endl;
			}
			else if (word == "+") // Checks for automatically formed words
			{
				bool valid = checkAndAddAutoFormedWord(board, dict, positionInput);
				if (valid)
					validInput = true;
				else //if not valid, do not end loop
					cout << endl;
			}
			else if (word == "#") // Insert an hash
			{
				board.insertHash(positionInput);
				validInput = true;
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
			else if (word == "R") // Remove word
			{
				bool wordInserted = randomInsertWord(board, dict, positionInput);
				if (wordInserted)
					validInput = true;
				cout << endl;
			}
			else // normal word insertion
			{
				if (canBeInserted(board, dict, word, positionInput, editMode)) //has in account which edit mode is beeing used
				{
					board.insertWord(word, positionInput);
					board.insertWordHashes(word, positionInput);
					validInput = true;
				}
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

	if (board.hasHash(insertionPosition))
		return false;
	else if (board.isWordUsed(word))
		return false;
	else if (board.isOnTopOfWord(word, positionInput)) // Verify if word would be inserted on top of another
		return false;
	else if (!board.matchesInterceptedPositions(word, positionInput) || !testInsertion(board, dictionary, word, positionInput)) // Verify if the insertion can be executed while keeping the board valid
		return false;
	return true;
}

//=================================================================================================================================
// Verifies if a word can be inserted in a determined location, with one more test than above, returning a bool. No messages are displayed.

bool isValidInsertionPlus(Board &board, Dictionary &dictionary, string word, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = board.calculateInsertionCoordinates(positionInput);
	char direction = positionInput.at(2);

	if (board.hasHash(insertionPosition))
		return false;
	else if (!board.wordFitsSpace(word, positionInput))
		return false;
	else if (board.isWordUsed(word))
		return false;
	else if (board.isOnTopOfWord(word, positionInput)) // Verify if word would be inserted on top of another
		return false;
	else if (!board.matchesInterceptedPositions(word, positionInput) || !testInsertion(board, dictionary, word, positionInput)) // Verify if the insertion can be executed while keeping the board valid
		return false;
	return true;
}

//=================================================================================================================================
// Asks for the dimensions of a board and randomly fills it with some words, returning it. Does not complete it.

Board generateRandomBoard(Dictionary &dictionary)
{
	pair<int, int> boardSize = askBoardSize();
	Board board(boardSize.first, boardSize.second);
	int insertionAttempts = 12;

	board.clearBoard(); //assure board is empty

	cout << "\nGenerating random puzzle...\n";

	vector<string> headlines = dictionary.getHeadlines();
	for (int i = 0; i < insertionAttempts; i++)
	{
		//Generate random position
		int line = rand() % board.getVerticalSize();
		int column = rand() % board.getHorizontalSize();
		int dir = rand() % 2;

		//========================
		//    First direction   //
		//========================

		char direction = (dir == 0 ? 'H' : 'V');
		char c_position[] = {'A' + (char) line , 'A' + (char) column, direction, '\0' };
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

		//Try to insert some random words to increase efficiency
		const int RANDOM_TRIES = 12;
		bool insertedWord = false;
		for (int j = 0; j < RANDOM_TRIES; j++)
		{
			int randomN = rand() % headlines.size();
			
			if (isValidInsertionPlus(board, dictionary, headlines.at(randomN), position))
			{
				board.insertWord(headlines.at(randomN), position);
				board.insertWordHashes(headlines.at(randomN), position);
				insertedWord = true;
				break;
			}
		}
		if (!insertedWord) //if successfully inserted a random word, go to next iteration
		{
			//Gets the words that fit the space
			vector<string> fittingWords = dictionary.fittingWords(availableSpace);

			//Filters the words that may actually be inserted
			vector<string> validWords;
			for (size_t j = 0; j < fittingWords.size(); j++)
			{
				if (isValidInsertion(board, dictionary, fittingWords.at(j), position)) //Checks if word can be inserted
					validWords.push_back(fittingWords.at(j));
			}

			//Only attempt insertion if there are words for it
			if (validWords.size() != 0)
			{
				//Perform insertion
				int wordIndex = rand() % validWords.size();
				board.insertWord(validWords.at(wordIndex), position);
				board.insertWordHashes(validWords.at(wordIndex), position);
			}
		}

		//========================
		//   Second direction   //
		//========================

		direction = (direction == 'V' ? 'H' : 'V');
		char c_position2[] = { 'A' + (char)line , 'A' + (char)column, direction, '\0' };
		position = string(c_position2);

		//Calculate available space
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

		//Try to insert some random words to increase efficiency
		insertedWord = false;
		for (int j = 0; j < RANDOM_TRIES; j++)
		{
			int randomN = rand() % headlines.size();

			if (isValidInsertionPlus(board, dictionary, headlines.at(randomN), position))
			{
				board.insertWord(headlines.at(randomN), position);
				board.insertWordHashes(headlines.at(randomN), position);
				insertedWord = true;
				break;
			}
		}
		if (!insertedWord) //if successfully inserted a random word, go to next iteration
		{
			//Gets the words that fit the space
			vector<string> fittingWords = dictionary.fittingWords(availableSpace);

			//Filters the words that may actually be inserted
			vector<string> validWords;
			for (size_t j = 0; j < fittingWords.size(); j++)
			{
				if (isValidInsertion(board, dictionary, fittingWords.at(j), position)) //Checks if word can be inserted
					validWords.push_back(fittingWords.at(j));
			}

			//Only attempt insertion if there are words for it
			if (validWords.size() != 0)
			{
				//Perform insertion
				int wordIndex = rand() % validWords.size();
				board.insertWord(validWords.at(wordIndex), position);
				board.insertWordHashes(validWords.at(wordIndex), position);
			}
		}
	}
	return board;
}
