#include "GameMap.h"
#include "nlohmann\json.hpp"

GameMap::GameMap(const int row, const int column) : 
	map(row), row(row), column(column)
{
	generate();
}

GameMap::GameMap(DimensionMap::SizeMap size) : 
	map(size.x), row(size.x), column(size.y)
{
	generate();
}

int GameMap::getRow() const
{
	return row;
}

int GameMap::getColumn() const
{
	return column;
}

std::list<uint8_t> GameMap::buildWay(int from, int to)
{
	std::list<uint8_t> way;
	int current_x = from / column;
	int current_y = from % column;
	int target_x = to / column;
	int target_y = to % column;
	
	bool diagonal = false;
	while ((current_x != target_x) || (current_y != target_y)) {
		if (diagonal) {
			if (current_x > target_x) {
				--current_x;
			}
			else if (current_x < target_x) {
				++current_x;
			}
		}
		else {
			if (current_y > target_y) {
				--current_y;
			}
			else if (current_y < target_y) {
				++current_y;
			}
		}
		if (current_x == target_x) {
			diagonal = false;
		}
		else if (current_y == target_y) {
			diagonal = true;
		}
		else {
			diagonal = !diagonal;
		}
		way.push_back(map[current_x][current_y].getID());
	}
	return way;
}

GameMap::~GameMap()
{
}

std::string GameMap::getMapJson()
{
	return map_json;
}

void GameMap::createMapJson()
{
	nlohmann::json json_obj;
	for (size_t n = 0; n < row; ++n) {
		for (size_t m = 0; m < column; ++m) {
			json_obj["map"][n][m]["id"] = map[n][m].getID();
		}
	}
	map_json = json_obj.dump();
}

std::vector<uint8_t> GameMap::placeTowns(std::vector<DimensionMap::SizeMap> &towns)
{
	std::vector<uint8_t> ids;
	for (size_t i = 0; i < towns.size(); ++i) {
		ids.push_back(map[towns[i].x][towns[i].y].getID());
	}
	createMapJson();
	return ids;
}

void GameMap::generate()
{
	uint8_t id = 0;
	for (size_t n = 0; n < row; ++n) {
		for (size_t m = 0; m < column; ++m) {
			map.at(n).emplace_back(NodeMap(id++));
		}
	}
}

DimensionMap::SizeMap DimensionMap::detect(const size_t players)
{
	if (players == 2) {
		return SizeMap(5, 3);
	}
	else if (players == 3) {
		return SizeMap(5, 3);
	}
	else if (players == 4) {
		return SizeMap(5, 5);
	}
	return SizeMap(0, 0);
}

std::vector<DimensionMap::SizeMap> DimensionMap::placeTowns(const size_t players, DimensionMap::SizeMap size)
{
	std::vector<DimensionMap::SizeMap> ids;
	ids.reserve(players);
	if (players == 2) {
		ids.push_back(SizeMap(0, 1));
		ids.push_back(SizeMap(4, 1));
	}
	else if (players == 3) {
		ids.push_back(SizeMap(0, 0));
		ids.push_back(SizeMap(4, 0));
		ids.push_back(SizeMap(2, 2));
	}
	else if (players == 4) {
		ids.push_back(SizeMap(0, 0));
		ids.push_back(SizeMap(4, 0));
		ids.push_back(SizeMap(0, 4));
		ids.push_back(SizeMap(4, 4));
	}
	return ids;
}
