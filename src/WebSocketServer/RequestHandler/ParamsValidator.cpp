#include "ParamsValidator.h"
#include <algorithm>
#include <cctype>  

bool ParamsValidator::validate(std::shared_ptr<Params::Params>& params)
{
	return true;
}

bool ParamsValidator::validate(std::shared_ptr<Params::CreateLobby>& params)
{
	if (params->max_users < 2 || params->max_users > 4) {
		return true;
	}
	return false;
}

bool ParamsValidator::validate(std::shared_ptr<Params::JoinLobby>& params)
{
	if (params->uuid_lobby.length() != 36) {
		return true;
	}
	if (!tool::isValidString(params->uuid_lobby)) {
		return true;
	}
	return false;
}

bool ParamsValidator::validate(std::shared_ptr<Params::RaiseArmy>& params)
{
	if (params->soldiers < 0 || params->soldiers > 1'000'000) {
		return true;
	}
	return false;
}

bool ParamsValidator::validate(std::shared_ptr<Params::DisbandArmy>& params)
{
	if (params->soldiers < 0 || params->soldiers > 1'000'000) {
		return true;
	}
	return false;
}

bool ParamsValidator::validate(std::shared_ptr<Params::BuildBuildings>& params)
{
	return false;
}

bool ParamsValidator::tool::isValidCharacter(char c)
{
	return std::isalnum(static_cast<unsigned char>(c)) || c == '-';
}

bool ParamsValidator::tool::isValidString(const std::string& str)
{
	return std::all_of(str.begin(), str.end(), isValidCharacter);
}
