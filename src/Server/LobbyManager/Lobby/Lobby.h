#pragma once
#include <memory>
#include <vector>

class User;

class Lobby {
public:
	Lobby() = delete;
	Lobby(int countUser);
	~Lobby();
	void join(std::shared_ptr<User> user);
	void leave(std::shared_ptr<User> user);
	bool isFull();
	std::vector<std::shared_ptr<User>>& getUsers();
private:
	int countUser;
	std::vector<std::shared_ptr<User>> users;
};