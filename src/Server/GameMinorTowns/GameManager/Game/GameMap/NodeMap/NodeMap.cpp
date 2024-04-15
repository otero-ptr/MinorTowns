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
