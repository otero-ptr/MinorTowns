#pragma once

enum NODE_TYPE {
	DEFAULT,
	TOWN
};

class NodeMap {
public:
	NodeMap(int id, NODE_TYPE type);
	~NodeMap();
	const int getID();
	bool occupyTown();
	bool isTown();
private:
	int id;
	NODE_TYPE type;
};