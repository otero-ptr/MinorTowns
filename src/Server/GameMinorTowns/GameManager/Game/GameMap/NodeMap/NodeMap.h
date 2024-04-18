#pragma once

enum NODE_TYPE {
	NONE,
	DEFAULT,
	TOWN
};

class NodeMap {
public:
	NodeMap(int id, NODE_TYPE type);
	~NodeMap();
	const int getID() const;
	NODE_TYPE getType() const;
	bool occupyTown();
	bool isTown() const;
private:
	int id;
	NODE_TYPE type;
};