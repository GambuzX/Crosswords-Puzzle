#include <iostream>
#include <string>

#include "Dictionary.h"
#include "Board.h"

using namespace std;

string askDictionaryName();

int main()
{
	string dictName = askDictionaryName();
	Dictionary dictionary(dictName);
	dictionary.ProcessDictionary();

	cout << dictionary.isValid("ABOVE") << dictionary.isValid("POTATO") << dictionary.isValid("GAMBUZINO") << endl;

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

int askBoardSize()
{
	int boardSize;
	cout << "Board size? ";
	
	cin >> boardSize;
	while (!cin)
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Board size? ";
		cin >> boardSize;
	}
	return boardSize;
}