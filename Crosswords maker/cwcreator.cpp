#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <fstream>
#include <conio.h>
#include "ColorMaster.h"
#include "Dictionary.h"
#include "Board.h"

using namespace std;

//TODO New mode -> Less restrictive board creation, with words not from the dictionary. Completely up to the user -> may add synonyms by hand

//TODO varrer tabuleiro e procurar palavras automaticamente formadas

//TODO _getch() after instructions
//TODO Clean up code
//TODO Specify objectives of each file and function
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
void Instructions();
void Options();

char YesNoQuestion(string question);

string askDictionaryName();
string askBoardName();
string determineBoardName();

pair<int, int> askBoardSize();

Dictionary CreateDictionary(bool &success);
Dictionary CreateDictionary(string dictName, bool &success);

Board CreateBoard();
Board ResumeBoard(string &dictName, bool &operationSuccess);

bool askToSaveBoard(Board board, string dictName);
bool canBeInserted(Board &board, Dictionary &dictionary, string word, string positionInput);
bool isBoardValid(Board &board, Dictionary &dictionary);
bool isBoardValid(Board &board, Dictionary &dictionary, string word, string position);
bool testInsertion(Board &board, Dictionary &dictionary, string word, string positionInput);

void helpUser(Board &board, Dictionary &dictionary, string positionInput);
void EditBoard(Board board, Dictionary &dict);

ColorMaster colorMaster;

//=================================================================================================================================

int main()
{
	Introduction();
	cout << endl;

	char answer = YesNoQuestion("Display instructions (Y/N)? ");
	if (answer == 'Y')
		Instructions();

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
		} while (cin.fail() || answer < 0 || answer > 2);

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
			cout << " ====================================\n";
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
			cout << " ====================================\n";
			cout << " |          RESUME PUZZLE           |\n";
			cout << " ====================================\n";

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

void Instructions()
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

	cout << "You will be asked successively two questions, ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "POSITION";
	colorMaster.setcolor(DEFAULT);
	cout << " and ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "WORD";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n\n";

	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Position\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Format: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "LCD";
	colorMaster.setcolor(DEFAULT);
	cout << " (line, column, direction).\n";
	cout << "Can either be lower or uppercase, but ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "must respect this order!";
	colorMaster.setcolor(DEFAULT);
	cout << " Direction is 'H' or 'V'.\n\n";

	cout << "Other options: \n";
	cout << "- "; 
	colorMaster.setcolor(SYMBOL_COLOR); 
	cout << "I";
	colorMaster.setcolor(DEFAULT);
	cout << " to display these instructions.\n";
	cout << "- ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "CTRL-Z";
	colorMaster.setcolor(DEFAULT);
	cout << " to stop creating the board.\n";

	colorMaster.setcolor(QUESTION_COLOR);
	cout << "\nWord\n";
	colorMaster.setcolor(DEFAULT);
	cout << "Format: ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Word to be inserted in the board\n\n";
	colorMaster.setcolor(DEFAULT);

	cout << "Other options: \n";
	cout << "- ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "I";
	colorMaster.setcolor(DEFAULT);
	cout << " to display these instructions.\n";
	cout << "- ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "-";
	colorMaster.setcolor(DEFAULT);
	cout << " to remove a previously placed word.\n";
	cout << "- ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "?";
	colorMaster.setcolor(DEFAULT);
	cout << " for a list of words that can be placed on the specified position.\n";
	/*cout << "- ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "#";
	colorMaster.setcolor(DEFAULT);
	cout << " to insert an Hash.\n";*/
	cout << "- ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "<";
	colorMaster.setcolor(DEFAULT);
	cout << " to return to the Position question.\n";

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

	//cout << "\nPress any key to continue\n";
	_getch();
	//TODO make more appealing
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
	cout << " - Create puzzle\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "2";
	colorMaster.setcolor(DEFAULT);
	cout << " - Resume puzzle\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "0";
	colorMaster.setcolor(DEFAULT);
	cout << " - Exit\n";
}

//=================================================================================================================================
//Asks a Yes / No question and returns the answer char

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
// Asks for the name of the board

string askBoardName()
{
	string boardName;
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Board file name? ";
	colorMaster.setcolor(DEFAULT);
	cin >> boardName;
	return boardName;
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
	} while (!cin || boardSize.first <= 0 || boardSize.first > 26);

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

