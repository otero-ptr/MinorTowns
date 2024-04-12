#include "Middleware.h"
#include "Poco\JSON\JSON.h"
#include "Poco\JSON\Parser.h"
Middleware::Middleware()
{
}

Middleware::~Middleware()
{
}

std::shared_ptr<User> Middleware::Authourzation(std::string username, std::string address, std::string port)
{
	std::shared_ptr<User> user;
	user = std::make_shared<User>(username, address + ":" + port);
	return user;
}

bool Middleware::isJson(const std::string& str) {
    Poco::JSON::Parser parser;
    try {
        parser.parse(str);
        return true;
    }
    catch (Poco::Exception&) {
        return false;
    }
}

ACTION_STATUS Middleware::action(std::string jsonMessage, std::shared_ptr<User> user)
{
    std::cout << "Middleware" << std::endl;
    try {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result;
        result = parser.parse(jsonMessage);
        Poco::JSON::Object::Ptr jsonObj = result.extract<Poco::JSON::Object::Ptr>();

        // Получаем значение по ключу
        if (jsonObj->has("action")) {
            Poco::Dynamic::Var value = jsonObj->get("action");
            std::string actionTODO = value.toString();
            std::cout << "Value of action: " << actionTODO << std::endl;
            if (actionTODO == "create_lobby") {

            }
            else if (actionTODO == "join_lobby") {

            } 
            else if (actionTODO == "") {

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
