#pragma once
#include <memory>
#include "../User/User.h"

enum ACTION_STATUS {
	ST_OK,
	ST_ERROR
};

class Middleware {
public:
	Middleware();
	~Middleware();

	std::shared_ptr<User> Authourzation(std::string username, std::string address, std::string port);
	ACTION_STATUS action(std::string jsonMessage, std::shared_ptr<User> user);
private:
	bool isJson(const std::string& str);
	void joinLobby();
	void createLobby();
	void leaveLobby();
	void joinGame();
	void leaveGame();
};