/*
Class that represents a dictionary. 
*
To use it, associate the name of the dictionary and process it. The dictionary file lines must be in the format [Headline]: [synonym1], [synonym2], ... [synonymN].
This will create a word list with all the headlines and synonyms, allowing you to use all the remaining methods.
*/

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include <map>

class Dictionary
{
public:
	// Constructors
	Dictionary() {};
	Dictionary(std::string dictName) : dictionaryName(dictName) {};

	// Getters
	std::string getName();
	std::string GetWordSynonym(std::string word);
	std::vector<std::string> getWordSynonyms(std::string word);
	std::vector<std::string> getHeadlines();
	std::vector<std::string> fittingWords(int availableSpace);

	// Setters
	void setName(std::string);
	
	// Dictionary management and interaction
	void showWordList();
	bool ProcessDictionary();

	// Validity tests
	bool isInWordList(std::string word);
	bool isValidHeadline(std::string);
	bool isValidSynonym(std::string);

private:
	// Dictionary words
	std::map<std::string, std::vector<std::string>> wordList; // Key = word ; Value = vector of all synonyms

	std::string dictionaryName;

	// Utility
	std::string toUpper(const std::string &word);
	bool isInVector(std::string, std::vector<std::string>);
};

#endif
