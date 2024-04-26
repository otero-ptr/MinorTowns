#include "LobbyUpdateNotifier.h"

LobbyUpdateNotifier::LobbyUpdateNotifier(std::function<std::string()> f) : getLobbyList(f)
{
	this->active = true;
	this->thNotify = std::jthread(&LobbyUpdateNotifier::notify, this);
}

LobbyUpdateNotifier::~LobbyUpdateNotifier()
{
	this->active = false;
	if (this->thNotify.joinable()) {
		this->thNotify.join();
	}
}

void LobbyUpdateNotifier::subscribe(std::shared_ptr<User> user)
{
	this->subscribedUsers.insert(std::make_pair(user->getUUID(), user));
	user->messagePool.pushBackMessage(this->lobbyList);
}

void LobbyUpdateNotifier::unsubscribe(std::shared_ptr<User> user)
{
	auto it = this->subscribedUsers.find(user->getUUID());
	if (it != this->subscribedUsers.end()) {
		this->subscribedUsers.erase(user->getUUID());
	}
}

void LobbyUpdateNotifier::notify()
{
	while (this->active) {
		if (this->lobbyList != this->getLobbyList()) {
			this->lobbyList = this->getLobbyList();
			for (auto& user : this->subscribedUsers) {
				user.second->messagePool.pushBackMessage(this->lobbyList);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
