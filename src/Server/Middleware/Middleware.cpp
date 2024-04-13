#include "Middleware.h"
#include "Poco\JSON\JSON.h"
#include "Poco\JSON\Parser.h"
#include "GameMinorTowns/GameMinorTowns.h"
#include <thread>

Middleware::Middleware(std::shared_ptr<GameMinorTowns> game)
{
    this->gameMinorTowns = game;
}

Middleware::~Middleware()
{
}

std::shared_ptr<User> Middleware::Authorization(std::string username, std::string address, unsigned short port)
{
    if (this->authValidate.checkUsername(username)) {
        return nullptr;
    }
	std::shared_ptr<User> user;
	user = std::make_shared<User>(username, address + ":" + std::to_string(port));
	return user;
}

ACTION_STATUS Middleware::action(std::string jsonMessage, std::shared_ptr<User> user)
{
    std::cout << "Middleware" << std::endl;
    try {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result;
        result = parser.parse(jsonMessage);
        Poco::JSON::Object::Ptr jsonObj = result.extract<Poco::JSON::Object::Ptr>();

        if (jsonObj->has("action")) {
            Poco::Dynamic::Var value = jsonObj->get("action");
            std::string actionTODO = value.toString();
            std::cout << "Value of action: " << actionTODO << std::endl;
            if (actionTODO == "create_game") {
                std::thread th(&GameMinorTowns::createGame, this->gameMinorTowns, user, 3 /* added parametr from JSON */);
                th.detach();
            }
            else if (actionTODO == "join_lobby") {
                std::thread th(&GameMinorTowns::joinLobby, this->gameMinorTowns, user, "adwada" /* added parametr from JSON */);
                th.detach();
            } 
            else if (actionTODO == "leaveLobby") {
                std::thread th(&GameMinorTowns::leaveLobby, this->gameMinorTowns, user);
                th.detach();
            }
        }
        else {
            return ACTION_STATUS::ST_ERROR;
        }
    }
    catch (Poco::Exception&) {
        return ACTION_STATUS::ST_ERROR;
    }
	return ACTION_STATUS::ST_OK;
}
