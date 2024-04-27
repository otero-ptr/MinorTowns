#include "Town.h"

Town::Town(int townId, std::shared_ptr<User> user, int capitalNode, Economy economy, Buildings buildings) : economy(economy), buildings(buildings)
{
	this->id = townId;
	this->own = user;
	this->capitalNode = capitalNode;
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
			this->buildings.setPriceBuildings(buildingType, this->buildings.getPriceBuildings(buildingType) * this->buildings.getPriceIncrease(), this->buildings.getModifierBuildings(buildingType));
			if (buildingType == TypeBuilding::Church) {
				this->economy.setMultiplier(this->economy.getMultiplier() + this->buildings.getCountBuildings(TypeBuilding::Church) * this->buildings.getModifierBuildings(buildingType)); // church multiplier 0.1
			}
			else if (buildingType == TypeBuilding::Manufactory) {
				this->economy.setTickIncome(this->economy.getTickIncome() + this->buildings.getModifierBuildings(buildingType)); // manufactory income 10
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

std::shared_ptr<User> Town::getOwnTown() const
{
	return this->own;
}

const Economy& Town::getTownEconomy()
{
	return this->economy;
}

const Buildings& Town::getTownBuildings()
{
	return this->buildings;
}

const int Town::getID() const
{
	return this->id;
}

bool Town::operator<(const Town& other) const
{
	return this->economy.getNetWorth() < other.economy.getNetWorth();
}