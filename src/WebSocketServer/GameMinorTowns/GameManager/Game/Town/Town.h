#pragma once
#include <memory>
#include "Economy\Economy.h"
#include "Buildings\Church.h"
#include "Buildings\Manufactory.h"
#include "Buildings\TypeBuildings.h"

class User;

struct TownData {
	uint8_t town_id;
	std::string username;
	struct EconomyData {
		double budget;
		double net_worth;
		int32_t income;
		float multiplier;
	} economy;
	struct CharchData {
		uint32_t count;
		uint32_t price;
		float bonus;
	} charch;
	struct ManufactoryData {
		uint32_t count;
		uint32_t price;
		uint32_t bonus;
	} manufactory;
	struct ArmyData {
		int count_soldiers;
		int locate_node;
		int32_t cost;
	} army;
};

class Town {
public:
	Town() = delete;
	Town(int id, std::shared_ptr<User> user, 
		int capital_node, std::unique_ptr<Economy> economy,
		std::unique_ptr<Church> charch, 
		std::unique_ptr<Manufactory> manufactory);
	~Town();
	const int getCapitalNodeId();
	void TownTickProcessing();
	void buildBuilding(const TypeBuildings type_building);
	void destroyBuilding(const TypeBuildings type_building);
	std::shared_ptr<User> getOwnTown() const;
	const int getID() const;
	bool operator<(const Town& other) const;
	const TownData getData() const;

	Town(Town&& other) noexcept = default;
	Town& operator=(Town&& other) noexcept = default;
private:
	int id;
	int capital_node;
	std::unique_ptr<Economy> economy;
	std::unique_ptr<Church> church;
	std::unique_ptr<Manufactory> manufactory;
	std::shared_ptr<User> own;
};