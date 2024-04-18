#include "NodeMap.h"

NodeMap::NodeMap(int id, NODE_TYPE type)
{
	this->id = id;
	this->type = type;
}

NodeMap::~NodeMap()
{
}

const int NodeMap::getID() const
{
	return this->id;
}

NODE_TYPE NodeMap::getType() const
{
	return this->type;
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

bool NodeMap::isTown() const
{
	if (this->type == NODE_TYPE::TOWN) {
		return true;
	}
	else {
		return false;
	}
}
