#include <iostream>
#include <string>
#include <map>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#include "ColorMaster.h"
#include "Dictionary.h"
#include "Board.h"
#include "Player.h"
#include "Puzzle.h"

using namespace std;

void Introduction();
void Instructions();
string askBoardName();
string askPlayerName();
void solveCurrentPuzzle(Puzzle&);
bool CheckPlayerWon(Puzzle&);

ColorMaster colorMaster;

//TODO test everything
//TODO clean up code

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

//==========================================================================================

int main()
{
	srand(time_t(NULL));

	Introduction();
	cout << endl;

	char answer;
	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
		}
		colorMaster.setcolor(QUESTION_COLOR);
		cout << "Display instructions (Y/N)? ";
		colorMaster.setcolor(DEFAULT);
		cin >> answer;
		answer = toupper(answer);
	} while (answer != 'Y' && answer != 'N');

	if (answer == 'Y')
		Instructions();
	cout << endl;
	
	bool finishedProgram = false;
	do
	{
		//LOAD BOARD
		string dictName;
		string boardName = askBoardName();
		Board *board = new Board();
		bool boardLoaded = board->loadBoard(boardName, dictName);
		if (!boardLoaded)
		{
			colorMaster.setcolor(ERROR_MESSAGE);
			cout << "\nCould not locate board file with that name.\n";
			colorMaster.setcolor(DEFAULT);
			exit(1);
		}
		else
		{
			colorMaster.setcolor(SUCCESS);
			cout << "\nBoard file was opened successfully.\n";
			colorMaster.setcolor(DEFAULT);
		}

		//OPEN DICTIONARY
		Dictionary *dictionary = new Dictionary(dictName);
		bool dictOpen = dictionary->ProcessDictionary();

		if (!dictOpen)
		{
			colorMaster.setcolor(ERROR_MESSAGE);
			cout << "\nCould not locate dictionary with that name.\n";
			colorMaster.setcolor(DEFAULT);
			exit(1);
		}
		else
		{
			colorMaster.setcolor(SUCCESS);
			cout << "\nDictionary was opened successfully.\n";
			colorMaster.setcolor(DEFAULT);
		}

		//CREATE PLAYER
		cin.ignore(10000, '\n'); //needed because of remaining '\n' in the buffer
		cout << endl;
		string playerName = askPlayerName();
		Player *player = new Player(playerName);

		//CREATE PUZZLE TO BE SOLVED
		Puzzle puzzle(*board, *dictionary, boardName, *player);
		delete board; //board is copied to the puzzle instance, duplicate is not needed
		delete dictionary; // same with dictionary
		delete player; // same with player
		puzzle.createPlayerBoard();
		puzzle.buildClueList();

		solveCurrentPuzzle(puzzle);

		char answer;
		do
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "\nDo you wish to play again (Y/N) ? ";
			colorMaster.setcolor(DEFAULT);
			cin >> answer;
			cout << endl;
			answer = toupper(answer);
		} while (answer != 'N' && answer != 'Y');

		if (answer == 'N') //Leave program
			finishedProgram = true;

	} while (!finishedProgram);

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Thanks for playing! Hope you had fun :D\n";
	colorMaster.setcolor(DEFAULT);

	exit(0);
	return 0;
}

//TODO option to submit solution

//=================================================================================================================================
// Displays the introduction

void Introduction()
{
	cout << endl << " ";
	colorMaster.setcolor(BLACK, WHITE);
	cout << "  ____                                         _       ____        _                \n";
	colorMaster.setcolor(WHITE, BLACK); cout << " "; colorMaster.setcolor(BLACK, WHITE);
	cout << " / ___|_ __ ___  ___ _____      _____  _ __ __| |___  / ___|  ___ | |_   _____ _ __ \n";
	colorMaster.setcolor(WHITE, BLACK); cout << " "; colorMaster.setcolor(BLACK, WHITE);
	cout << "| |   | '__/ _ \\/ __/ __\\ \\ /\\ / / _ \\| '__/ _` / __| \\___ \\ / _ \\| \\ \\ / / _ \\ '__|\n";
	colorMaster.setcolor(WHITE, BLACK); cout << " "; colorMaster.setcolor(BLACK, WHITE);
	cout << "| |___| | | (_) \\__ \\__ \\\\ V  V / (_) | | | (_| \\__ \\  ___) | (_) | |\\ V /  __/ |   \n";
	colorMaster.setcolor(WHITE, BLACK); cout << " "; colorMaster.setcolor(BLACK, WHITE);
	cout << " \\____|_|  \\___/|___/___/ \\_/\\_/ \\___/|_|  \\__,_|___/ |____/ \\___/|_| \\_/ \\___|_|   \n";
	colorMaster.setcolor(WHITE, BLACK);
}

