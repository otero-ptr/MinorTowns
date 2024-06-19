#include "NodeMapTown.h"

NodeMapTown::NodeMapTown(int town_id, int id, NODE_TYPE type) : NodeMap(id, type), town_id(town_id)
{
}

int NodeMapTown::getTownID() const
{
	return this->town_id;
}
