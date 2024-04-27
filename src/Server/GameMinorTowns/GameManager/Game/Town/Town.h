#pragma once
#include <memory>
#include "Economy\Economy.h"
#include "Buildings\Buildings.h"
#include "User\User.h"

class Town {
public:
	Town() = delete;
	Town(int townId, std::shared_ptr<User> user, int capitalNode, Economy economy, Buildings buildings);
	~Town();
	const int getCapitalNodeId();
	void TownTickProcessing();
	void buildBuilding(const int &buildingType);
	void destroyBuilding(const int &buildingType);
	std::shared_ptr<User> getOwnTown() const;
	const Economy& getTownEconomy();
	const Buildings& getTownBuildings();
	const int getID() const;
	bool operator<(const Town& other) const;
private:
	int capitalNode;
	int id;
	Economy economy;
	Buildings buildings; 
	std::shared_ptr<User> own;
};