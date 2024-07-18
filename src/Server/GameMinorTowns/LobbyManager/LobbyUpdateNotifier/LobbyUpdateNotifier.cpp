#include "LobbyUpdateNotifier.h"

LobbyUpdateNotifier::LobbyUpdateNotifier(std::string& lobby_list) : lobby_list(lobby_list), lobby_list_old(lobby_list)
{
	this->active = true;
	this->th_notify = std::jthread(&LobbyUpdateNotifier::notify, this);
}

LobbyUpdateNotifier::~LobbyUpdateNotifier()
{
	this->active = false;
	if (this->th_notify.joinable()) {
		this->th_notify.join();
	}
}

void LobbyUpdateNotifier::subscribe(std::shared_ptr<User> user)
{
	this->mtx.lock();
	auto it = this->subscribed_users.find(user->getUUID());
	if (it == this->subscribed_users.end()) {
		this->subscribed_users.insert(std::make_pair(user->getUUID(), user));
		user->message_pool.pushBackMessage(this->lobby_list_old);
	}
	this->mtx.unlock();
}

void LobbyUpdateNotifier::unsubscribe(std::shared_ptr<User> user)
{
	this->mtx.lock();
	auto it = this->subscribed_users.find(user->getUUID());
	if (it != this->subscribed_users.end()) {
		this->subscribed_users.erase(user->getUUID());
	}
	this->mtx.unlock();
}

void LobbyUpdateNotifier::notify()
{
	while (this->active) {
		this->mtx.lock();
		if (this->lobby_list_old != this->lobby_list) {;
			this->lobby_list_old = this->lobby_list;
			for (auto it = subscribed_users.begin(); it != subscribed_users.end();) {
				if (it->second.expired()) {
					it = subscribed_users.erase(it);
				}
				else {
					auto user = it->second.lock();
					user->message_pool.pushBackMessage(this->lobby_list);
					++it;
				}
			}
		}
		this->mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
