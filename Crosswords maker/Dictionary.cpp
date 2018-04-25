#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

Dictionary::Dictionary(std::string dictionaryName)
{
	this->dictionaryName = dictionaryName;
}

//=================================================================================================================================

void Dictionary::ProcessDictionary()
{
	if (dictionaryName == "")
	{
		cout << "Dictionary name? ";
		cin >> dictionaryName;
	}

	ifstream dict(dictionaryName);

	if (!dict.is_open())
	{
		cout << "File with that name does not exist.\n";
		exit(1);
	}

	string line;
	while (getline(dict, line))
	{
		size_t endPos = line.find_first_of(':');
		string word = toUpper( line.substr(0, endPos));
		line.erase(0, endPos + 2);

		vector<string> synonyms;
		string synonym;
		while (line.find_first_of(',') != string::npos)
		{
			endPos = line.find_first_of(',');
			synonym = line.substr(0, endPos);
			synonyms.push_back(synonym); //Assume all synonyms are valid
			line.erase(0, endPos + 2);
		}
		synonym = line; //at this point only the last word remains
		synonyms.push_back(synonym);
		if (isValid(word))
			wordList.insert(pair<string, vector<string>>(word, synonyms));
	}
}

//=================================================================================================================================

bool Dictionary::isValid(string word)
{
	for (int i = 0; i < word.length(); i++)
	{
		if (word.at(i) < 'A' || word.at(i) > 'Z')
			return false;
	}
	return true;
}

//=================================================================================================================================

string Dictionary::toUpper(const string &word)
{
	string upperWord = word;
	for (int i = 0; i < upperWord.length(); i++)
		upperWord.at(i) = toupper(upperWord.at(i));
	return upperWord;
}

//=================================================================================================================================

void Dictionary::showWordList()
{
	map<string, vector<string>>::iterator it;
	for (it = wordList.begin(); it != wordList.end(); it++)
	{
		cout << it->first << ": ";

		vector<string> vec = it->second;
		for (int i = 0; i < vec.size(); i++)
			cout << vec.at(i) << "; ";
		cout << endl;
	}
}