#pragma once
#include <cstdint>

class Economy {
public:
	Economy() = delete;
	Economy(double start_budget = 0.0, int32_t income = 0, float start_multiplier = 1.0);
	~Economy();
	const double getBudget() const;
	const double getNetWorth() const;
	const int32_t getIncome() const;
	const float getMultiplier() const;
	void setIncome(int32_t value);
	void setMultiplier(float value);
	void expenseBuild(double value);
	void add(int32_t value);
	void sub(int32_t value);
private:
	double budget;
	double net_worth;
	float multiplier;
	int32_t income; // income can be negative, so need fix this, double only <=0
};