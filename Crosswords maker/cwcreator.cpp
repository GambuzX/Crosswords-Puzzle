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
	//string dictName = askDictionaryName();
	//Dictionary dictionary(dictName);
	//dictionary.ProcessDictionary();

	pair<int, int> board = askBoardSize();

	cout << board.first << "   " << board.second << endl;

	return 0;
}

//=================================================================================================================================

string askDictionaryName()
{
	string dictName;
	cout << "Dictionary name? ";
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