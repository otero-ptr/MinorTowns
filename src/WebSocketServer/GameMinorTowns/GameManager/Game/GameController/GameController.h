#pragma once
#include "GameNotify.h"
#include <memory>
#include <vector>
#include "Town.h"
//class Town;

class GameController {
public:
	GameController() = delete;
	GameController(const int max_tick,const int max_repeat_tick);
	~GameController();
	void control(const int& tick, const std::vector<Town> &towns);
	void notify(const std::vector<std::weak_ptr<User>>& users, GameNotify&& game_notify);
	bool isGameEnd() const;
private:
	const int max_tick;
	const int max_repeat_tick;
	int repeat_tick = 0;
	bool game_start = false;
	bool game_end = false;
	int8_t contender_id = -1;
};