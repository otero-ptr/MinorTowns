#include "Army.h"
#include <stdexcept>


Army::Army(int soldiers, int ownTownId)
{	
	if (soldiers >= 0) {
		this->soldiers = soldiers;
	}
	else {
		throw std::invalid_argument("The value cannot be less than one.");
	}
	this->ownTownId = ownTownId;
	this->idNode = ownTownId;
}

Army::~Army()
{
}

void Army::detach(const int soldiers)
{
	if (this->ownTownId == this->idNode) {
		if (soldiers >= this->soldiers) {
			this->soldiers -= soldiers;
		}
		else {
			throw std::invalid_argument("Cannot detach more soldiers than in the army.");
		}
	}
}

void Army::merge(const int soldiers)
{
	if (this->ownTownId == this->idNode) {
		this->soldiers += soldiers;
	}
	else {
		throw std::invalid_argument("The army is not located in the capital.");
	}
}

const int Army::getNode()
{
	return this->idNode;
}

const int Army::getCount()
{
	return this->soldiers;
}
