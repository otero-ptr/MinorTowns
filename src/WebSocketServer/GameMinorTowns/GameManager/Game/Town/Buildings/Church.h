#pragma once
#include "Buildings.h"
#include <cmath>

class Church : public Buildings {
public:
	Church(float step_multiplier, float increase_modifier, uint32_t price )
	: Buildings(price), multiplier(1.0),
		step_multiplier(step_multiplier),
		increase_modifier(increase_modifier) {

	}
	virtual void build() override{
		std::lock_guard<std::mutex> lock(mtx);
		price = static_cast<uint32_t>(std::ceil(price * (1 + increase_modifier))); // potential problem
		multiplier += step_multiplier;
		++count;
	}
	virtual void destroy() override {
		std::lock_guard<std::mutex> lock(mtx);
	}
	const float getModifier() const {
		return multiplier;
	}
private:
	float multiplier;
	const float step_multiplier;
	const float increase_modifier;
};