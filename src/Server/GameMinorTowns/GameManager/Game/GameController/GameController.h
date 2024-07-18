#pragma once
#include "GameMinorTowns\GameManager\Game\Town\Town.h"

class GameController {
public:
	GameController();
	~GameController();
	void control(const int& tick, const std::vector<Town> &towns);
	bool isGameEnd() const;
private:
	int max_tick = 240;
	int max_repeat_tick= 60;
	int repeat_tick = 0;
	const Town* top_town = nullptr;
	bool game_end = false;
};