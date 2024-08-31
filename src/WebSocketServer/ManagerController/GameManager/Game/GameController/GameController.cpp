#include "GameController.h"
#include "GameNotify.h"
#include "Town/Town.h"
#include "User.h"
#include <vector>

GameController::GameController(const int max_tick, const int max_repeat_tick)
	: max_tick(max_tick), max_repeat_tick(max_repeat_tick) {

}

GameController::~GameController()
{

}

void GameController::control(const std::vector<std::pair<std::shared_ptr<User>, std::shared_ptr<Town>>> &leaderboard)
{
	++tick;
	if (!game_end) {
		if (leaderboard.size() == 1) {
			contender_id = leaderboard.front().second->getID();
			game_end = true;
			for (auto& it : leaderboard) {
				it.first->message_pool.push("{\"win_town\": " + std::to_string(contender_id) + "}");
			}
		}
		if (tick == 0) {
			for (auto& it : leaderboard) {
				it.first->message_pool.push("{\"game_phase\": 1}");
			}
		}
		if (tick == max_tick) {
			for (auto& it : leaderboard) {
				it.first->message_pool.push("{\"game_phase\": 2}");
			}
		}
		if (tick >= max_tick) {
			if (contender_id == -1 || leaderboard.front().second->getID() != contender_id) {
				contender_id = leaderboard.front().second->getID();
				repeat_tick = 0;
				for (auto& it : leaderboard) {
					it.first->message_pool.push("{\"contender_town\": " + std::to_string(contender_id) + "}");
				}
			}
			if (++repeat_tick > max_repeat_tick) {
				game_end = true;
				for (auto& it : leaderboard) {
					it.first->message_pool.push("{\"win_town\": " + std::to_string(contender_id) + "}");
				}
			}
			if (tick > max_tick*2) {
				game_end = true;
				for (auto& it : leaderboard) {
					it.first->message_pool.push("{\"win_town\": " + std::to_string(contender_id) + "}");
				}
			}
		}
	}

}

void GameController::notify(const std::vector<std::pair<std::shared_ptr<User>, std::shared_ptr<Town>>>& leaderboard, GameNotify&& game_notify)
{
	for (auto& [user_place, town_place] : leaderboard) {
		user_place->message_pool.push("{\"game_notify\": " + std::to_string(static_cast<int>(game_notify)) + "}");
	}
}

void GameController::notify(const std::vector<std::shared_ptr<User>>& users, GameNotify&& game_notify)
{
	for (auto& user : users) {
		user->message_pool.push("{\"game_notify\": " + std::to_string(static_cast<int>(game_notify)) + "}");
	}
}


bool GameController::isEnd() const
{
	return game_end;
}

int GameController::getTick()
{
	return tick;
}
