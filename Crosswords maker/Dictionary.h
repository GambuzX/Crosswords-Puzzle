#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include <map>

class Dictionary
{
public:
	//Constructors
	Dictionary(std::string dictionaryName);

	void ProcessDictionary();
	void showWordList();

	bool isWordInWordList(std::string word);

	std::string GetWordSynonym(std::string word);


private:
	std::map<std::string, std::vector<std::string>> wordList; // Key = word ; Value = vector of all synonyms
	std::string dictionaryName;

	bool isValid(std::string);
	std::string toUpper(const std::string &word);
};

#endif
