#include "Game.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "User/User.h"

Game::Game(std::vector<std::shared_ptr<User>> users)
{
	this->users = users;
	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	this->uuid = uuid.toString();
	this->gameMap = std::make_unique<GameMap>(25);
}

const std::string Game::getUUID()
{
	return this->uuid;
}
