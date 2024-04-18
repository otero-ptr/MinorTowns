#include "GameMap.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Dynamic/Var.h"

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
}

std::string GameMap::getMapJson()
{
	return this->mapJson;
}

void GameMap::createMapJson()
{
	Poco::JSON::Object json;
	Poco::JSON::Array rowArr;
	for (auto& row : this->myMap) {
		Poco::JSON::Array colArr;
		for (auto& col : row) {
			Poco::JSON::Object objJson;
			objJson.set("id", col->getID());
			objJson.set("type", static_cast<int>(col->getType()));
			colArr.add(objJson);
		}
		rowArr.add(colArr);
	}
	json.set("map", rowArr);
	std::ostringstream oss;
	json.stringify(oss);
	this->mapJson = oss.str();
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