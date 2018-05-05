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

	//Destructor
	~Dictionary();

	//Getters
	std::string getName();

	bool ProcessDictionary();
	void showWordList(); //To check if word list is well implemented
	bool isInWordList(std::string word); // checks if word exists in word list
	bool existsWildcardMatchingWord(std::string word);

	std::vector<std::string> fittingWords(int availableSpace);
	std::string GetWordSynonym(std::string word);


private:
	std::map<std::string, std::vector<std::string>> wordList; // Key = word ; Value = vector of all synonyms
	std::string dictionaryName;

	bool isValid(std::string);
	bool isInVector(std::string, std::vector<std::string>);
	std::string toUpper(const std::string &word);
	bool wildcardMatch(const char *str, const char *strWild);
};

#endif
