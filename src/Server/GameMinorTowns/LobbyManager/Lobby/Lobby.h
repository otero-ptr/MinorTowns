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
	bool isEmpty();
	std::vector<std::shared_ptr<User>> getUsers();
	const std::string getUUID();
	const int getNowUsers();
	const int getMaxUsers();

	std::string getLobbyData();
private:
	void createLobbyData();
	void createUUID();
	int maxUser;
	std::vector<std::shared_ptr<User>> users;

	std::string uuid;
	std::string lobbyData;
};