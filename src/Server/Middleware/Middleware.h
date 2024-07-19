#pragma once
#include <memory>
#include "User/User.h"
#include "AuthorizatonValidate/AuthorizationValidate.h"
#include "request_handler\request_result.h"

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
	std::pair<MIDDLEWARE_STATUS, std::string> action(RequestResult request_result, std::shared_ptr<User> user);
	MIDDLEWARE_STATUS disconnect(std::shared_ptr<User> user);
private:
	AuthorizatonValidate auth_validate;
	std::shared_ptr<GameMinorTowns> game_minor_towns;
};