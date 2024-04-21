#pragma once

class Economy {
public:
	Economy() = delete;
	Economy(double startBudget = 0.0, double tickIncome = 0.0);
	~Economy();
	const double getBudget();
	const double getNetWorth();
	const double getTickIncome();
	const double getMultiplier();
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