#pragma once
#include <memory>
#include <string>

class IManagerController;
class ManagerController;
class User;
class RequestResult;

enum MIDDLEWARE_STATUS {
	ST_OK,
	ST_ERROR
};

class Middleware {
public:
	explicit Middleware(std::shared_ptr<IManagerController> manager_controller);
	~Middleware();

	std::shared_ptr<User> authorization(std::string username, std::string address, unsigned short port);
	std::pair<MIDDLEWARE_STATUS, std::string> action(RequestResult request_result, std::shared_ptr<User> user);
	MIDDLEWARE_STATUS disconnect(std::shared_ptr<User> user);
private:
	std::shared_ptr<IManagerController> manager_controller;
};