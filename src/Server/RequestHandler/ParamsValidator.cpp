#include "ParamsValidator.h"
#include <algorithm>
#include <cctype>  

const Params::Params* ParamsValidator::validate(const Params::Params* params)
{
	return nullptr;
}

const Params::CreateLobby* ParamsValidator::validate(const Params::CreateLobby* params)
{
	if (params->max_users < 2 || params->max_users > 4) {
		return nullptr;
	}
	return params;
}

const Params::JoinLobby* ParamsValidator::validate(const Params::JoinLobby* params)
{
	if (params->uuid_lobby.length() != 36) {
		return nullptr;
	}
	if (!tool::isValidString(params->uuid_lobby)) {
		return nullptr;
	}
	return params;
}

const Params::RaiseArmy* ParamsValidator::validate(const Params::RaiseArmy* params)
{
	if (params->soldiers < 0 || params->soldiers > 1'000'000) {
		return nullptr;
	}
	return params;
}

const Params::DisbandArmy* ParamsValidator::validate(const Params::DisbandArmy* params)
{
	if (params->soldiers < 0 || params->soldiers > 1'000'000) {
		return nullptr;
	}
	return params;
}

const Params::BuildBuildings* ParamsValidator::validate(const Params::BuildBuildings* params)
{
	return params;
}

bool ParamsValidator::tool::isValidCharacter(char c)
{
	return std::isalnum(static_cast<unsigned char>(c)) || c == '-';
}

bool ParamsValidator::tool::isValidString(const std::string& str)
{
	return std::all_of(str.begin(), str.end(), isValidCharacter);
}
