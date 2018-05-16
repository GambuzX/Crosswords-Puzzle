#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

//=================================================================================================================================
// Returns the name of the dictionary.

string Dictionary::getName(){	return dictionaryName;}

//=================================================================================================================================
// Returns a random synonym of the given word. Assumes the word is in the dictionary.

string Dictionary::GetWordSynonym(string word)
{
	vector<string> synonyms = wordList[toUpper(word)];
	int randomN = rand() % synonyms.size();
	return toUpper(synonyms.at(randomN));
}

//=================================================================================================================================
// Returns the synonym list for a given word.

vector<string> Dictionary::getWordSynonyms(string word) { return wordList[word]; }

//=================================================================================================================================
// Returns all the headlines in the word list.

vector<string> Dictionary::getHeadlines()
{ 
	vector<string> words;
	map<string, vector<string>>::iterator it;

	for (it = wordList.begin(); it != wordList.end(); it++)
		words.push_back(it->first);	
	return words; 
}

//=================================================================================================================================
// Returns a vector with all of the words that fit a determined space.

vector<string> Dictionary::fittingWords(int availableSpace)
{
	vector<string> validWords;
	map<string, vector<string>>::iterator it;
	for (it = wordList.begin(); it != wordList.end(); it++)
		if ((int)it->first.length() <= availableSpace)
			validWords.push_back(it->first);
	return validWords;
}

//=================================================================================================================================
// Sets the name of the dictionary.

void Dictionary::setName(string name){	dictionaryName = name;}

//=================================================================================================================================
// Displays the word list with its synonyms.

void Dictionary::showWordList()
{
	map<string, vector<string>>::iterator it;
	for (it = wordList.begin(); it != wordList.end(); it++)
	{
		cout << it->first << ": ";

		vector<string> vec = it->second;
		for (size_t i = 0; i < vec.size(); i++)
			cout << vec.at(i) << "; ";
		cout << endl;
	}
}

//=================================================================================================================================
// Verifies if dictionary with the name attribute exists and processes it, storing all word entries with its synonyms.
// All words are kept in uppercase.

bool Dictionary::ProcessDictionary()
{
	//Open dictionary
	ifstream dict(dictionaryName);
	if (!dict.is_open())
	{
		return false;
	}

	cout << "\nProcessing dictionary...\n";

	//Go through all the lines
	string line;
	while (getline(dict, line))
	{
		// Separate headline from synonyms
		size_t endPos = line.find_first_of(':');
		string word = toUpper( line.substr(0, endPos));
		line.erase(0, endPos + 2);

		//Go through all synonyms (except last one)
		vector<string> synonyms;
		string synonym;
		while (line.find_first_of(',') != string::npos)
		{
			endPos = line.find_first_of(',');
			synonym = line.substr(0, endPos);
			if (isValidSynonym(synonym))
				synonyms.push_back(toUpper(synonym));
			line.erase(0, endPos + 2);
		} 

		// At this point only the last word remains
		synonym = line;
		if (isValidSynonym(synonym))
			synonyms.push_back(toUpper(synonym));

		// If it is valid according to the class standards
		if (isValidHeadline(word))
		{
			pair<map<string, vector<string>>::iterator, bool> ret; //return type variable of map::insert
			ret = wordList.insert(pair<string, vector<string>>(word, synonyms));
			if (ret.second == false) //if insertion failed, i.e., it is a duplicate entry
			{
				// Since the entry is repeated, only adds new synonyms to the previous entry
				for (size_t i = 0; i < synonyms.size(); i++)
				{
					if (!isInVector(synonyms.at(i), wordList[word]))
						wordList[word].push_back(synonyms.at(i));
				}
			}
		}
	}
	return true;
}

//=================================================================================================================================
// Checks if a given word is in the word list.

bool Dictionary::isInWordList(string word)
{
	map<string, vector<string>>::iterator it;
	it = wordList.find(word);
	return it != wordList.end();
}

//=================================================================================================================================
// Verifies if the given headline is valid.

bool Dictionary::isValidHeadline(string word)
{
	for (size_t i = 0; i < word.length(); i++)
	{
		if (word.at(i) < 'A' || word.at(i) > 'Z')
			return false;
	}
	return true;
}

//=================================================================================================================================
// Verifies if the given synonym is valid.

bool Dictionary::isValidSynonym(string word)
{
	word = toUpper(word);
	for (size_t i = 0; i < word.length(); i++)
	{
		if ((word.at(i) < 'A' || word.at(i) > 'Z') && word.at(i) != ' ' && word.at(i) != '-' && word.at(i) != '\'')
			return false;
	}
	return true;
}

//=================================================================================================================================
// Converts given string to uppercase.

string Dictionary::toUpper(const string &word)
{
	string upperWord = word;
	for (size_t i = 0; i < upperWord.length(); i++)
		upperWord.at(i) = toupper(upperWord.at(i));
	return upperWord;
}

//=================================================================================================================================
// Checks if a given word is in a vector of words.

bool Dictionary::isInVector(string word, vector<string> words)
{
	for (size_t i = 0; i < words.size(); i++)
		if (words.at(i) == word)
			return true;
	return false;
}