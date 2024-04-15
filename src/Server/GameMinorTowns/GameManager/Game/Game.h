#pragma once
#include <vector>
#include "User/User.h"

class Game {
public:
	Game() = delete;
	Game(std::vector<std::shared_ptr<User>> users){}
	~Game() {}
private:
};