//=================================================================================================================================
// Displays the instructions

void Instructions()
{
	colorMaster.setcolor(BLACK, WHITE);
	cout << "\nINSTRUCTIONS\n\n";
	colorMaster.setcolor(WHITE, BLACK);

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "White cells";
	colorMaster.setcolor(DEFAULT);
	cout << " represent places where you ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "can";
	colorMaster.setcolor(DEFAULT);
	cout << " insert words\n";

	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "Black cells";
	colorMaster.setcolor(DEFAULT);
	cout << " represent places where you ";
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
	cout << "S";
	colorMaster.setcolor(DEFAULT);
	cout << " to submit current board, if board is full.\n";
	cout << "- ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "CTRL-Z";
	colorMaster.setcolor(DEFAULT);
	cout << " to stop playing.\n";

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
	cout << " to ask for an hint about the word in the specified position.\n";
	cout << "- ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "<";
	colorMaster.setcolor(DEFAULT);
	cout << " to return to the Position question.\n";

	cout << "\nWhen there are no more empty spaces to fill, you will be prompted to ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "submit your solution";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n";

	cout << "You will then get ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "feedback";
	colorMaster.setcolor(DEFAULT);
	cout << " on your submission.\n";

	cout << "If you guess all the words, ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "statistics on your performance";
	colorMaster.setcolor(DEFAULT);
	cout << " will be shown and ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "saved to a file";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n";

	cout << "If your guess is incorrect and you decide to leave, you can choose to see the ";
	colorMaster.setcolor(SYMBOL_COLOR);
	cout << "solutions";
	colorMaster.setcolor(DEFAULT);
	cout << ".\n";

	//cout << "\nPress any key to continue\n";
	_getch();
	//TODO make more appealing
}

//=================================================================================================================================
//TODO Show options

