#pragma once
#include <vector>
#include <memory>
#include <string>
#include "NodeMap\NodeMap.h"

class GameMap {
public:
	GameMap() = delete;
	GameMap(const int x_nodes, const int y_nodes);
	~GameMap();
	int getFreeSquarePosition();
	int getX();
	int getY();
	std::shared_ptr<NodeMap> operator[](int index) const;

	std::string getMapJson();
	void createMapJson();
private:
	void generate();
	int x_nodes;
	int y_nodes;
	std::vector<std::vector<std::shared_ptr<NodeMap>>> myMap;
	std::string mapJson;
};