#include "Player.h"
#include <string>

using namespace std;

//==========================================================================================
//

Player::Player()
{
	//TODO store initial time
}

//==========================================================================================
// Constructor with name

Player::Player(string name)
{
	this->name = name;
	//TODO store initial time
}

//==========================================================================================

std::string Player::getName()
{
	return name;
}

//==========================================================================================

double Player::getTotalTime()
{
	return totalTime;
}

//==========================================================================================

int Player::getNumberOfClues()
{
	return numberOfClues;
}

//==========================================================================================



