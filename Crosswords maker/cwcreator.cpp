#include <iostream>
#include <string>

#include "Dictionary.h"
#include "Board.h"

using namespace std;

string askForDictionaryName();

int main()
{
	string dictName = askForDictionaryName();
	Dictionary dictionary(dictName);
	dictionary.ProcessDictionary();

	cout << dictionary.isValid("ABOVE") << dictionary.isValid("POTATO") << dictionary.isValid("GAMBUZINO") << endl;

	return 0;
}

//=================================================================================================================================

string askForDictionaryName()
{
	string dictName;
	cout << "Dictionary name? ";
	cin >> dictName;
	return dictName;
}