#include "Buildings.h"

void Buildings::build(const int& type)
{
	if (type == TypeBuilding::Church) {
		this->church += 1;
	}
	else if (type == TypeBuilding::Manufactory) {
		this->manufactory += 1;
	}
}

void Buildings::destroy(const int& type)
{
	if (type == TypeBuilding::Church) {
		if (this->church > 0) {
			this->church -= 1;
		}
	}
	else if (type == TypeBuilding::Manufactory) {
		if (this->manufactory > 0) {
			this->manufactory -= 1;
		}
	}
}

bool Buildings::isTypeExists(const int& type)
{
	if (type < 0) {
		return false;
	}
	return type < static_cast<int>(TypeBuilding::MaxValue);
}

int Buildings::getCountBuildings(const int& type)
{
	if (type == TypeBuilding::Church) {
		return this->church;
	}
	else if (type == TypeBuilding::Manufactory) {
		return this->manufactory;
	}
	return -1;
}

void Buildings::setPriceBuildings(const int& type, double value)
{
	if (type == TypeBuilding::Church) {
		this->priceChurch = value;
	}
	else if (type == TypeBuilding::Manufactory) {
		this->priceManufactory = value;
	}
}

double Buildings::getPriceBuildings(const int& type)
{
	if (type == TypeBuilding::Church) {
		return this->priceChurch;
	}
	else if (type == TypeBuilding::Manufactory) {
		return this->priceManufactory;
	}
}