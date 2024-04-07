#pragma once
#include <vector>
#include <map>
#include <memory>
#include "../User/User.h"
#include "Game/Game.h"

class GameManager {
public:
	GameManager();
	~GameManager();
	void createGame(std::vector<User> users);
	void getGame() {}
private:
	std::map<int, std::unique_ptr<Game>> games;
};