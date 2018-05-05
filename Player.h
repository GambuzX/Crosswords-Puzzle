#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player
{
public:
	// Constructors
	Player();
	Player(std::string name);

	// Getters
	std::string getName();
	double getTotalTime();
	int getNumberOfClues();

private:
	std::string name;
	double initialTime, finalTime, totalTime;
	int numberOfClues = 0;
};


#endif