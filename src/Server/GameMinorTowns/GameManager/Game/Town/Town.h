#pragma once
#include <memory>
#include "Economy\Economy.h"
#include "User\User.h"

class Town {
public:
	Town() = delete;
	Town(std::shared_ptr<User> user, int capitalNode);
	~Town();
	const int getCapitalNodeId();
private:
	int capitalNode;
	Economy economy;
	std::shared_ptr<User> own;
};