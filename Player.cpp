#include "Player.h"
#include <string>
#include <time.h>

using namespace std;

//==========================================================================================
//

Player::Player()
{
	time(&initialTime); //sets initial time
}

//==========================================================================================
// Constructor with name

Player::Player(string name)
{
	this->name = name;
	time(&initialTime); //sets initial time
}

//==========================================================================================

std::string Player::getName()
{
	return name;
}

//==========================================================================================

int Player::getNumberOfClues()
{
	return numberOfClues;
}

//==========================================================================================

void Player::incrementNumberOfClues()
{
	numberOfClues++;
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



