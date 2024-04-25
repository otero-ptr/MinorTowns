#include "GameController.h"

GameController::GameController()
{

}

GameController::~GameController()
{

}

void GameController::control(const int& tick, const std::vector<Town>& towns)
{
	if (!this->gameEnd) {
		if (towns.size() == 1) {
			this->topTown = &towns[0];
			this->gameEnd = true;
			this->topTown->getOwnTown()->messagePool.pushBackMessage("{\"win_town\": " + std::to_string(this->topTown->getID()) + "}");
		}
		if (tick == 0) {
			for (auto& it : towns) {
				it.getOwnTown()->messagePool.pushBackMessage("{\"game_phase\": 1}");
			}
		}
		if (tick == this->maxTick) {
			for (auto& it : towns) {
				it.getOwnTown()->messagePool.pushBackMessage("{\"game_phase\": 2}");
			}
		}
		if (tick >= this->maxTick) {
			if (this->topTown == nullptr || towns[0].getID() != this->topTown->getID()) {
				this->topTown = &towns[0];
				this->repeatTick = 0;
				for (auto& it : towns) {
					it.getOwnTown()->messagePool.pushBackMessage("{\"contender_town\": " + std::to_string(this->topTown->getID()) + "}");
				}
			}
			if (++this->repeatTick > this->maxRepeatTick) {
				this->gameEnd = true;
				for (auto& it : towns) {
					it.getOwnTown()->messagePool.pushBackMessage("{\"win_town\": " + std::to_string(this->topTown->getID()) + "}");
				}
			}
			if (tick > this->maxTick*2) {
				this->gameEnd = true;
				for (auto& it : towns) {
					it.getOwnTown()->messagePool.pushBackMessage("{\"win_town\": " + std::to_string(this->topTown->getID()) + "}");
				}
			}
		}
	}

}

bool GameController::isGameEnd() const
{
	return this->gameEnd;
}