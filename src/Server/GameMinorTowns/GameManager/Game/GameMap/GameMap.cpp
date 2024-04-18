#include "GameMap.h"
#include <iostream>

GameMap::GameMap(const int x_nodes, const int y_nodes) : myMap(y_nodes)
{
	this->x_nodes = x_nodes;
	this->y_nodes = y_nodes;
	this->generate();
}

int GameMap::getFreeSquarePosition()
{
	if (!this->myMap.at(0).at(0)->isTown()) {
		this->myMap.at(0).at(0)->occupyTown();
		return this->myMap.at(0).at(0)->getID();
	}
	else if (!this->myMap.at(0).at(this->x_nodes-1)->isTown()) {
		this->myMap.at(0).at(this->x_nodes - 1)->occupyTown();
		return this->myMap.at(0).at(this->x_nodes - 1)->getID();
	}
	else if (!this->myMap.at(this->y_nodes -1).at(0)->isTown()) {
		this->myMap.at(this->y_nodes - 1).at(0)->occupyTown();
		return this->myMap.at(0).at(this->x_nodes - 1)->getID();
	}
	else if (!this->myMap.at(this->y_nodes -1).at(this->x_nodes - 1)->isTown()) {
		this->myMap.at(this->y_nodes - 1).at(this->x_nodes - 1)->occupyTown();
		return this->myMap.at(0).at(this->x_nodes - 1)->getID();
	}
	else {
		return -1;
	}
}

int GameMap::getX()
{
	return this->x_nodes;
}

int GameMap::getY()
{
	return this->y_nodes;
}

std::shared_ptr<NodeMap> GameMap::operator[](int index) const
{
	if (index > this->x_nodes * this->y_nodes || index < 0) {
		return nullptr;
	}
	int i = 0;
	int j = this->y_nodes - 1;
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
	{
		for (int i = 0; i < this->x_nodes; i++) {
			for (int j = 0; j < this->y_nodes; j++) {
				std::cout << this->myMap[i][j]->getType() << "\t";
			}
			std::cout << std::endl;
		}
	}
}

void GameMap::generate()
{
	int id = 0;
	for (int y = 0; y < this->y_nodes; ++y) {
		for (int x = 0; x < this->x_nodes; ++x) {
			this->myMap.at(y).push_back(std::make_shared<NodeMap>(id++, NODE_TYPE::DEFAULT));
		}
	}
}