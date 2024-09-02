#include "Economy.h"

Economy::Economy(int32_t start_budget, int32_t tick_income, float start_multiplier)
	: budget(start_budget), net_worth(start_budget),
	income(tick_income), multiplier(start_multiplier)
{

}

Economy::~Economy()
{
}

const double Economy::getBudget() const
{
	return budget;
}

const double Economy::getNetWorth() const
{
	return net_worth;
}

const int32_t Economy::getIncome() const
{
	return income;
}

const float Economy::getMultiplier() const
{
	return multiplier;
}

void Economy::setIncome(int32_t value)
{
	income = value;
}

void Economy::setMultiplier(float value)
{
	multiplier = value;
}

void Economy::expense(int32_t value)
{
	budget -= value;
}

void Economy::add(int32_t value)
{
	budget += value;
	net_worth += value;
}

void Economy::sub(int32_t value)
{
	budget -= value;
	net_worth -= value;
}