#pragma once

struct GameSettings {
	GameSettings();
	int maxTick;
	int repeatTick;
	int cooldownTick;
	
	struct Economy {
		double startBudget;
		double startIncome;
		double startMultiplier;
	} economy;

	struct Buildings {
		double priceIncrease;
		int priceChurch;
		int priceManufactory;
		double modifierChurch;
		int modifierManufactory;
	} buildings;
};