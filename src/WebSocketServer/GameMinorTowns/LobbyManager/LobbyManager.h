#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "LobbyUpdateNotifier/LobbyUpdateNotifier.h"

namespace sw {
	namespace redis {
		class Redis;
	}
}
class User;

class LobbyManager {
public:
	LobbyManager() = delete;
	explicit LobbyManager(int cooldown_refresher, std::string redis_uri);
	~LobbyManager();
	std::string createLobby(uint8_t count);
	void joinLobby(std::string uuid_lobby, std::shared_ptr<User>& user);
	void leaveLobby(std::shared_ptr<User>& user);
	uint8_t countLobbyUsers(std::string uuid_lobby);
	uint8_t countLobbyMaxUsers(std::string uuid_lobby);
	bool isLobbyFull(std::string uuid_lobby);
	bool isLobbyEmpty(std::string uuid_lobby);
	bool isLobby(std::string uuid_lobby);
	void closeLobby(std::string uuid_lobby);
	std::vector<std::string> getLobbyUsers(std::string uuid_lobby);
	std::vector<std::shared_ptr<User>> extractLobbyUsers(std::string uuid_lobby);
	void subscribeLobbyUpdate(std::shared_ptr<User>& user);
	void unsubscribeLobbyUpdate(std::shared_ptr<User>& user);
private:
	void refreshListLobby(std::stop_token token);
	void notifyLobbyUsers(std::string_view uuid_lobby);
	std::shared_ptr<sw::redis::Redis> redis;
	const int cooldown_refresher;
	std::string list_lobby;
	std::jthread th_refresher;
	std::unique_ptr<LobbyUpdateNotifier> notifier;
	std::unordered_map<std::string, std::weak_ptr<User>> users;
	std::list<std::string> lobbies;
};