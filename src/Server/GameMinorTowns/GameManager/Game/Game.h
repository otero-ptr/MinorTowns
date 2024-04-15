#pragma once
#include <memory>
#include <vector>
#include <string>

class User;

class Game {
public:
	Game() = delete;
	Game(std::vector<std::shared_ptr<User>> users);
	~Game() {}
	const std::string getUUID();
private:
	std::vector<std::shared_ptr<User>> users;
	std::string uuid;
};