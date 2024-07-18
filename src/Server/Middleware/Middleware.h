#pragma once
#include <memory>
#include "User/User.h"
#include "AuthorizatonValidate/AuthorizationValidate.h"

class GameMinorTowns;

enum MIDDLEWARE_STATUS {
	ST_OK,
	ST_ERROR
};

class Middleware {
public:
	Middleware(std::shared_ptr<GameMinorTowns> game);
	~Middleware();

	std::shared_ptr<User> authorization(std::string username, std::string address, unsigned short port);
	MIDDLEWARE_STATUS action(std::string json_message, std::shared_ptr<User> user);
	MIDDLEWARE_STATUS disconnect(std::shared_ptr<User> user);
private:
	AuthorizatonValidate auth_validate;
	std::shared_ptr<GameMinorTowns> game_minor_towns;
};