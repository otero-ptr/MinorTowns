#pragma once
#include <memory>
#include <vector>
#include <string>

class User;

class Lobby {
public:
	Lobby() = delete;
	Lobby(int maxUser);
	~Lobby();
	void join(std::shared_ptr<User> user);
	void leave(std::shared_ptr<User> user);
	bool isFull();
	std::vector<std::shared_ptr<User>>& getUsers();
	const std::string getUnique();
	const int getNowUsers();
	const int getMaxUsers();
private:
	int maxUser;
	std::vector<std::shared_ptr<User>> users;
	std::string unique; // uuid
};