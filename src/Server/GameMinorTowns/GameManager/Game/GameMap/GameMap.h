#pragma once
#include <vector>

class NodeMap;

class GameMap {
public:
	GameMap() = delete;
	GameMap(const int nodes);
	~GameMap();
private:
	void generate();
	int nodes;
	std::vector<NodeMap> myMap;
};