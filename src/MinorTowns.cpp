#include <iostream>
#include "Server/Server.h"


int main() {
	Server serv;
	serv.run();
	return 0;
}


/* for game test
	#include "GameMinorTowns\GameManager\Game\Game.h"
 	std::shared_ptr<User> user1 = std::make_shared<User>("user1", "address");
	std::shared_ptr<User> user2 = std::make_shared<User>("user2", "address");
	std::shared_ptr<User> user3 = std::make_shared<User>("user3", "address");
	std::shared_ptr<User> user4 = std::make_shared<User>("user4", "address");
	std::vector< std::shared_ptr<User>> users;
	users.push_back(user1);
	users.push_back(user2);
	users.push_back(user3);
	users.push_back(user4);
	Game game(users);
*/