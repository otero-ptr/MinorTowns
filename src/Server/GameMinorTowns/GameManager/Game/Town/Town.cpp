#include "Town.h"
#include <iostream>

Town::Town(std::shared_ptr<User> user, int capitalNode) : economy(100.0, 10.0)
{
	this->own = user;
	this->capitalNode = capitalNode;
	this->BuildingsInit();
}

Town::~Town()
{
}

const int Town::getCapitalNodeId()
{
	return this->capitalNode;
}

void Town::TownTickProcessing()
{
	this->economy.income(this->economy.getTickIncome() * this->economy.getMultiplier());
}

void Town::buildBuilding(const int& buildingType)
{
	if (this->buildings.isTypeExists(buildingType)) {
		if (this->economy.getBudget() >= this->buildings.getPriceBuildings(buildingType)) {
			this->economy.expenseBuild(this->buildings.getPriceBuildings(buildingType));
			this->buildings.build(buildingType);
			this->buildings.setPriceBuildings(buildingType, this->buildings.getPriceBuildings(buildingType) + this->buildings.getPriceBuildings(buildingType) / 2); // first price + first price / 2
			if (buildingType == TypeBuilding::Church) {
				this->economy.setMultiplier(1.0 + this->buildings.getCountBuildings(TypeBuilding::Church) * 0.1); // church multiplier 0.1
			}
			else if (buildingType == TypeBuilding::Manufactory) {
				this->economy.setTickIncome(this->economy.getTickIncome() + 10); // manufactory income 10
			}
		}
	}
}

void Town::destroyBuilding(const int& buildingType)
{
	if (this->buildings.isTypeExists(buildingType)) {
		if (0 < this->buildings.getCountBuildings(buildingType)) {
			this->buildings.destroy(buildingType);
		}
	}
}

void Town::BuildingsInit()
{
	this->buildings.setPriceBuildings(0, 70.0); //church
	this->buildings.setPriceBuildings(1, 30.0); //manufactory
}
