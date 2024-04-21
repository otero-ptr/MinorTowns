#include "Economy.h"

Economy::Economy(double startBudget, double tickIncome)
{
	this->income(startBudget);
	this->tickIncome = tickIncome;
}

Economy::~Economy()
{
}

const double Economy::getBudget()
{
	return this->budget;
}

const double Economy::getNetWorth()
{
	return this->netWorth;
}

const double Economy::getTickIncome()
{
	return this->tickIncome;
}

const double Economy::getMultiplier()
{
	return this->multiplier;
}

void Economy::setTickIncome(double value)
{
	this->tickIncome = value;
}

void Economy::setMultiplier(double value)
{
	this->multiplier = value;
}

void Economy::expenseBuild(double value)
{
	this->budget -= value;
}

void Economy::income(double value)
{
	this->budget += value;
	this->netWorth += value;
}
