#include "Town.h"

Town::Town(uint8_t id, std::unique_ptr<Economy> economy,
	std::unique_ptr<Church> charch, 
	std::unique_ptr<Manufactory> manufactory, uint32_t tax,
	uint8_t price_convocation, uint8_t price_maintenance)
	: id(id), economy(std::move(economy)), 
	church(std::move(charch)), 
	manufactory(std::move(manufactory)),
	tax(tax),
	price_convocation(price_convocation), 
	price_maintenance(price_maintenance) {

}

Town::~Town()
{
}

bool Town::isBankrupt() const
{
	return economy->getNetWorth() < 0;
}

void Town::processing()
{
	float multiplier = (church->getModifier() + economy->getMultiplier() - 1.f);
	int32_t income = 0;
	income += tax;
	income += manufactory->getIncome();
	income =  static_cast<int32_t>(std::ceil(income * multiplier));
	income -= cost_army;
	economy->add(income);
	economy->setIncome(income);
}

void Town::buildBuilding(const TypeBuildings type_building)
{
	if (type_building == TypeBuildings::Church) {
		if (economy->getBudget() >= church->getPrice()) {
			economy->expense(church->getPrice());
			church->build();
		}
	} else if (type_building == TypeBuildings::Manufactory) {
		if (economy->getBudget() >= manufactory->getPrice()) {
			economy->expense(manufactory->getPrice());
			manufactory->build();
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

bool Town::allocateForArmy(uint32_t soldiers)
{
	uint32_t cost = std::ceil(soldiers * 0.01);
	if (economy->getBudget() >= cost * price_convocation) {
		economy->sub(cost * price_convocation);
		cost_army = cost_army + (cost * price_maintenance);
		return true;
	}
	return false;
}

bool Town::deallocateForArmy(uint32_t soldiers)
{
	uint32_t cost = std::ceil(soldiers * 0.01);
	cost_army = cost_army - (cost * price_maintenance);
	return true;
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

	data.economy.tax = tax;
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

	data.army.cost_tick = cost_army;

	return data;
}