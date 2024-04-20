#include "Game.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "User/User.h"
#include <iostream>

Game::Game(std::vector<std::shared_ptr<User>> users)
{
	this->users = users;
	this->init();
	for (auto& user : users) {
		user->messagePool.pushBackMessage(this->gameMap->getMapJson());
	}
	this->active = true;
	this->thTick = std::jthread(&Game::tick, this);
}

Game::~Game()
{
	this->active = false;
	if (this->thTick.joinable()) {
		this->thTick.join();
	}
}

const std::string Game::getUUID()
{
	return this->uuid;
}

bool Game::isActive()
{
	return this->active;
}

void Game::tick()
{
	int cooldown = 5000;
	while (this->active) {
		++tickCount;
		auto start = std::chrono::steady_clock::now();

		std::cout << "this game tick [UUID]: " << this->uuid << std::endl;
		for (auto& user : this->users) {
			user->messagePool.pushBackMessage("tick" + std::to_string(this->tickCount));
		}

		auto end = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		std::this_thread::sleep_for(std::chrono::milliseconds(cooldown - duration.count()));
	}
}

void Game::init()
{
	this->createUUID();
	std::vector<int> idTowns = this->createMap();
	this->createTowns(idTowns);
}

void Game::createUUID()
{
	Poco::UUIDGenerator generator;
	this->uuid = generator.create().toString();
}

void Game::createTowns(std::vector<int> &idTowns)
{
	this->towns.reserve(this->users.size());
	for (int i = 0; i < users.size(); i++) {
		this->towns.push_back(std::make_unique<Town>(this->users[i], idTowns[i]));
	}
}

std::vector<int> Game::createMap()
{
	auto sizeMap = DimensionMap::detect(this->users.size());
	auto posTowns = DimensionMap::placeTowns(this->users.size(), sizeMap);
	this->gameMap = std::make_unique<GameMap>(sizeMap);
	return this->gameMap->placeTowns(posTowns);
}
