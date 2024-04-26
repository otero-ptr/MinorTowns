#pragma once
#include "GameMinorTowns\GameManager\Game\Town\Town.h"

class GameController {
public:
	GameController();
	~GameController();
	void control(const int& tick, const std::vector<Town> &towns);
	bool isGameEnd() const;
private:
	int maxTick = 240;
	int maxRepeatTick= 60;
	int repeatTick = 0;
	const Town* topTown = nullptr;
	bool gameEnd = false;
};