#pragma once
#include "NodeMap.h"

class NodeMapTown : public NodeMap {
public:
	NodeMapTown(int town_id, int id, NODE_TYPE type);
	int getTownID() const;
private:
	int town_id;
};