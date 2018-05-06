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

	void incrementNumberOfClues();
	void setInitialTime();
	double calculateTimeSpent();

private:
	std::string name;
	time_t initialTime;
	double timeSpent;
	int numberOfClues = 0;
};


#endif