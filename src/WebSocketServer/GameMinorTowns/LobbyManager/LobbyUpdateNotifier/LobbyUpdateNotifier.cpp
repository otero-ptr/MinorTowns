#include "LobbyUpdateNotifier.h"
#include "User.h"

LobbyUpdateNotifier::LobbyUpdateNotifier(std::string& lobby_list) 
	: lobby_list(lobby_list), lobby_list_old(lobby_list),
	th_notify(std::jthread([this](std::stop_token token) { notify(token); })) {

}

LobbyUpdateNotifier::~LobbyUpdateNotifier()
{
	if (th_notify.joinable()) {
		th_notify.request_stop();
		th_notify.join();
	}
}

void LobbyUpdateNotifier::subscribe(std::shared_ptr<User>& user)
{
	std::lock_guard<std::mutex> lock(mtx);
	auto it = users.find(user->getUUID());
	if (it == users.end()) {
		users.insert(std::make_pair(user->getUUID(), user));
		user->message_pool.push(lobby_list_old);
	}
}

void LobbyUpdateNotifier::unsubscribe(std::shared_ptr<User>& user)
{
	std::lock_guard<std::mutex> lock(mtx);
	auto it = users.find(user->getUUID());
	if (it != users.end()) {
		users.erase(user->getUUID());
	}
}

void LobbyUpdateNotifier::notify(std::stop_token token)
{
	while (!token.stop_requested()) {
		mtx.lock();
		if (lobby_list_old != lobby_list) {;
			lobby_list_old = lobby_list;
			for (auto it = users.begin(); it != users.end();) {
				if (it->second.expired()) {
					it = users.erase(it);
				}
				else {
					auto user = it->second.lock();
					user->message_pool.push(lobby_list);
					++it;
				}
			}
		}
		mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
