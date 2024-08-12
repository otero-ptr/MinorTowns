#include "LobbyManager.h"
#include "nlohmann/json.hpp"
#include "sw\redis++\redis++.h"
#include "util/gen_uuid.h"

LobbyManager::LobbyManager(int cooldown_refresher, std::string redis_uri) :
	redis(std::make_unique<sw::redis::Redis>(redis_uri)),
	active(true), cooldown_refresher(cooldown_refresher),
	th_refresher(std::jthread(&LobbyManager::refreshListLobby, this)),
	notifier(std::make_unique<LobbyUpdateNotifier>(this->list_lobby))

{

}

LobbyManager::~LobbyManager()
{
	this->active = false;
	if (this->th_refresher.joinable()) {
		this->th_refresher.join();
	}
}

std::string LobbyManager::createLobby(size_t count)
{
	std::string uuid(GenerateUUID());
	this->redis->hset(uuid, "maxUser", std::to_string(count));
	this->lobbies.push_back(uuid);
	return uuid;
}

void LobbyManager::joinLobby(std::string uuid_lobby, std::shared_ptr<User> user)
{
	if (user->getLocation() != Location::MENU) {
		return;
	}
	if (!this->redis->sismember(uuid_lobby + ":users", user->getUUID())) {
		this->redis->sadd(uuid_lobby + ":users", user->getUUID());
		this->users.insert(std::make_pair(user->getUUID(), user));
		user->setLocation(Location::LOBBY, uuid_lobby);
		this->notifyLobbyUsers(uuid_lobby);
	}
}

void LobbyManager::leaveLobby(std::shared_ptr<User> user)
{
	if (user->getLocation() != Location::LOBBY) {
		return;
	}
	if (this->redis->sismember(user->getUUIDLocation() + ":users", user->getUUID())) {
		this->redis->srem(user->getUUIDLocation() + ":users", user->getUUID());
		this->users.erase(user->getUUID());
		this->notifyLobbyUsers(user->getUUIDLocation());
		user->setLocation(Location::MENU, "menu");
	}
}

bool LobbyManager::isLobbyFull(std::string uuid_lobby)
{
	size_t now_users = this->countLobbyUsers(uuid_lobby);
	size_t max_users = std::stoi(this->redis->hget(uuid_lobby, "maxUser").value());
	return now_users >= max_users;
}

bool LobbyManager::isLobbyEmpty(std::string uuid_lobby)
{
	return this->countLobbyUsers(uuid_lobby) == 0;
}

bool LobbyManager::isLobby(std::string uuid_lobby)
{
	return this->redis->exists(uuid_lobby);
}

std::vector<std::string> LobbyManager::getLobbyUsers(std::string uuid_lobby)
{
	std::vector<std::string> result;
	std::unordered_set<std::string> set_users;
	this->redis->smembers(uuid_lobby + ":users", std::inserter(set_users, set_users.begin()));
	for (auto& it : set_users) {
		result.push_back(it);
	}
	return result;
}

std::vector<std::shared_ptr<User>> LobbyManager::extractLobbyUsers(std::string uuid_lobby)
{
	std::vector<std::shared_ptr<User>> result;
	std::vector<std::string> uuid_users = this->getLobbyUsers(uuid_lobby);
	result.reserve(uuid_users.size());
	for (std::string_view uuid_user : uuid_users) {
		auto iter = this->users.find(uuid_user.data());
		if (iter != this->users.end()) {
			std::shared_ptr<User> user = this->users[uuid_user.data()];
			result.push_back(user);
		}
	}
	return result;
}

void LobbyManager::subscribeLobbyUpdate(std::shared_ptr<User> user)
{
	this->notifier->subscribe(user);
}

void LobbyManager::unsubscribeLobbyUpdate(std::shared_ptr<User> user)
{
	this->notifier->unsubscribe(user);
}

void LobbyManager::closeLobby(std::string uuid_lobby)
{
	if (this->redis->exists(uuid_lobby)) {
		this->notifyLobbyUsers(uuid_lobby);
		if (this->countLobbyUsers(uuid_lobby) > 0) {
			for (std::string_view it : this->getLobbyUsers(uuid_lobby)) {
				this->users.erase(it.data());
			}
		}
		this->redis->del(uuid_lobby);
		this->redis->del(uuid_lobby + ":users");
		this->lobbies.remove(uuid_lobby);
	}
}

size_t LobbyManager::countLobbyUsers(std::string uuid_lobby)
{
	return this->redis->scard(uuid_lobby + ":users");
}

size_t LobbyManager::countLobbyMaxUsers(std::string uuid_lobby)
{
	return std::stoi(this->redis->hget(uuid_lobby, "maxUser").value());
}

void LobbyManager::refreshListLobby()
{
	while (this->active) {
		nlohmann::json json_list;
		json_list["lobbies"] = nullptr; // init lobbies
		for (std::string_view it : this->lobbies) {
			nlohmann::json json_lobby;
			json_lobby["uuid_lobby"] = it;
			json_lobby["users_now"] = this->countLobbyUsers(it.data());
			json_lobby["users_max"] = this->countLobbyMaxUsers(it.data());
			json_list["lobbies"].push_back(json_lobby);
		}
		this->list_lobby = json_list.dump();
		std::this_thread::sleep_for(std::chrono::milliseconds(this->cooldown_refresher));
	}
}

void LobbyManager::notifyLobbyUsers(std::string_view uuid_lobby)
{
	nlohmann::json lobby_data;
	lobby_data["lobby"]["uuid_lobby"] = uuid_lobby;
	lobby_data["lobby"]["users_now"] = this->countLobbyUsers(uuid_lobby.data());
	lobby_data["lobby"]["users_max"] = this->countLobbyMaxUsers(uuid_lobby.data());
	auto result = this->getLobbyUsers(uuid_lobby.data());
	size_t index = 0;
	for (std::string_view uuid : result) {
		lobby_data["lobby"]["users"][index++] = this->users[uuid.data()]->getUsername();
	}
	for (std::string_view uuid : result) {
		this->users[uuid.data()]->message_pool.push(lobby_data.dump());
	}
}
