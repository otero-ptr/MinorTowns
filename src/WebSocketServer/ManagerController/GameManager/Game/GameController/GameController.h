#pragma once
#include "GameNotify.h"
#include <memory>
#include <vector>
#include "Town.h"

class GameController {
public:
	GameController() = delete;
	GameController(const int max_tick,const int max_repeat_tick);
	~GameController();
	void control(const std::vector<std::pair<std::shared_ptr<User> ,std::shared_ptr<Town>>>& leaderboard);
	void notify(const std::vector<std::shared_ptr<User>>& users, GameNotify&& game_notify);
	void notify(const std::vector<std::pair<std::shared_ptr<User>, std::shared_ptr<Town>>>& leaderboard, GameNotify&& game_notify);
	bool isEnd() const;
	int getTick();
private:
	const int max_tick;
	int tick = 0;
	const int max_repeat_tick;
	int repeat_tick = 0;
	bool game_end = false;
	uint8_t contender_id = UINT8_MAX;
};