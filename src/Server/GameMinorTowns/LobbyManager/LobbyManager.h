#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "User/User.h"
#include "LobbyUpdateNotifier\LobbyUpdateNotifier.h"

namespace sw {
	namespace redis {
		class Redis;
	}
}

class LobbyManager {
public:
	LobbyManager() = delete;
	LobbyManager(int cooldown_refresher, std::string redis_uri);
	~LobbyManager();
	std::string createLobby(size_t count);
	void joinLobby(std::string uuid_lobby, std::shared_ptr<User> user);
	void leaveLobby(std::shared_ptr<User> user);
	size_t countLobbyUsers(std::string uuid_lobby);
	size_t countLobbyMaxUsers(std::string uuid_lobby);
	bool isLobbyFull(std::string uuid_lobby);
	bool isLobbyEmpty(std::string uuid_lobby);
	void closeLobby(std::string uuid_lobby);
	std::vector<std::string> getLobbyUsers(std::string uuid_lobby);
	std::vector<std::shared_ptr<User>> extractLobbyUsers(std::string uuid_lobby);
	void subscribeLobbyUpdate(std::shared_ptr<User> user);
	void unsubscribeLobbyUpdate(std::shared_ptr<User> user);
private:
	void refreshListLobby();
	void notifyLobbyUsers(std::string_view uuid_lobby);
	std::shared_ptr<sw::redis::Redis> redis;
	bool active;
	const int cooldown_refresher;
	std::string list_lobby;
	std::jthread th_refresher;
	std::unique_ptr<LobbyUpdateNotifier> notifier;
	std::unordered_map<std::string, std::shared_ptr<User>> users;
	std::list<std::string> lobbies;
};