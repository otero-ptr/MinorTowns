#include "GameMinorTowns.h"
#include "GameMinorTowns\GameManager\GameManager.h"
#include "GameMinorTowns\LobbyManager\LobbyManager.h"

GameMinorTowns::GameMinorTowns()
{
	this->gameManager = std::make_unique<GameManager>();
	this->lobbyManager = std::make_unique<LobbyManager>();
}
