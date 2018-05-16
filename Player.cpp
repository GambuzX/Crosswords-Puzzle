#include "Player.h"
#include <string>
#include <time.h>

using namespace std;

//=================================================================================================================================
// Constructor with no parameters. Sets the initial time to current time. Assures counters are at 0.

Player::Player()
{
	time(&initialTime); //sets initial time
	clues = 0;
	wrongSubmissions = 0;
}

//=================================================================================================================================
// Constructor with name as parameter. Stores name and sets the initial time to current time. Assures counters are at 0.

Player::Player(string name)
{
	this->name = name;
	time(&initialTime); //sets initial time
	clues = 0;
	wrongSubmissions = 0;
}

//=================================================================================================================================
// Returns the player name.

std::string Player::getName() {	return name; }

//=================================================================================================================================
// Returns number of clues the player asked for.

int Player::getNumberOfClues(){	return clues;}

//=================================================================================================================================
// Returns number of wrong submissions.

int Player::getNumberOfWrongSubmissions(){	return wrongSubmissions;}

//=================================================================================================================================
// Sets the initial time to current time.

void Player::setInitialTime() { time(&initialTime); }

//=================================================================================================================================
// Increments number of clues.

void Player::incrementNumberOfClues(){	clues++;}

//=================================================================================================================================
// Increments number of wrong submission.

void Player::incrementWrongSubmissions(){	wrongSubmissions++;}

//=================================================================================================================================
// Calculates the time in seconds it passed since the player was initialized, returning it.

double Player::calculateTimeSpent()
{
	time_t currentTime;
	time(&currentTime);
	timeSpent = difftime(currentTime, initialTime);
	return timeSpent; // return value is in seconds
}



