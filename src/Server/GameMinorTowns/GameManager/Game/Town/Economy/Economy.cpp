#include "Economy.h"

Economy::Economy()
{
}

Economy::Economy(int startBudget)
{
	this->income(startBudget);
}

Economy::~Economy()
{
}

const int Economy::getBudget()
{
	return this->budget;
}

const int Economy::getNetWorth()
{
	return this->netWorth;
}

void Economy::expense(int value)
{
	this->budget -= value;
	this->netWorth -= value;
}

void Economy::income(int value)
{
	this->budget += value;
	this->netWorth += value;
}
