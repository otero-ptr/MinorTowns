#pragma once
#include <memory>
#include <vector>
#include <string>

class Game {
public:
	Game() = delete;
	Game(std::vector<std::shared_ptr<User>> users);
	~Game() {}
	const std::string getUUIDgit();
private:
	std::vector<std::shared_ptr<User>> users;
	std::string uuid;
};