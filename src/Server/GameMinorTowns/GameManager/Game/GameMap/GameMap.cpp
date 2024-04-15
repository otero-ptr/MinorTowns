#include "GameMap.h"
#include "NodeMap\NodeMap.h"
#include <iostream>

GameMap::GameMap(const int nodes)
{
	this->nodes = nodes;
	this->generate();
}

GameMap::~GameMap()
{
}

void GameMap::generate()
{
	this->myMap.reserve(this->nodes);
	for (int i = 0; i < nodes; i++) {
		this->myMap.push_back(NodeMap(i, NODE_TYPE::DEFAULT));
	}
}
