#pragma once
#include <vector>

class NodeMap;

class GameMap {
public:
	GameMap() = delete;
	GameMap(const int x_nodes, const int y_nodes);
	int getFreeSquarePosition();
	~GameMap();
private:
	void generate();
	int x_nodes;
	int y_nodes;
	std::vector<NodeMap> myMap;
};