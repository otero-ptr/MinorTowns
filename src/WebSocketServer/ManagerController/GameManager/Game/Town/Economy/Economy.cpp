#include "Economy.h"

Economy::Economy(double start_budget, int32_t tick_income, float start_multiplier)
	: budget(start_budget), net_worth(start_budget),
	income(tick_income), multiplier(start_multiplier)
{

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
	return this->net_worth;
}

const int32_t Economy::getIncome() const
{
	return this->income;
}

const float Economy::getMultiplier() const
{
	return this->multiplier;
}

void Economy::setIncome(int32_t value)
{
	this->income = value;
}

void Economy::setMultiplier(float value)
{
	this->multiplier = value;
}

void Economy::expenseBuild(double value)
{
	this->budget -= value;
}

void Economy::add(int32_t value)
{
	this->budget += value;
	this->net_worth += value;
}

void Economy::sub(int32_t value)
{
	this->budget -= value;
	this->net_worth -= value;
}