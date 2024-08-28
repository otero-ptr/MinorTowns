#pragma once
#include <vector>
#include <memory>
#include <string>
#include <list>
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
	explicit GameMap(const int row, const int column);
	explicit GameMap(DimensionMap::SizeMap size);
	~GameMap();
	int getRow() const;
	int getColumn() const;
	std::string getMapJson();
	std::vector<uint8_t> placeTowns(std::vector<DimensionMap::SizeMap> &towns);
	std::list<uint8_t> buildWay(int from, int to);
private:
	void generate();
	void createMapJson();
	std::vector<std::vector<NodeMap>> map;
	int row;
	int column;
	std::string map_json;
};