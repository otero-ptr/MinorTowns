#pragma once
#include <memory>
#include "Economy\Economy.h"
#include "Buildings\Buildings.h"
#include "User\User.h"

class Town {
public:
	Town() = delete;
	Town(std::shared_ptr<User> user, int capitalNode);
	~Town();
	const int getCapitalNodeId();
	void TownTickProcessing();
	void buildBuilding(const int &buildingType);
	void destroyBuilding(const int &buildingType);
//private:
	void BuildingsInit();
	int capitalNode;
	Economy economy;
	Buildings buildings; 
	std::shared_ptr<User> own;
};