#pragma once
#include "GameMinorTowns\GameManager\Game\Town\Town.h"
#include "GameNotify.h"

class GameController {
public:
	GameController() = delete;
	GameController(const int max_tick,const int max_repeat_tick);
	~GameController();
	void control(const int& tick, const std::vector<Town> &towns);
	void notify(const std::vector<std::weak_ptr<User>>& users, GameNotify&& game_notify);
	bool isGameEnd() const;
private:
	const Town* top_town = nullptr;
	const int max_tick;
	const int max_repeat_tick;
	int repeat_tick = 0;
	bool game_start = false;
	bool game_end = false;
};