#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

/*Dictionary::Dictionary(std::string dictionaryName)
{
	this->dictionaryName = dictionaryName;
}*/

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
		{
			pair<map<string, vector<string>>::iterator, bool> ret; //return type variable of map::insert
			ret = wordList.insert(pair<string, vector<string>>(word, synonyms));
			if (ret.second == false) //if insertion failed, i.e., it is a duplicate entry
			{
				// Since the entry is repeated, only adds new synonyms to the previous entry
				for (int i = 0; i < synonyms.size(); i++)
				{
					if (!isInVector(synonyms.at(i), wordList[word]))
						wordList[word].push_back(synonyms.at(i));
				}
			}
		}
	}
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

//=================================================================================================================================
// Checks if a given word is in the word list

bool Dictionary::isInWordList(string word)
{
	map<string, vector<string>>::iterator it;
	it = wordList.find(word);
	return it != wordList.end();
}

//=================================================================================================================================
vector<string> Dictionary::fittingWords(int availableSpace)
{
	vector<string> validWords;
	map<string, vector<string>>::iterator it;
	for (it = wordList.begin(); it != wordList.end(); it++)
		if (it->first.length() <= availableSpace)
			validWords.push_back(it->first);
	return validWords;
}

//=================================================================================================================================
// Verifies the given headline is valid

bool Dictionary::isValid(string word)
{
	for (size_t i = 0; i < word.length(); i++)
	{
		if (word.at(i) < 'A' || word.at(i) > 'Z')
			return false;
	}
	return true;
}

//=================================================================================================================================
// Checks if a given word is in a vector of words

bool Dictionary::isInVector(string word, vector<string> words)
{
	for (int i = 0; i < words.size(); i++)
		if (words.at(i) == word)
			return true;
	return false;
}

//=================================================================================================================================
// Converts given string to uppercase

string Dictionary::toUpper(const string &word)
{
	string upperWord = word;
	for (size_t i = 0; i < upperWord.length(); i++)
		upperWord.at(i) = toupper(upperWord.at(i));
	return upperWord;
}
