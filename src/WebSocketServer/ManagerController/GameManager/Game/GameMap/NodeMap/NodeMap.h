#pragma once
#include "NodeType.h"

class NodeMap {
public:
	NodeMap(int id, NODE_TYPE type);
	virtual ~NodeMap();
	int getID() const;
	NODE_TYPE getType() const;
private:
	int id;
	NODE_TYPE type;
};