#pragma once
#include <memory>
#include "../User/User.h"
#include "AuthorizatonValidate/AuthorizationValidate.h"
enum ACTION_STATUS {
	ST_OK,
	ST_ERROR
};

class Middleware {
public:
	Middleware();
	~Middleware();

	std::shared_ptr<User> Authorization(std::string username, std::string address, unsigned short port);
	ACTION_STATUS action(std::string jsonMessage, std::shared_ptr<User> user);
private:
	void joinLobby();
	void createLobby();
	void leaveLobby();
	void joinGame();
	void leaveGame();

	AuthorizatonValidate authValidate;
};