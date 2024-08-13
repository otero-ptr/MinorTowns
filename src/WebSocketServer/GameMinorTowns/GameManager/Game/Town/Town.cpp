#include "Town.h"

Town::Town(int id, std::shared_ptr<User> user, int capital_node, 
	std::unique_ptr<Economy> economy, 
	std::unique_ptr<Charch> charch, 
	std::unique_ptr<Manufactory> manufactory)
	: id(id), capital_node(capital_node),
	economy(std::move(economy)), charch(std::move(charch)), 
	manufactory(std::move(manufactory)), own(user) {

}

Town::~Town()
{
}

const int Town::getCapitalNodeId()
{
	return capital_node;
}

void Town::TownTickProcessing()
{
	economy->add(static_cast<int32_t>(std::ceil(
		economy->getIncome() * economy->getMultiplier()
	))); // potential problem
}

void Town::buildBuilding(const TypeBuildings type_building)
{
	if (type_building == TypeBuildings::Charch) {
		if (economy->getBudget() >= charch->getPrice()) {
			economy->expenseBuild(charch->getPrice());
			charch->build();
			economy->setMultiplier(charch->getModifier());
		}
	} else if (type_building == TypeBuildings::Manufactory) {
		if (economy->getBudget() >= manufactory->getPrice()) {
			economy->expenseBuild(manufactory->getPrice());
			manufactory->build();
			economy->setIncome(manufactory->getIncome());
		}
	}
}

void Town::destroyBuilding(const TypeBuildings type_building)
{
	if (type_building == TypeBuildings::Charch) {
		if (0 < charch->getCount()) {
			charch->destroy();
		}
	}
	else if (type_building == TypeBuildings::Manufactory) {
		if (0 < manufactory->getCount()) {
			manufactory->destroy();
		}
	}
}

std::shared_ptr<User> Town::getOwnTown() const
{
	return own;
}

const int Town::getID() const
{
	return id;
}

bool Town::operator<(const Town& other) const
{
	return economy->getNetWorth() < other.economy->getNetWorth();
}

const TownData Town::getData() const
{
	TownData data;
	data.town_id = id;
	data.username = own->getUsername();
	data.economy.budget = economy->getBudget();
	data.economy.net_worth = economy->getNetWorth();
	data.economy.income = economy->getIncome();
	data.economy.multiplier = economy->getMultiplier();

	data.charch.count = charch->getCount();
	data.charch.price = charch->getPrice();
	data.charch.bonus = charch->getModifier();

	data.manufactory.count = manufactory->getCount();
	data.manufactory.price = manufactory->getPrice();
	data.manufactory.bonus = manufactory->getIncome();

	data.army.cost = 0;
	data.army.count_soldiers = 0;
	data.army.locate_node = 0;
	return data;
}