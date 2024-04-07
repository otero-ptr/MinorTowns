#pragma once
#include <memory>
#include <vector>
#include <string>

class User;

struct LobbyInfo {
	LobbyInfo(const std::string& unique, const int& max, const int& now) : unique(unique), max(max), now(now) {}
	const std::string unique;
	const int max;
	const int now;
};


class Lobby {
public:
	Lobby() = delete;
	Lobby(int countUser);
	~Lobby();
	void join(std::shared_ptr<User> user);
	void leave(std::shared_ptr<User> user);
	bool isFull();
	std::vector<std::shared_ptr<User>>& getUsers();
	const std::string getUnique();
	LobbyInfo getLobbyInfo();
private:
	int countUser;
	std::vector<std::shared_ptr<User>> users;
	std::string unique; // uuid
};