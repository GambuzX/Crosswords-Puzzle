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

//==========================================================================================
//COLOR CODES:

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3A
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
			cout << "\nCould not locate board with that name.\n";
			colorMaster.setcolor(DEFAULT);
			exit(1);
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
//TODO Instructions

void Instructions()
{
	cout << "Instructions: \n";
	cout << "(...)\n";

	//TODO Implement these ideas
	// C for clues at anytime
	// 
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
		string positionInput, word;

		////////////////////////////////
		//      ASK FOR POSITION      //
		////////////////////////////////

		bool validPositionInput = false;
		do
		{
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "Position ";
			colorMaster.setcolor(DEFAULT);
			cout << "(\"LCD\" / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "CTRL - Z";
			colorMaster.setcolor(DEFAULT);
			cout << " = stop) ";
			colorMaster.setcolor(QUESTION_COLOR);
			cout << "? ";
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
				cout << endl;
				answer = toupper(answer);
				if (answer == 'Y')
					leave = true;
			} while (answer != 'Y' && answer != 'N');

			if (leave)
				break;
			else
				continue;
		}

		////////////////////////////////
		//        ASK FOR WORD        //
		////////////////////////////////

		bool validInput = false;
		bool skipInsertion = false;
		do
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}
			colorMaster.setcolor(QUESTION_COLOR); //TODO button for instructions instead of displaying all things
			cout << "Word";
			colorMaster.setcolor(DEFAULT);
			cout << " (";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "<";
			colorMaster.setcolor(DEFAULT);
			cout << " = return / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "-";
			colorMaster.setcolor(DEFAULT);
			cout << " = remove / ";
			colorMaster.setcolor(SYMBOL_COLOR);
			cout << "?";
			colorMaster.setcolor(DEFAULT);
			cout << " = help)";
			colorMaster.setcolor(QUESTION_COLOR);
			cout << " ? ";
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