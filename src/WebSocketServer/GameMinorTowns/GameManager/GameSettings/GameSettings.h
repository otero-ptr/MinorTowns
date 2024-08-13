#pragma once
#include "GameMinorTowns\GameManager\Game\Town\Buildings\Charch.h"
#include "GameMinorTowns\GameManager\Game\Town\Buildings\Manufactory.h"
#include "GameMinorTowns\GameManager\Game\Town\Economy\Economy.h"
class GameSettings {
public:
	GameSettings();
	std::unique_ptr<Economy> makeEconomy();
	std::unique_ptr<Charch> makeCharch();
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