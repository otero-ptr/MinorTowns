#include "GameMap.h"
#include "nlohmann\json.hpp"
#include "NodeMap\NodeMapTown.h"

GameMap::GameMap(const int row, const int column) : myMap(row)
{
	this->row_nodes = row;
	this->column_nodes = column;
	this->generate();
}

GameMap::GameMap(DimensionMap::SizeMap size) : myMap(size.x)
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
	while (this->myMap[i][j]->getID() != index) {
		if (this->myMap[i][j]->getID() > index) {
			--j;
		}
		else if (this->myMap[i][j]->getID() < index) {
			++i;
		}
	}
	return this->myMap[i][j];
}

GameMap::~GameMap()
{
}

std::string GameMap::getMapJson()
{
	return this->mapJson;
}

void GameMap::createMapJson()
{
	nlohmann::json jsonObj;
	for (int x = 0; x < this->myMap.size(); ++x) {
		for (int y = 0; y < this->myMap[x].size(); ++y) {
			jsonObj["map"][x][y]["id"] = this->myMap[x][y]->getID();
			jsonObj["map"][x][y]["type"] = static_cast<int>(this->myMap[x][y]->getType());
			if (this->myMap[x][y]->getType() == NODE_TYPE::TOWN) {
				std::shared_ptr<NodeMapTown> townPtr = std::dynamic_pointer_cast<NodeMapTown>(this->myMap[x][y]);
				if (townPtr) {
					jsonObj["map"][x][y]["town_id"] = townPtr->getTownID();
				}

			}
		}
	}
	this->mapJson = jsonObj.dump();
}

std::vector<int> GameMap::placeTowns(std::vector<DimensionMap::SizeMap> &towns)
{
	std::vector<int> ids;
	for (int i = 0; i < towns.size(); ++i) {
		this->myMap[towns[i].x][towns[i].y] = std::make_shared<NodeMapTown>(i, this->myMap[towns[i].x][towns[i].y]->getID(), NODE_TYPE::TOWN);
		ids.push_back(this->myMap[towns[i].x][towns[i].y]->getID());
	}
	this->createMapJson();
	return ids;
}

void GameMap::generate()
{
	int id = 0;
	for (int x = 0; x < this->row_nodes; ++x) {
		for (int y = 0; y < this->column_nodes; ++y) {
			this->myMap.at(x).push_back(std::make_shared<NodeMap>(id++, NODE_TYPE::DEFAULT));
		}
	}
}

DimensionMap::SizeMap DimensionMap::detect(const int players)
{
	if (players == 2) {
		return SizeMap(5, 3);
	}
	else if (players == 3) {
		return SizeMap(5, 4);
	}
	else if (players == 4) {
		return SizeMap(5, 5);
	}
}

std::vector<DimensionMap::SizeMap> DimensionMap::placeTowns(const int players, DimensionMap::SizeMap size)
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
		ids.push_back(SizeMap(2, 3));
	}
	else if (players == 4) {
		ids.push_back(SizeMap(0, 0));
		ids.push_back(SizeMap(4, 0));
		ids.push_back(SizeMap(0, 4));
		ids.push_back(SizeMap(4, 4));
	}
	return ids;
}
