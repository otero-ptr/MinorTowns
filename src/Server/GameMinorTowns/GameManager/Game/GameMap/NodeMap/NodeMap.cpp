#include "NodeMap.h"

NodeMap::NodeMap(int id, NODE_TYPE type)
{
	this->id = id;
	this->type = type;
}

NodeMap::~NodeMap()
{
}

int NodeMap::getID() const
{
	return this->id;
}

NODE_TYPE NodeMap::getType() const
{
	return this->type;
}