#include "LobbyManager.h"
#include "nlohmann/json.hpp"
#include "sw/redis++/redis++.h"
#include "User.h"
#include "gen_uuid.h"
#include "log.h"

LobbyManager::LobbyManager(int cooldown_refresher, std::string redis_uri) :
	redis(std::make_unique<sw::redis::Redis>(redis_uri)),
	cooldown_refresher(cooldown_refresher),
	th_refresher(std::jthread([this](std::stop_token token) { refreshListLobby(token); })),
	notifier(std::make_unique<LobbyUpdateNotifier>(list_lobby)) {

}

LobbyManager::~LobbyManager()
{
	if (th_refresher.joinable()) {
		th_refresher.request_stop();
		th_refresher.join();
	}
}

std::string LobbyManager::createLobby(uint8_t count)
{
	std::string uuid(GenerateUUID());
	redis->hset(uuid, "maxUser", std::to_string(count));
	lobbies.push_back(uuid);
	LOGGER_INFO("Lobby[" + uuid + "]" + " created [" + std::to_string(count) + "users]")
	return uuid;
}

void LobbyManager::joinLobby(std::string uuid_lobby, std::shared_ptr<User>& user)
{
	if (user->getLocation() != Location::MENU) {
		return;
	}
	if (!redis->sismember(uuid_lobby + ":users", user->getUUID())) {
		redis->sadd(uuid_lobby + ":users", user->getUUID());
		users.insert(std::make_pair(user->getUUID(), user));
		user->setLocation(Location::LOBBY, uuid_lobby);
		notifyLobbyUsers(uuid_lobby);
		LOGGER_INFO("User[" + user->getUUID() + "] joined -> lobby[" + uuid_lobby + "]");
	}
}

void LobbyManager::leaveLobby(std::shared_ptr<User>& user)
{
	if (user->getLocation() != Location::LOBBY) {
		return;
	}
	if (redis->sismember(user->getUUIDLocation() + ":users", user->getUUID())) {
		redis->srem(user->getUUIDLocation() + ":users", user->getUUID());
		users.erase(user->getUUID());
		LOGGER_INFO("User[" + user->getUUID() + "] left -> lobby[" + user->getUUIDLocation() + "]");
		user->setLocation(Location::MENU, "menu");
		notifyLobbyUsers(user->getUUIDLocation());
	}
}

bool LobbyManager::isLobbyFull(std::string uuid_lobby)
{
	size_t now_users = countLobbyUsers(uuid_lobby);
	size_t max_users = std::stoi(redis->hget(uuid_lobby, "maxUser").value());
	return now_users >= max_users;
}

bool LobbyManager::isLobbyEmpty(std::string uuid_lobby)
{
	return countLobbyUsers(uuid_lobby) == 0;
}

bool LobbyManager::isLobby(std::string uuid_lobby)
{
	return redis->exists(uuid_lobby);
}

std::vector<std::string> LobbyManager::getLobbyUsers(std::string uuid_lobby)
{
	std::vector<std::string> result;
	std::unordered_set<std::string> set_users;
	redis->smembers(uuid_lobby + ":users", std::inserter(set_users, set_users.begin()));
	for (auto& it : set_users) {
		result.push_back(it);
	}
	return result;
}

std::vector<std::shared_ptr<User>> LobbyManager::extractLobbyUsers(std::string uuid_lobby)
{
	std::vector<std::shared_ptr<User>> result;
	std::vector<std::string> uuid_users = getLobbyUsers(uuid_lobby);
	result.reserve(uuid_users.size());
	for (std::string_view uuid_user : uuid_users) {
		auto iter = users.find(uuid_user.data());
		if (iter != users.end()) {
			auto user = users[uuid_user.data()];
			if (!user.expired()) {
				result.push_back(user.lock());
			}
		}
	}
	return result;
}

void LobbyManager::subscribeLobbyUpdate(std::shared_ptr<User>& user)
{
	notifier->subscribe(user);
}

void LobbyManager::unsubscribeLobbyUpdate(std::shared_ptr<User>& user)
{
	notifier->unsubscribe(user);
}

void LobbyManager::closeLobby(std::string uuid_lobby)
{
	if (redis->exists(uuid_lobby)) {
		notifyLobbyUsers(uuid_lobby);
		if (countLobbyUsers(uuid_lobby) > 0) {
			for (std::string_view it : getLobbyUsers(uuid_lobby)) {
				users.erase(it.data());
			}
		}
		redis->del(uuid_lobby);
		redis->del(uuid_lobby + ":users");
		lobbies.remove(uuid_lobby);
		LOGGER_INFO("Lobby[" + uuid_lobby + "]" + " closed");
	}
}

uint8_t LobbyManager::countLobbyUsers(std::string uuid_lobby)
{
	return redis->scard(uuid_lobby + ":users");
}

uint8_t LobbyManager::countLobbyMaxUsers(std::string uuid_lobby)
{
	return std::stoi(redis->hget(uuid_lobby, "maxUser").value());
}

void LobbyManager::refreshListLobby(std::stop_token token)
{
	while (!token.stop_requested()) {
		nlohmann::json json_list;
		json_list["lobbies"] = nullptr; // init lobbies
		for (std::string_view it : lobbies) {
			nlohmann::json json_lobby;
			json_lobby["uuid_lobby"] = it;
			json_lobby["users_now"] = countLobbyUsers(it.data());
			json_lobby["users_max"] = countLobbyMaxUsers(it.data());
			json_list["lobbies"].push_back(json_lobby);
		}
		list_lobby = json_list.dump();
		std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_refresher));
	}
}

void LobbyManager::notifyLobbyUsers(std::string_view uuid_lobby)
{
	nlohmann::json lobby_data;
	lobby_data["lobby"]["uuid_lobby"] = uuid_lobby;
	lobby_data["lobby"]["users_now"] = countLobbyUsers(uuid_lobby.data());
	lobby_data["lobby"]["users_max"] = countLobbyMaxUsers(uuid_lobby.data());
	auto result = getLobbyUsers(uuid_lobby.data());
	size_t index = 0;
	for (std::string_view uuid : result) {
		auto user = users[uuid.data()].lock();
		if (user) {
			lobby_data["lobby"]["users"][index++] = user->getUsername();
		}
	}
	for (std::string_view uuid : result) {
		auto user = users[uuid.data()].lock();
		if (user) {
			user->message_pool.push(lobby_data.dump());
		}
	}
}
