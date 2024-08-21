#pragma once
#include "Buildings\Church.h"
#include "Buildings\Manufactory.h"
#include "Economy\Economy.h"

class GameSettings {
public:
	GameSettings();
	std::unique_ptr<Economy> makeEconomy();
	std::unique_ptr<Church> makeChurch();
	std::unique_ptr<Manufactory> makeManufactory();
	int getCooldownTick();
	int getMaxTick();
	int getRepeatTick();

private:
	int max_tick;
	int repeat_tick;
	int cooldown_tick;

	struct Economy_ {
		double start_budget;
		int32_t start_income;
		float start_multiplier;
	} economy;

	struct Buildings_ {
		float price_increase;
		uint32_t price_church;
		uint32_t price_manufactory;
		float modifier_church;
		uint32_t modifier_manufactory;
	} buildings;

};