#pragma once
#include "User\User.h"
#include <memory>
#include <thread>
#include <unordered_map>
#include <mutex>

class LobbyUpdateNotifier {
public:
	LobbyUpdateNotifier() = delete;
	explicit LobbyUpdateNotifier(std::string& lobby_list);
	~LobbyUpdateNotifier();
	void subscribe(std::shared_ptr<User>& user);
	void unsubscribe(std::shared_ptr<User>& user);
private:
	void notify(std::stop_token token);
	std::string& lobby_list;
	std::string lobby_list_old;
	std::unordered_map<std::string, std::weak_ptr<User>> users;
	std::jthread th_notify;
	std::mutex mtx;
};