#include "NodeMap.h"

NodeMap::NodeMap(uint8_t id) : id(id)
{

}

NodeMap::~NodeMap()
{
}

uint8_t NodeMap::getID() const
{
	return id;
}