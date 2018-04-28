#include <iostream>
#include <string>
#include <map>

#include "Dictionary.h"
#include "Board.h"

using namespace std;

string askDictionaryName();
pair<int, int> askBoardSize();

int main()
{
	string dictName = askDictionaryName();
	Dictionary dictionary(dictName);
	dictionary.ProcessDictionary();

	pair<int, int> boardSize = askBoardSize();

	Board board(boardSize.first, boardSize.second, dictionary);
	board.showBoard();
	cout << endl << endl;

	board.insertWord("PUZZLE", pair<int, int>(1, 1), 'H');
	board.showBoard();
	cout << endl << endl;

	board.insertWord("PRODIGY", pair<int, int>(1, 1), 'V');
	board.showBoard();
	cout << endl << endl;

	board.helpUser(pair<int, int>(1, 1), 'H');
	cout << endl << endl;

	board.helpUser(pair<int, int>(2, 1), 'H');
	cout << endl << endl;

	return 0;
}

// TODO Assure uppercase words
// TODO Make sure max size is 26 (because of letters)

//=================================================================================================================================

string askDictionaryName()
{
	string dictName;
	cout << "Dictionary file name? ";
	cin >> dictName;
	return dictName;
}

pair<int,int> askBoardSize()
{
	pair<int, int> boardSize;

	cout << "Horizontal board size? ";	
	cin >> boardSize.first;
	while (!cin)
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Horizontal board size? ";
		cin >> boardSize.first;
	}

	cout << "Vertical board size? ";
	cin >> boardSize.second;
	while (!cin)
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Vertical board size? ";
		cin >> boardSize.second;
	}
	return boardSize;
}