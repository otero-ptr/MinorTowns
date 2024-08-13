#include "GameController.h"
#include "GameNotify.h"
#include <string>

GameController::GameController(const int max_tick, const int max_repeat_tick)
	: max_tick(max_tick), max_repeat_tick(max_repeat_tick) {

}

GameController::~GameController()
{

}

void GameController::control(const int& tick, const std::vector<Town>& towns)
{
	if (!this->game_end) {
		if (towns.size() == 1) {
			this->top_town = &towns[0];
			this->game_end = true;
			this->top_town->getOwnTown()->message_pool.push("{\"win_town\": " + std::to_string(this->top_town->getID()) + "}");
		}
		if (tick == 0) {
			for (auto& it : towns) {
				it.getOwnTown()->message_pool.push("{\"game_phase\": 1}");
			}
		}
		if (tick == this->max_tick) {
			for (auto& it : towns) {
				it.getOwnTown()->message_pool.push("{\"game_phase\": 2}");
			}
		}
		if (tick >= this->max_tick) {
			if (this->top_town == nullptr || towns[0].getID() != this->top_town->getID()) {
				this->top_town = &towns[0];
				this->repeat_tick = 0;
				for (auto& it : towns) {
					it.getOwnTown()->message_pool.push("{\"contender_town\": " + std::to_string(this->top_town->getID()) + "}");
				}
			}
			if (++this->repeat_tick > this->max_repeat_tick) {
				this->game_end = true;
				for (auto& it : towns) {
					it.getOwnTown()->message_pool.push("{\"win_town\": " + std::to_string(this->top_town->getID()) + "}");
				}
			}
			if (tick > this->max_tick*2) {
				this->game_end = true;
				for (auto& it : towns) {
					it.getOwnTown()->message_pool.push("{\"win_town\": " + std::to_string(this->top_town->getID()) + "}");
				}
			}
		}
	}

}

void GameController::notify(const std::vector<std::weak_ptr<User>>& users, GameNotify&& game_notify)
{
	for (auto& it : users) {
		if (auto user = it.lock()) {
			user->message_pool.push("{\"game_notify\": " + std::to_string(static_cast<int>(game_notify)) + "}");
		}
	}
}

bool GameController::isGameEnd() const
{
	return this->game_end;
}