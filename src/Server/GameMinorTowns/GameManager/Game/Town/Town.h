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
	std::shared_ptr<User> getOwnTown();
	const Economy& getTownEconomy();
	const Buildings& getTownBuildings();
private:
	void BuildingsInit();
	int capitalNode;
	Economy economy;
	Buildings buildings; 
	std::shared_ptr<User> own;
};