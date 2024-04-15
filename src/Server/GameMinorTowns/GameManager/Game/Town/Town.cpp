#include "Town.h"

Town::Town(std::shared_ptr<User> user, int capitalNode) : economy(100)
{
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
