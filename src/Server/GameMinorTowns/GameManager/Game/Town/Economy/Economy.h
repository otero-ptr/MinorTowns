#pragma once

class Economy {
public:
	Economy();
	Economy(int startBudget);
	~Economy();
	const int getBudget();
	const int getNetWorth();
	void expense(int value);
	void income(int value);
private:
	int budget = 0;
	int netWorth = 0;
};