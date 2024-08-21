#pragma once
#include "Buildings.h"
#include <cmath>

class Manufactory : public Buildings {
public:
	Manufactory(uint32_t step_income, float increase_modifier, uint32_t price)
		: Buildings(price), income(0),
		step_income(step_income),
		increase_modifier(increase_modifier) {

	}
	virtual void build() override {
		std::lock_guard<std::mutex> lock(mtx);
		price = static_cast<uint32_t>(std::ceil(price * (1 + increase_modifier))); // potential problem
		income += step_income;
		++count;
	}
	virtual void destroy() override {
		std::lock_guard<std::mutex> lock(mtx);
	}
	const uint32_t getIncome() const {
		return income;
	}
private:
	uint32_t income;
	const uint32_t step_income;
	const float increase_modifier;
};