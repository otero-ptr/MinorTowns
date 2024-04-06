#pragma once
#include <vector>
#include <map>
#include <memory>

class User;
class Game;

class GameManager {
public:
	GameManager();
	~GameManager();
	void createGame(std::vector<User> users);
	void getGame() {}
private:
	std::map<int, std::unique_ptr<Game>> games;
};