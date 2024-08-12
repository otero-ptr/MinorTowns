#include "GameMap.h"
#include "nlohmann\json.hpp"
#include "NodeMap\NodeMapTown.h"

GameMap::GameMap(const int row, const int column) : map(row)
{
	this->row_nodes = row;
	this->column_nodes = column;
	this->generate();
}

GameMap::GameMap(DimensionMap::SizeMap size) : map(size.x)
{
	this->row_nodes = size.x;
	this->column_nodes = size.y;
	this->generate();
}

int GameMap::getRow()
{
	return this->row_nodes;
}

int GameMap::getColumn()
{
	return this->column_nodes;
}

std::shared_ptr<NodeMap> GameMap::operator[](int index) const
{
	if (index > this->row_nodes * this->column_nodes || index < 0) {
		return nullptr;
	}
	int i = 0;
	int j = this->column_nodes - 1;
	while (map[i][j]->getID() != index) {
		if (map[i][j]->getID() > index) {
			--j;
		}
		else if (map[i][j]->getID() < index) {
			++i;
		}
	}
	return map[i][j];
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
	nlohmann::json jsonObj;
	for (int n = 0; n < map.size(); ++n) {
		for (int m = 0; m < map[m].size(); ++m) {
			jsonObj["map"][n][m]["id"] = map[n][m]->getID();
			jsonObj["map"][n][m]["type"] = static_cast<int>(map[n][m]->getType());
			if (map[n][m]->getType() == NODE_TYPE::TOWN) {
				std::shared_ptr<NodeMapTown> townPtr = std::dynamic_pointer_cast<NodeMapTown>(map[n][m]);
				if (townPtr) {
					jsonObj["map"][n][m]["town_id"] = townPtr->getTownID();
				}

			}
		}
	}
	map_json = jsonObj.dump();
}

std::vector<int> GameMap::placeTowns(std::vector<DimensionMap::SizeMap> &towns)
{
	std::vector<int> ids;
	for (int i = 0; i < towns.size(); ++i) {
		map[towns[i].x][towns[i].y] = std::make_shared<NodeMapTown>(i, map[towns[i].x][towns[i].y]->getID(), NODE_TYPE::TOWN);
		ids.push_back(map[towns[i].x][towns[i].y]->getID());
	}
	this->createMapJson();
	return ids;
}

void GameMap::generate()
{
	int id = 0;
	for (int n = 0; n < this->row_nodes; ++n) {
		for (int m = 0; m < this->column_nodes; ++m) {
			map.at(n).push_back(std::make_shared<NodeMap>(id++, NODE_TYPE::DEFAULT));
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
