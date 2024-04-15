#include "GameMap.h"
#include "NodeMap\NodeMap.h"

GameMap::GameMap(const int x_nodes, const int y_nodes)
{
	this->x_nodes = x_nodes;
	this->y_nodes = y_nodes;
	this->generate();
}

int GameMap::getFreeSquarePosition()
{
	if (!this->myMap.at(0).isTown()) {
		this->myMap.at(0).occupyTown();
		return 0;
	}
	else if (!this->myMap.at(this->x_nodes-1).isTown()){
		this->myMap.at(this->x_nodes - 1).occupyTown();
		return this->x_nodes - 1;
	}
	else if (!this->myMap.at(this->x_nodes * this->y_nodes- this->x_nodes).isTown()) {
		this->myMap.at(this->x_nodes * this->y_nodes - this->x_nodes).occupyTown();
		return this->x_nodes * this->y_nodes - this->x_nodes;
	}
	else if (!this->myMap.at(this->x_nodes * this->y_nodes - 1).isTown()) {
		this->myMap.at(this->x_nodes * this->y_nodes - 1).occupyTown();
		return this->x_nodes * this->y_nodes - 1;
	}
	else {
		return -1;
	}
}

GameMap::~GameMap()
{
}

void GameMap::generate()
{
	int total = this->x_nodes * this->y_nodes;
	this->myMap.reserve(total);
	for (int i = 0; i < total; i++) {
		this->myMap.push_back(NodeMap(i, NODE_TYPE::DEFAULT));
	}
}
