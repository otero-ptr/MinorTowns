#pragma once
#include "params.h"

namespace ParamsValidator {
	namespace tool {
		bool isValidCharacter(char c);
		bool isValidString(const std::string& str);
	};
	const Params::Params* validate(const Params::Params* params);
	const Params::CreateLobby* validate(const Params::CreateLobby* params);
	const Params::JoinLobby* validate(const Params::JoinLobby* params);
	const Params::RaiseArmy* validate(const Params::RaiseArmy* params);
	const Params::DisbandArmy* validate(const Params::DisbandArmy* params);
	const Params::BuildBuildings* validate(const Params::BuildBuildings* params);
};