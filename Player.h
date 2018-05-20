/*
Class that represents a Player, adapted to a Crosswords Game.
*
Is initialized with the player name. Keeps track of the time since game started, number of extra clues asked and wrong submissions.
*/

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
	int getNumberOfSolutions();
	int getNumberOfWrongSubmissions();

	//Setters
	void setInitialTime();

	//Incrementers
	void incrementNumberOfClues();
	void incrementNumberOfSolutions();
	void incrementWrongSubmissions();

	//Calculate time since player was initiliazed
	double calculateTimeSpent();

private:
	std::string name;

	time_t initialTime;

	double timeSpent;

	int clues = 0, solutions = 0, wrongSubmissions = 0;
};


#endif