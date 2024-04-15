#include <iostream>
#include "Server/Server.h"
#include "GameMinorTowns\GameManager\Game\Game.h"
#include "User\User.h"


int main() {
	Server serv;
	//serv.run();
	std::vector<std::shared_ptr<User>> users;
	std::shared_ptr<User> user1 = std::make_shared<User>("user1", "proxy");
	std::shared_ptr<User> user2 = std::make_shared<User>("user2", "proxy");
	std::shared_ptr<User> user3 = std::make_shared<User>("user3", "proxy");
	std::shared_ptr<User> user4 = std::make_shared<User>("user4", "proxy");
	users.push_back(user1);
	users.push_back(user2);
	users.push_back(user3);
	users.push_back(user4);
	Game game(users);
	return 0;
}