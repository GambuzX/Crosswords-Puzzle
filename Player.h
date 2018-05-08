#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <time.h>

class Player
{
public:
	// Constructors
	Player();
	Player(std::string name);

	// Getters
	std::string getName();
	int getNumberOfClues();
	int getNumberOfWrongSubmissions();

	void incrementNumberOfClues();
	void incrementWrongSubmissions();
	void setInitialTime();

	double calculateTimeSpent();

private:
	std::string name;

	time_t initialTime;

	double timeSpent;

	int clues = 0, wrongSubmissions = 0;
};


#endif