/*
Class that represents a dictionary. 
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
	//Constructors
	Dictionary() {};
	Dictionary(std::string dictName) : dictionaryName(dictName) {};

	//Getters
	std::string getName();
	std::string GetWordSynonym(std::string word);
	std::vector<std::string> getWordSynonyms(std::string word);
	std::vector<std::string> fittingWords(int availableSpace);
	std::vector<std::string> getHeadlines();

	void setName(std::string);
	void showWordList(); //To check if word list is well implemented

	bool ProcessDictionary();
	bool isInWordList(std::string word); // checks if word exists in word list
	bool isValidHeadline(std::string);
	bool isValidSynonym(std::string);
	//bool existsWildcardMatchingWord(std::string word);

private:
	std::map<std::string, std::vector<std::string>> wordList; // Key = word ; Value = vector of all synonyms
	std::string dictionaryName;

	bool isInVector(std::string, std::vector<std::string>);
	std::string toUpper(const std::string &word);
};

#endif
