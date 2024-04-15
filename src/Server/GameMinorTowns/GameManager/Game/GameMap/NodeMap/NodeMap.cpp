#include "NodeMap.h"

NodeMap::NodeMap(int id, NODE_TYPE type)
{
	this->id = id;
	this->type = type;
}

NodeMap::~NodeMap()
{
}

const int NodeMap::getID()
{
	return this->id;
}

bool NodeMap::occupyTown()
{
	if (this->isTown()) {
		return false;
	}
	else {
		this->type = NODE_TYPE::TOWN;
		return true;
	}
}

bool NodeMap::isTown()
{
	if (this->type == NODE_TYPE::TOWN) {
		return true;
	}
	else {
		return false;
	}
}
