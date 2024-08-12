#pragma once
#include <vector>
#include <memory>
#include <string>
#include "NodeMap\NodeMap.h"

namespace DimensionMap {
	struct SizeMap {
		SizeMap(int x, int y) { this->x = x, this->y = y; }
		int x;
		int y;
	};
	DimensionMap::SizeMap detect(const size_t players);
	std::vector<DimensionMap::SizeMap> placeTowns(const size_t players, DimensionMap::SizeMap size);
}

class GameMap {
public:
	GameMap() = delete;
	GameMap(const int row, const int column);
	GameMap(DimensionMap::SizeMap size);
	~GameMap();
	int getRow();
	int getColumn();
	std::shared_ptr<NodeMap> operator[](int index) const;
	std::string getMapJson();
	std::vector<int> placeTowns(std::vector<DimensionMap::SizeMap> &towns);
private:
	void generate();
	void createMapJson();
	int row_nodes;
	int column_nodes;
	std::vector<std::vector<std::shared_ptr<NodeMap>>> map;
	std::string map_json;
};