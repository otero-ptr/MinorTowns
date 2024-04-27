#include "GameSettings.h"
#include "yaml-cpp\yaml.h"

GameSettings::GameSettings() {
	YAML::Node config = YAML::LoadFile("game.yml");
	this->maxTick = config["game"]["max_tick"].as<int>();
	this->repeatTick = config["game"]["repeat_tick"].as<int>();
	this->cooldownTick = config["game"]["tick_cooldown"].as<int>();

	this->economy.startBudget = config["game"]["economy"]["start_budget"].as<double>();
	this->economy.startIncome = config["game"]["economy"]["start_income"].as<double>();
	this->economy.startMultiplier = config["game"]["economy"]["start_multiplier"].as<double>();

	this->buildings.priceIncrease = config["game"]["buildings"]["price_increase"].as<double>();
	this->buildings.priceChurch = config["game"]["buildings"]["price_church"].as<int>();
	this->buildings.priceManufactory = config["game"]["buildings"]["price_manufactory"].as<int>();
	this->buildings.modifierChurch = config["game"]["buildings"]["modifier_church"].as<double>();
	this->buildings.modifierManufactory = config["game"]["buildings"]["modifier_manufactory"].as<int>();
}