void Options()
{
	cout << "Options: \n";
	cout << " (...)\n";
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
// Asks for the name of the player

string askPlayerName()
{
	string playerName;
	colorMaster.setcolor(QUESTION_COLOR);
	cout << "Player name? ";
	colorMaster.setcolor(DEFAULT);
	getline(cin, playerName);
	return playerName;
}

//=================================================================================================================================
// Asks the user to solve the current puzzle.

void solveCurrentPuzzle(Puzzle &puzzle)
{
	puzzle.showPlayerBoard();
	puzzle.showClueList();
	cout << endl;

	bool finishedPuzzle = false;
	do
	{
		bool skipInsertion = false;
		string positionInput, word;

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
					finishedPuzzle = true;
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
					cout << endl;
					Instructions();
					validPositionInput = true;
					skipInsertion = true;
				}

				//Check validity
				if (puzzle.validPositionInput(positionInput))
				{
					if (puzzle.hasHash(positionInput))
					{
						colorMaster.setcolor(ERROR_MESSAGE);
						cout << "\nYou can not place a word in that location.\n\n";
						colorMaster.setcolor(DEFAULT);
					}
					else if (!puzzle.isValidInsertionLocation(positionInput)) //Only accept input if there is a word starting there
					{
						colorMaster.setcolor(ERROR_MESSAGE);
						cout << "\nThere is no word starting in that location and direction.\n\n";
						colorMaster.setcolor(DEFAULT);
					}
					else
					{
						validPositionInput = true;
					}
				}
			}
		} while (!validPositionInput); //loop until valid input

		if (finishedPuzzle) //If CTRL-Z, confirm if the player wishes to leave
		{
			bool leave = false;
			char answer;
			do
			{
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(10000, '\n');
				}
				colorMaster.setcolor(QUESTION_COLOR);
				cout << "\nIf you leave all your progress will be lost. Proceed (Y/N)? ";
				colorMaster.setcolor(DEFAULT);
				cin >> answer;
				answer = toupper(answer);
				if (answer == 'Y')
					leave = true;
			} while (answer != 'Y' && answer != 'N');

			if (leave)
				break;
			else
			{
				skipInsertion = true;
				finishedPuzzle = false;
			}
		}

		////////////////////////////////
		//        ASK FOR WORD        //
		////////////////////////////////

		bool validInput = false;
		do
		{
			if (skipInsertion)
				break;			

			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}
			colorMaster.setcolor(QUESTION_COLOR); //TODO button for instructions instead of displaying all things
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
				bool wordRemoved = puzzle.removeWord(positionInput);
				if (wordRemoved)
					validInput = true; // exit loop
				cout << endl;
			}
			else if (word == "?") // Ask for help
			{
				puzzle.showDifferentSynonym(positionInput);
				cout << endl;
			}
			else if (word == "I") // Ask for help
			{
				Instructions();
				cout << endl << endl;
				puzzle.showPlayerBoard();
				cout << endl;
			}
			else // default
			{
				if (puzzle.canBeInserted(word, positionInput)) //Check validity and output error messages if necessary 
				{
					puzzle.insertWord(word, positionInput);
					validInput = true;
				}
			}
		} while (!validInput); //loop until valid input
		
		cout << endl;
		puzzle.showPlayerBoard();
		cout << endl;

		if (puzzle.getNumberOfPlayerWords() == puzzle.getNumberOfSolutionWords())
		{
			bool endPuzzle = CheckPlayerWon(puzzle);
			if (endPuzzle)
				finishedPuzzle = true;
			else
			{
				cout << endl;
				puzzle.showPlayerBoard();
				cout << endl;
			}
		}

	} while (!finishedPuzzle);
}

//=================================================================================================================================
// Checks if the player has won. Returns a bool indicating whether or not to stop editing puzzle.

bool CheckPlayerWon(Puzzle& puzzle)
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
		cout << "Submit solution (Y/N)? ";
		colorMaster.setcolor(DEFAULT);
		cin >> answer;
		answer = toupper(answer);
	} while (answer != 'N' && answer != 'Y');

	if (answer == 'Y')
	{
		if (puzzle.boardsMatch()) //if the player won
		{
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "\nCongratulations! You have guessed all the words!\n";
			colorMaster.setcolor(DEFAULT);

			//display player stats
			puzzle.showPlayerStats();

			//save puzzle
			puzzle.saveStats();
			
			colorMaster.setcolor(SUCCESS);
			cout << "\nPlayer stats were saved successfully.\n";
			colorMaster.setcolor(DEFAULT);

			return true;
		}
		else
		{
			puzzle.addWrongSubmission();
			int correctWords = puzzle.calculateNumberOfCorrectWords();
			cout << "\nYou have guessed ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << correctWords;
			colorMaster.setcolor(DEFAULT);
			cout << " words!\nTake a better look at these ones:\n";

			puzzle.showWrongAnswers();

			//Ask if player wants to keep trying
			char answer2;
			do
			{
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(10000, '\n');
				}
				colorMaster.setcolor(QUESTION_COLOR);
				cout << "\nKeep playing (Y/N)? ";
				colorMaster.setcolor(DEFAULT);
				cin >> answer2;
				answer2 = toupper(answer2);
			} while (answer2 != 'N' && answer2 != 'Y');

			if (answer2 == 'N')
			{
				//Ask if player wants to see the solutions
				char answer3;
				do
				{
					if (cin.fail())
					{
						cin.clear();
						cin.ignore(10000, '\n');
					}
					colorMaster.setcolor(QUESTION_COLOR);
					cout << "\nShow solutions (Y/N)? ";
					colorMaster.setcolor(DEFAULT);
					cin >> answer3;
					answer3 = toupper(answer3);
				} while (answer3 != 'N' && answer3 != 'Y');

				if (answer3 == 'Y')
					puzzle.showSolutionBoard();

				//leave
				return true;
			} 
		}
	}
	return false; //keep playing
}