bool askToSaveBoard(Board board, string dictName)
{
	bool success = true;
	char answer = YesNoQuestion("Do you wish to save the current board (Y/N) ? ");

	if (answer == 'Y')
	{
		//Check if board is finished or not
		char answer2 = YesNoQuestion("Is the board finished (Y/N) ? ");
		if (answer2 == 'Y')
			board.fillRemainingSpots();

		//Save file
		string fileName = determineBoardName();
		success = board.saveBoard(fileName, dictName);
		if (success)
		{
			colorMaster.setcolor(SUCCESS);
			cout << "\nBoard was saved successfully.\n";
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
// Creates a new dictionary

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
		//_getch();
		success = false;
		return Dictionary();
	}
	success = true;
	return dictionary;
}

//=================================================================================================================================
// Creates a new dictionary

Dictionary CreateDictionary(string dictName, bool &success)
{
	Dictionary dictionary(dictName);
	bool dictionaryOpened = dictionary.ProcessDictionary();
	if (!dictionaryOpened)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nCould not locate dictionary with that name.\n";
		colorMaster.setcolor(DEFAULT);
		//_getch();
		success = false;
		return Dictionary();
	}
	success = true;
	return dictionary;
}

//=================================================================================================================================
// Creates a new puzzle from scratch

Board CreateBoard()
{
	pair<int, int> boardSize = askBoardSize();
	Board board(boardSize.first, boardSize.second);
	return board;
}

//=================================================================================================================================
// Resumes an already existing puzzle from a file

Board ResumeBoard(string &dictionaryName, bool &operationSuccess)
{
	cout << endl;
	string boardName = askBoardName();
	Board board;
	string dictName;
	bool boardLoaded = board.loadBoard(boardName, dictName);

	if (!boardLoaded)
	{
		colorMaster.setcolor(ERROR_MESSAGE);
		cout << "\nCould not locate board file with that name.\n";
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
	else if (!dictionary.isValidHeadline(word)) // Verify word is valid
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
// Verifies if all words in the board in both directions are valid

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
// Verifies if all words in the board in both directions are valid, but limited to positions the word occupies

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
// Simulates an insertion and verifies if the resulting board is valid.

bool testInsertion(Board &board, Dictionary &dictionary, string word, string positionInput)
{
	//Backup
	std::vector<std::vector<char>> oldboard = board.getBoard();
	std::vector<std::pair<std::string, std::string>> oldUsedWords = board.getUsedWords();

	board.insertWord(word, positionInput);
	bool valid = isBoardValid(board, dictionary, word, positionInput);

	//Restore backup
	board.setBoard(oldboard);
	board.setUsedWords(oldUsedWords);

	return valid;
}

//=================================================================================================================================

void helpUser(Board &board, Dictionary &dictionary, string positionInput)
{
	// insertionPos = (line, column)
	pair<int, int> insertionPosition = board.calculateInsertionCoordinates(positionInput);
	char direction = toupper(positionInput.at(2));
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

	vector<string> fittingWords = dictionary.fittingWords(availableSpace);
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
// Allows to make changes to an existing board

void EditBoard(Board board, Dictionary &dict)
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
			cout << "Position ? ";
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

				pair<int, int> coordinates = board.calculateInsertionCoordinates(positionInput);

				if (positionInput == "I")
				{
					cout << endl;
					Instructions();
					validPositionInput = true;
					skipLoop = true;
				}

				//Check validity
				else if (board.validPositionInput(positionInput))
				{
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
			bool successfulSave = askToSaveBoard(board, dict.getName());
			if (!successfulSave) //if there was a problem saving board, continue with the loop
				continue;
			break; //if successful save of the board, end loop
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
			cout << "Word ? ";
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
			else if (word == "I") // Ask for help
			{
				Instructions();
				cout << endl << endl;
				board.showBoard();
				cout << endl;
			}
			else // default
				if (canBeInserted(board, dict, word, positionInput)) //Check validity and output error messages if necessary
				{
					board.insertWord(word, positionInput);
					validInput = true;
				}
		} while (!validInput); //loop until valid input

		cout << endl;
		board.showBoard();
		cout << endl;
	}
}
