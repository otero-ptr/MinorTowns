#pragma once
#include <memory>
#include "User\User.h"
#include <thread>
#include <unordered_map>
#include <functional>

class LobbyUpdateNotifier {
public:
	LobbyUpdateNotifier() = delete;
	LobbyUpdateNotifier(std::function<std::string()> f);
	~LobbyUpdateNotifier();
	void subscribe(std::shared_ptr<User> user);
	void unsubscribe(std::shared_ptr<User> user);
private:
	void notify();
	std::function<std::string()> getLobbyList;
	std::string lobbyList;
	std::unordered_map<std::string, std::shared_ptr<User>> subscribedUsers;
	bool active;
	std::jthread thNotify;
};