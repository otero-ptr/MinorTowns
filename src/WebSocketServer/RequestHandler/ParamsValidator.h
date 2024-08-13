#pragma once
#include "params.h"
#include <memory>

namespace ParamsValidator {
	namespace tool {
		bool isValidCharacter(char c);
		bool isValidString(const std::string& str);
	};
	bool validate(std::shared_ptr<Params::Params>& params);
	bool validate(std::shared_ptr<Params::CreateLobby>& params);
	bool validate(std::shared_ptr<Params::JoinLobby>& params);
	bool validate(std::shared_ptr<Params::RaiseArmy>& params);
	bool validate(std::shared_ptr<Params::DisbandArmy>& params);
	bool validate(std::shared_ptr<Params::BuildBuildings>& params);
};