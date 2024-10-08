#pragma once
#include <memory>
#include "Economy\Economy.h"
#include "Buildings\Church.h"
#include "Buildings\Manufactory.h"
#include "Buildings\TypeBuildings.h"

class User;

struct TownDetails {
	uint8_t town_id;
	struct EconomyData {
		double budget;
		double net_worth;
		int32_t income;
		float multiplier;
		uint32_t tax;
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
		uint32_t cost_tick;
	} army;
};

class Town {
public:
	Town() = delete;
	Town(uint8_t id, std::unique_ptr<Economy> economy,
		std::unique_ptr<Church> charch, 
		std::unique_ptr<Manufactory> manufactory, uint32_t tax,
		uint8_t price_convocation, uint8_t price_maintenance);
	~Town();
	bool isBankrupt() const;
	void processing();
	void buildBuilding(const TypeBuildings type_building);
	void destroyBuilding(const TypeBuildings type_building);
	bool allocateForArmy(uint32_t soldiers);
	bool deallocateForArmy(uint32_t soldiers);
	const uint8_t getID() const;
	bool operator<(const Town& other) const;
	const TownDetails getDetails() const;

	Town(Town&& other) noexcept = default;
	Town& operator=(Town&& other) noexcept = default;
private:
	std::unique_ptr<Economy> economy;
	std::unique_ptr<Church> church;
	std::unique_ptr<Manufactory> manufactory;
	uint32_t cost_army = 0;
	uint32_t tax;
	const uint8_t price_convocation;
	const uint8_t price_maintenance;
	uint8_t id;
};