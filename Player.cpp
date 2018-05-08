#include "Player.h"
#include <string>
#include <time.h>

using namespace std;

//==========================================================================================
// Constructor with no parameters

Player::Player()
{
	time(&initialTime); //sets initial time
	clues = 0;
	wrongSubmissions = 0;
}

//==========================================================================================
// Constructor with name

Player::Player(string name)
{
	this->name = name;
	time(&initialTime); //sets initial time
	clues = 0;
	wrongSubmissions = 0;
}

//==========================================================================================
// Returns player name

std::string Player::getName() {	return name; }

//==========================================================================================
// Returns number of clues the player asked for

int Player::getNumberOfClues(){	return clues;}

//==========================================================================================
// Returns number of wrong submissions

int Player::getNumberOfWrongSubmissions(){	return wrongSubmissions;}

//==========================================================================================
// Increments number of clues

void Player::incrementNumberOfClues(){	clues++;}

//==========================================================================================
// Increments number of wrong submission

void Player::incrementWrongSubmissions(){	wrongSubmissions++;}

//==========================================================================================
// Sets the initial time

void Player::setInitialTime(){	time(&initialTime);}

//==========================================================================================
//Calculates the time in seconds it took since the player was initialized

double Player::calculateTimeSpent()
{
	time_t currentTime;
	time(&currentTime);
	timeSpent = difftime(currentTime, initialTime);
	return timeSpent; // return value is in seconds
}



