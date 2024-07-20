#pragma once
#include <cstdint>
#include <string>

namespace Params {
	struct Params {

	};

	struct CreateGame : public Params {
		uint8_t max_users;
	};

	struct JoinLobby : public Params {
		std::string uuid_lobby;
	};

	struct RaiseArmy : public Params {
		uint32_t soldiers;
	};

	struct DisbandArmy : public Params {
		uint32_t soldiers;
	};

	struct BuildBuildings : public Params {
		uint32_t building_id;
	};
};