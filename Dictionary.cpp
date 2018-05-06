#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/*Dictionary::Dictionary(std::string dictionaryName)
{
	this->dictionaryName = dictionaryName;
}*/

//=================================================================================================================================
// Returns the name of the dictionary

string Dictionary::getName()
{
	return dictionaryName;
}

//=================================================================================================================================
// Returns the synonym list for a given word

vector<string> Dictionary::getWordSynonyms(string word)
{
	return wordList[word];
}

//=================================================================================================================================
// Verifies if dictionary exists and processes it, storing all word entries
// All words kept in uppercase

bool Dictionary::ProcessDictionary()
{
	ifstream dict(dictionaryName);

	if (!dict.is_open())
	{
		return false;
	}

	cout << "\nProcessing dictionary...\n";

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
			synonyms.push_back(toUpper(synonym)); //Assume all synonyms are valid
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
// Displays the word list with its synonyms

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
// Checks if a given word is in the word list

bool Dictionary::isInWordList(string word)
{
	map<string, vector<string>>::iterator it;
	it = wordList.find(word);
	return it != wordList.end();
}

//=================================================================================================================================
// Verifies if any word matches the given one using wildcard match

bool Dictionary::existsWildcardMatchingWord(string word)
{
	map<string, vector<string>>::iterator it;
	string matchingWord = word + '*'; //Add wildcard operator
	for (it = wordList.begin(); it != wordList.end(); it++)
	{
		if (wildcardMatch(it->first.c_str(), matchingWord.c_str()))
			return true;
	}
	return false;
}

//=================================================================================================================================
// Returns a vector with all of the words that fit a determined space

vector<string> Dictionary::fittingWords(int availableSpace)
{
	vector<string> validWords;
	map<string, vector<string>>::iterator it;
	for (it = wordList.begin(); it != wordList.end(); it++)
		if ((int) it->first.length() <= availableSpace)
			validWords.push_back(it->first);
	return validWords;
}

//=================================================================================================================================
// Returns a random synonym of the given word. Assumes the word is in the dictionary

string Dictionary::GetWordSynonym(string word)
{
	vector<string> synonyms = wordList[toUpper(word)];
	int randomN = rand() % synonyms.size();
	return toUpper(synonyms.at(randomN)); // returns a random synonym from the vector of synonyms
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
	for (size_t i = 0; i < words.size(); i++)
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

//=================================================================================================================================
// WildcardMatch
// str - Input string to match
// strWild - Match mask that may contain wildcards like ? and *
//
// A ? sign matches any character, except an empty string.
// A * sign matches any string inclusive an empty string.
// Characters are compared caseless.
//
// ADAPTED FROM:
// https://www.codeproject.com/Articles/188256/A-Simple-Wildcard-Matching-Function
bool Dictionary::wildcardMatch(const char *str, const char *strWild)
{
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '?')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*')
		{
			// Need to do some tricks.
			// 1. The wildcard * is ignored.
			// So just an empty string matches. This is done by recursion.
			// Because we eat one character from the match string,
			// the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again,
			// with a wildcard * match. This is done by recursion.
			// Because we eat one character from the string,
			// the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}