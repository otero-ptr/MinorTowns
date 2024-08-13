#include "GameSettings.h"
#include "yaml-cpp\yaml.h"

GameSettings::GameSettings() {
	YAML::Node config = YAML::LoadFile("game.yml");
	max_tick = config["game"]["max_tick"].as<int>();
	repeat_tick = config["game"]["repeat_tick"].as<int>();
	cooldown_tick = config["game"]["tick_cooldown"].as<int>();
	economy.start_budget = config["game"]["economy"]["start_budget"].as<double>();
	economy.start_income = config["game"]["economy"]["start_income"].as<int32_t>();
	economy.start_multiplier = config["game"]["economy"]["start_multiplier"].as<float>();
	buildings.price_increase = config["game"]["buildings"]["price_increase"].as<float>();
	buildings.price_church = config["game"]["buildings"]["price_church"].as<int>();
	buildings.price_manufactory = config["game"]["buildings"]["price_manufactory"].as<int>();
	buildings.modifier_church = config["game"]["buildings"]["modifier_church"].as<float>();
	buildings.modifier_manufactory = config["game"]["buildings"]["modifier_manufactory"].as<int>();
}

std::unique_ptr<Economy> GameSettings::makeEconomy() {
	return std::make_unique<Economy>(economy.start_budget,
		economy.start_income,
		economy.start_multiplier);
}

std::unique_ptr<Charch> GameSettings::makeCharch()
{
	return std::make_unique<Charch>(buildings.modifier_church, buildings.price_increase, buildings.price_church);
}

std::unique_ptr<Manufactory> GameSettings::makeManufactory()
{
	return std::make_unique<Manufactory>(buildings.modifier_manufactory, buildings.price_increase, buildings.price_manufactory);
}

int GameSettings::getCooldownTick()
{
	return cooldown_tick;
}

int GameSettings::getMaxTick()
{
	return max_tick;
}

int GameSettings::getRepeatTick()
{
	return repeat_tick;
}
