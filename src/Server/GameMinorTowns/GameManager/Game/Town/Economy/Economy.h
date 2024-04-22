#pragma once

class Economy {
public:
	Economy() = delete;
	Economy(double startBudget = 0.0, double tickIncome = 0.0);
	~Economy();
	const double getBudget() const;
	const double getNetWorth() const;
	const double getTickIncome() const;
	const double getMultiplier() const;
	void setTickIncome(double value);
	void setMultiplier(double value);
	void expenseBuild(double value);
	void income(double value);
private:
	double budget;
	double netWorth;
	double tickIncome;
	double multiplier = 1.0;
};