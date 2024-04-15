#pragma once

enum NODE_TYPE {
	DEFAULT
};

class NodeMap {
public:
	NodeMap(int id, NODE_TYPE type);
	~NodeMap();
	const int getID();
private:
	int id;
	NODE_TYPE type;
};