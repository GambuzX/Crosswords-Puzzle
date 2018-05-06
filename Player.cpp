#include "Player.h"
#include <string>
#include <time.h>

using namespace std;

//==========================================================================================
//

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

std::string Player::getName()
{
	return name;
}

//==========================================================================================

int Player::getNumberOfClues()
{
	return clues;
}

//==========================================================================================

int Player::getNumberOfWrongSubmissions()
{
	return wrongSubmissions;
}

//==========================================================================================

void Player::incrementNumberOfClues()
{
	clues++;
}

//==========================================================================================

void Player::incrementWrongSubmissions()
{
	wrongSubmissions++;
}

//==========================================================================================

void Player::setInitialTime()
{
	time(&initialTime);
}

//==========================================================================================
//Calculates the time in seconds it took since the player was initialized

double Player::calculateTimeSpent()
{
	time_t currentTime;
	time(&currentTime);
	timeSpent = difftime(initialTime, currentTime);
	return timeSpent; // return value is in seconds
}

//==========================================================================================



