#include "Town.h"

Town::Town(uint8_t id, std::unique_ptr<Economy> economy,
	std::unique_ptr<Church> charch, 
	std::unique_ptr<Manufactory> manufactory)
	: id(id), economy(std::move(economy)), 
	church(std::move(charch)), 
	manufactory(std::move(manufactory)){

}

Town::~Town()
{
}

void Town::processing()
{
	int32_t income = static_cast<int32_t>(std::ceil(
		economy->getIncome() * economy->getMultiplier()));
	economy->add(income);
}

void Town::buildBuilding(const TypeBuildings type_building)
{
	if (type_building == TypeBuildings::Church) {
		if (economy->getBudget() >= church->getPrice()) {
			economy->expenseBuild(church->getPrice());
			church->build();
			economy->setMultiplier(church->getModifier());
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
	if (type_building == TypeBuildings::Church) {
		if (0 < church->getCount()) {
			church->destroy();
		}
	}
	else if (type_building == TypeBuildings::Manufactory) {
		if (0 < manufactory->getCount()) {
			manufactory->destroy();
		}
	}
}

bool Town::operator<(const Town& other) const
{
	return economy->getNetWorth() < other.economy->getNetWorth();
}

const uint8_t Town::getID() const {
	return id;
}

const TownDetails Town::getDetails() const
{
	TownDetails data{};
	data.town_id = id;

	data.economy.budget = economy->getBudget();
	data.economy.net_worth = economy->getNetWorth();
	data.economy.income = economy->getIncome();
	data.economy.multiplier = economy->getMultiplier();

	data.charch.count = church->getCount();
	data.charch.price = church->getPrice();
	data.charch.bonus = church->getModifier();

	data.manufactory.count = manufactory->getCount();
	data.manufactory.price = manufactory->getPrice();
	data.manufactory.bonus = manufactory->getIncome();

	data.army.cost = 0;

	return data;
}