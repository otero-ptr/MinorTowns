#include "Economy.h"

Economy::Economy(double startBudget, double tickIncome, double startMultiplier)
{
	this->income(startBudget);
	this->tickIncome = tickIncome;
	this->setMultiplier(startMultiplier);
}

Economy::~Economy()
{
}

const double Economy::getBudget() const
{
	return this->budget;
}

const double Economy::getNetWorth() const
{
	return this->netWorth;
}

const double Economy::getTickIncome() const
{
	return this->tickIncome;
}

const double Economy::getMultiplier() const
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
